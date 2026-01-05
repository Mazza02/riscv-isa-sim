    # void vaddi_vv(vint32m1_t *vs1, vint32m1_t *vs2, int vl_count)

    # a0 = ptr vs1
    # a1 = ptr vs2
    # a2 = vl_count
    # a3 = scalar_m

.globl vaddi_vv
vaddi_vv:

    # .word (0x0c000057 | (8 << 7) | (9 << 15) | (8 << 20) | (1 << 25))
    .insn r 0x57, 0x0, 0x07, x8, x9, x8
    ret
