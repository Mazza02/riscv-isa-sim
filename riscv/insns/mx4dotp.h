require_extension('F');

// Fetch 64-bit packed elements from integer or vector registers
reg_t packed_elements_a = RS1;
reg_t packed_elements_b = RS2;
reg_t scales = RS3;

// Unpack E8M0 scales (e.g. scale_A in bits 7:0, scale_B in bits 15:8)
mx_scale_e8m0_t scale_a(scales & 0xFF);
mx_scale_e8m0_t scale_b((scales >> 8) & 0xFF);

// Unpack the 16 x FP4 elements
omxfp4_e2m1 a[16];
omxfp4_e2m1 b[16];
for (int i = 0; i < 16; i++) {
  a[i] = omxfp4_e2m1((packed_elements_a >> (i * 4)) & 0xF);
  b[i] = omxfp4_e2m1((packed_elements_b >> (i * 4)) & 0xF);
}

// number of elements + mantissa bits for normalized dot product
DotConfig cfg(16, 4);
bulk_norm_out_t dot_res = bulk_norm_dot_mxfp(cfg, a, b, scale_a, scale_b);

//fetch accumulation from FRS1, add to dot product result, and write back to FRD
float32_t c_in = f32(FRS1);
float32_t c_out = f32_add(c_in, f32(dot_res.out));
WRITE_FRD(c_out);