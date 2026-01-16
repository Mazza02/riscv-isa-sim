require_either_extension('F', EXT_ZR);
require_zvfbfa

VI_VFP_VF_LOOP
({
  vd = VFP_MULADD_16(rs1, vs2, vd);
},
{
  vd = f32_mulAdd(rs1, vs2, vd);
},
{
  vd = f64_mulAdd(rs1, vs2, vd);
})
