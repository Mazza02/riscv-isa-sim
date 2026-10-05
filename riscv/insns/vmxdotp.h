require_either_extension('F', 'V');

reg_t scales = RS1;

mx_scale_e8m0_t scale_a(scales & 0xFF);
mx_scale_e8m0_t scale_b((scales >> 8) & 0xFF);

VI_VFP_BASE;
ZVBDOT_INIT(4);

#define COMMA ,

switch (P.VU.vsew) {
  case 8: {
    require_extension(EXT_VMXDOTP);
    if (P.VU.altfmt) {
      VMXDOTP_LOOP(uint8_t, uint8_t, float32_t,vmxdotp_dot_acc<omxfp8_e5m2 COMMA omxfp8_e5m2>, scale_a, scale_b);
    } else {
      VMXDOTP_LOOP(uint8_t, uint8_t, float32_t,vmxdotp_dot_acc<omxfp8_e4m3 COMMA omxfp8_e4m3>, scale_a, scale_b);
    }
    break;
  }
  default: require(false);
}

VECTOR_END;