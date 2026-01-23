require_either_extension('F', EXT_ZR);
require_fp;
printf("vrfmac_vf: works!\n");
// VI_VFP_VF_LOOP_ZR
// ({
//   vd = VFP_MULADD_16(rs1, vs2, vd);
// },
// {
//   vd = f32_mulAdd(rs1, vs2, vd);
// },
// {
//   vd = f64_mulAdd(rs1, vs2, vd);
// })
