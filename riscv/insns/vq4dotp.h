require_either_extension('F', 'V');

reg_t scales = RS3;

float16_t scale_a = { uint16_t(scales & 0xFFFF) };
float16_t scale_b = { uint16_t((scales >> 16) & 0xFFFF) };

VI_VFP_BASE;
ZVLDOT_INIT(4);

switch (P.VU.vsew) {
  case 8: {
    require_extension(EXT_VMXDOTP);
    VQ4DOTP_LOOP(uint8_t, int8_t, float32_t, vq4dotp_dot_acc, scale_a, scale_b);
    break;
  }
  default: require(false);
}

VECTOR_END;
