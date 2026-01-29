#void vrfmac_vf(vfloat32m1_t a1, float a2, int vl_count)
.global vrfmac_vf


vrfmac_vf:
      vsetvli t0, a0, e32, m1, ta, ma
    .insn r 0x57, 0x0, 0x64, x8, f10, x8 
    ret
