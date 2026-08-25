require_extension('F');

// Fetch 64-bit packed elements from integer or vector registers
reg_t packed_elements_a = RS1;
reg_t packed_elements_b = RS2;
reg_t scales = RS3;

// Unpack E8M0 scales (e.g. scale_A in bits 7:0, scale_B in bits 15:8)
mx_scale_e8m0_t scale_a(scales & 0xFF);
mx_scale_e8m0_t scale_b((scales >> 8) & 0xFF);

// Unpack the 8 x FP8 elements
omxfp8_e4m3 a[8];
omxfp8_e4m3 b[8];
for (int i = 0; i < 8; i++) {
  a[i] = omxfp8_e4m3((packed_elements_a >> (i * 8)) & 0xFF);
  b[i] = omxfp8_e4m3((packed_elements_b >> (i * 8)) & 0xFF);
}

// number of elements + mantissa bits for normalized dot product
DotConfig cfg(8, 4);
bulk_norm_out_t dot_res = bulk_norm_dot_mxfp(cfg, a, b, scale_a, scale_b);

//fetch accumulation from FRS1, add to dot product result, and write back to FRD
float32_t c_in = f32(FRS1);
float32_t c_out = f32_add(c_in, f32(dot_res.out));
WRITE_FRD(c_out);