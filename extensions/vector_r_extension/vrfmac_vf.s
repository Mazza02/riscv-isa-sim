#void vrfmac_vf(vfloat32m1_t a1, float a2, int vl_count)
.global vrfmac_vf


vrfmac_vf:
    .insn r 0x57, 0x5, 0x4D, x8, f10, x8
    # add 1 bit to funt7 -- 0x26 = 100110 -- 1001101 = 0x4D
    #define MATCH_VRFMAC_VF 0x98005057 + 0x02000000 = 0x9a855057 also works
    ret
