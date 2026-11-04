#ifndef _RISCV_ZVBDOT_H
#define _RISCV_ZVBDOT_H

#include "bulknormdot.h"
#include <vector>
#include <algorithm>

static inline float32_t f32_add_odd(float32_t a, float32_t b)
{
  auto rm = softfloat_roundingMode;
  auto flags = softfloat_exceptionFlags;

  softfloat_roundingMode = softfloat_round_odd;
  softfloat_exceptionFlags = 0;

  auto res = f32_add(a, b);

  if (softfloat_exceptionFlags & softfloat_flag_overflow) {
    res.v++; // FLT_MAX -> INF
  }

  auto new_flags = softfloat_exceptionFlags & (softfloat_flag_overflow | softfloat_flag_invalid);

  softfloat_roundingMode = rm;
  softfloat_exceptionFlags = flags | new_flags;

  return res;
}

static inline float32_t zvfwbdot16bf_dot_acc(const std::vector<uint16_t>& a, const std::vector<uint16_t>& b, float32_t c)
{
  std::vector<bf16_t> fa(a.size());
  std::transform(a.begin(), a.end(), fa.begin(), [](auto f) { return f; });

  std::vector<bf16_t> fb(b.size());
  std::transform(b.begin(), b.end(), fb.begin(), [](auto f) { return f; });

  DotConfig cfg(a.size(), int_log2(a.size()) + ((a.size() & (a.size() - 1)) != 0));
  auto res = bulk_norm_dot_bf16(cfg, &fa[0], &fb[0]);
  softfloat_exceptionFlags |= res.flags;
  return f32_add_odd(f32(res.out), c);
}

template<typename A, typename B>
float32_t zvfqbdot8f_dot_acc(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, float32_t c)
{
  std::vector<A> fa(a.size());
  std::transform(a.begin(), a.end(), fa.begin(), [](auto f) { return f; });

  std::vector<B> fb(b.size());
  std::transform(b.begin(), b.end(), fb.begin(), [](auto f) { return f; });

  DotConfig cfg(a.size(), int_log2(a.size()) + ((a.size() & (a.size() - 1)) != 0));
  auto res = bulk_norm_dot_ofp8(cfg, &fa[0], &fb[0]);
  softfloat_exceptionFlags |= res.flags;
  return f32_add_odd(f32(res.out), c);
}

template<typename A, typename B>
float32_t vmxdotp_dot_acc(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b, mx_scale_e8m0_t scale_a, mx_scale_e8m0_t scale_b,  float32_t c)
{
  std::vector<A> fa(a.size());
  std::transform(a.begin(), a.end(), fa.begin(), [](auto f) { return f; });

  std::vector<B> fb(b.size());
  std::transform(b.begin(), b.end(), fb.begin(), [](auto f) { return f; });

  DotConfig cfg(a.size(), int_log2(a.size()) + ((a.size() & (a.size() - 1)) != 0));
  auto res = bulk_norm_dot_mxfp(cfg, &fa[0], &fb[0], scale_a, scale_b);
  softfloat_exceptionFlags |= res.flags;
  return f32_add_odd(f32(res.out), c);
}


// static inline int32_t vq4dotp_dot(const std::vector<uint8_t>& a, const std::vector<int8_t>& b)
// {
//   int32_t sumi = 0;
//   // the low bits are all widened first and computed, then all the high bits are shifted and compute
//   std::vector<int8_t> w(2 * a.size());
//   for (size_t j = 0; j < w.size(); j++) {
//     if (j >= a.size()){
//       // assigns the longer vector register spaces and goes back on the packed weights, taking the higher bits requiring shifting back to normal position
//       w[j] = int8_t((a[j - a.size()] >> 4) - 8);
//     }else{
//       // turns all lower placed packed bits into int8 and places them in a new register at the start
//       w[j] = int8_t((a[j] & 0xF) - 8);
//     }
//     // int8 x int8 products in 32 bits
//     sumi += int32_t(w[j]) * int32_t(b[j]);
//   }
//   return sumi;
// }

static inline int32_t vq4dotp_dot(const std::vector<uint8_t>& a, const std::vector<int8_t>& b)
{
  const size_t half = a.size();  // number of packed bytes
  const size_t doubled = a.size() * 2; // to account for unpacking all bits
  int32_t sumi = 0;

  // has to go through both low-bit and high bit halves of q4
  for (size_t j = 0; j < doubled; j++) {
    size_t byte_index;
    int bit_index;
    if (j >= half) {
      byte_index = j - half;
      bit_index = 4;
    } else {
      byte_index = j;
      bit_index = 0;
    }

    // 4 bits, and its top bit inverted
    const uint8_t field = (a[byte_index] >> bit_index) & 0x0F;
    const uint8_t sign = ((field >> 3) & 1) ^ 1;

    // since we can't do mixed-precision here, byte output must be created via shifting..
    uint8_t weight = 0;
    weight |= field & 0x07;                 // 0-2 bits
    weight |= uint8_t(sign ? 0xF8 : 0x00);  // must copy sign to the rest..

    sumi += int32_t(int8_t(weight)) * int32_t(b[j]);
  }
  return sumi;
}

static inline std::vector<int32_t> vq6dotp42_dot(const std::vector<uint8_t>& a, const std::vector<int8_t>& b)
{
  const size_t vl = b.size();
  const size_t half = vl / 2;     // number of ql bytes
  const size_t quarter = vl / 4;  // number of qh bytes
  std::vector<int32_t> sums(vl / 16, 0);

  for (size_t n = 0; n < vl; n++) {
    // has to go through both low-bit and high bit halves of q4
    size_t ql_byte_index;
    int ql_bit_index;
    if (n >= half) {
      ql_byte_index = n - half;
      ql_bit_index = 4;
    } else {
      ql_byte_index = n;
      ql_bit_index = 0;
    }

    // this would be a buffer with the index of both ql and qh positions
    const size_t quarter_index = n / quarter; // 0-3
    const size_t qh_byte_index = half + (n - quarter_index * quarter);
    const int qh_bit_index = int(2 * quarter_index);

    // 4 + 2 bits, and its top bit inverted
    const uint8_t low = (a[ql_byte_index] >> ql_bit_index) & 0x0F;
    const uint8_t high = (a[qh_byte_index] >> qh_bit_index) & 0x03;
    const uint8_t sign = ((high >> 1) & 1) ^ 1;

    uint8_t weight = 0;
    weight |= low;                          
    weight |= uint8_t((high & 1) << 4);     // bit 4, last qh bit is sign
    weight |= uint8_t(sign ? 0xE0 : 0x00);  // remainder is sign

    // add to the partial sum of this weight's 16-weight group (since vl=256)
    // scales would have to be added to eliminate the partial sums
    sums[n / 16] += int32_t(int8_t(weight)) * int32_t(b[n]);
  }
  return sums;
}


#endif
