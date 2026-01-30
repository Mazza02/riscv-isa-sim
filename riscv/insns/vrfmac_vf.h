require_either_extension('F', EXT_ZR)
require_fp;
require_zvfbfa;

VI_VFP_VF_LOOP_ZR
({
  WRITE_ZRVFREG(0, VFP_MULADD_16(rs1, vs2, acc));
},
{
  WRITE_ZRVFREG(0, f32_mulAdd(rs1, vs2, acc));
},
{
  WRITE_ZRVFREG(0, f64_mulAdd(rs1, vs2, acc));
})



set_fp_exceptions;
