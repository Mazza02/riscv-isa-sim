ZVLDOT_INIT(4);

switch (P.VU.vsew) {
  case 8: {
    require_extension(EXT_VMXDOTP);
    /* same vd handling as vq6kdotp42: vd = sum (int32, bytes 0-3) | sub-block counter 0-7 (byte 4) */
    VQ6KDOTP42_LOOP(uint8_t, int8_t, int32_t, vq5kdotp_dot);
    break;
  }
  default: require(false);
}

VECTOR_END;
