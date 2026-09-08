require_either_extension('F', 'V');

// Fetch 64-bit packed elements from integer or vector registers
reg_t packed_elements_a = RS1;
reg_t packed_elements_b = RS2;
reg_t scales = RS1;

mx_scale_e8m0_t scale_a(scales & 0xFF);
mx_scale_e8m0_t scale_b((scales >> 8) & 0xFF);

// Unpack the 8 x FP8 elements
omxfp8_e4m3 a[8];
omxfp8_e4m3 b[8];
for (int i = 0; i < 8; i++) {
  a[i] = omxfp8_e4m3((packed_elements_a >> (i * 8)) & 0xFF);
  b[i] = omxfp8_e4m3((packed_elements_b >> (i * 8)) & 0xFF);
}

// vadd.vv vd, vs1, vs2, vm
VI_VV_LOOP
({
  vd = a[i] + b[i];
})