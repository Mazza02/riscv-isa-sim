# void fmac_s(float *output, float input, float filter)
.global fmac_s

fmac_s:
    # Arguments:
    # a0 holds the pointer (*output)
    # fa0 holds the input (input)
    # fa1 holds the filter (filter)
    
    flw fa2, 0(a0)          # fa2 = *output (initial value)

    .insn r4 0x5B, 0x0, 0x0, fa2, fa0, fa1, fa2
                            # fa2 = (input * filter) + *output


    fsw fa2, 0(a0)          # *output = fa2 (final result)
    
    ret
