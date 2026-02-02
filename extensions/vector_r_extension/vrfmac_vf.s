#void vrfmac_vf(vfloat32m1_t a1, float a2, int vl_count)
.global vrfmac_vf


vrfmac_vf:
    .insn 0x4, 0x9a855057 # It's #define MATCH_VRFMAC_VF 0x98005057 + 0x02000000 = 0x9a855057
    ret
