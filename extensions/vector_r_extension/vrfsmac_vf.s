#void vrfsmac_vf(float rd)
.global vrfsmac_vf

vrfsmac_vf:

   .insn r 0x57, 0x0, 0x68, ft0, x0, x0
   
   fsw ft0, 0(a0)

    ret
    