ZVLDOT_INIT(4);

switch (P.VU.vsew) {
  case 8: {
    require_extension(EXT_VMXDOTP);
    VQ4DOTP_LOOP(uint8_t, int8_t, int32_t, vq4dotp_dot);
    break;
  }
  default: require(false);
}

VECTOR_END;
