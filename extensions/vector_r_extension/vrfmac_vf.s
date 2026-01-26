#void vrfmac_vf(vfloat32m1_t vs1, vfloat32m1_t vs2, int vl_count)
.global vrfmac_vf

vrfmac_vf:
   .insn r 0x57, 0x0, 0x64, x6, x7, x8
    ret
