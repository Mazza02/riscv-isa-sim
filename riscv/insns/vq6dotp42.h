// 4 low bits / 2 high bits = 42
ZVLDOT_INIT(4);

switch (P.VU.vsew) {
  case 8: {
    require_extension(EXT_VMXDOTP);
    VQ6DOTP42_LOOP(uint8_t, int8_t, int32_t, vq6dotp42_dot);
    break;
  }
  default: require(false);
}

VECTOR_END;
