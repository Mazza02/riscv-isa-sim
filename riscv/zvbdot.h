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

static inline float32_t vq4dotp_dot_acc(const std::vector<uint8_t>& a, const std::vector<int8_t>& b, float16_t sa, float16_t sb, float32_t c)
{
  // widen each byte to int8 weights, low first, then high
  std::vector<int8_t> w(2 * a.size());
  for (size_t j = 0; j < a.size(); j++) {
    w[j] = int8_t((a[j] & 0xF) - 8);
    w[j + a.size()] = int8_t((a[j] >> 4) - 8);
  }

  // int8 x int8 products in 32 bits
  int32_t sumi = 0;
  for (size_t i = 0; i < w.size(); i++)
    sumi += int32_t(w[i]) * int32_t(b[i]);

  // scale and add
  const float32_t scale = f32_mul(f16_to_f32(sa), f16_to_f32(sb));
  return f32_add(c, f32_mul(i32_to_f32(sumi), scale));
}


#endif
