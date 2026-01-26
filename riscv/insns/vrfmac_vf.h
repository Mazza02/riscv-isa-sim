require_either_extension('F', EXT_ZR);
require_fp;
require_zvfbfa;
printf("vrfmac.vf ");

VI_VFP_VF_LOOP_ZR
({
  printf("vrfmac.vf ");
  acc = VFP_MULADD_16(rs1, vs2, acc);
},
{
  printf("vrfmac.vf ");
  acc = f32_mulAdd(rs1, vs2, acc);
},
{
  printf("vrfmac.vf ");
  acc = f64_mulAdd(rs1, vs2, acc);
})
