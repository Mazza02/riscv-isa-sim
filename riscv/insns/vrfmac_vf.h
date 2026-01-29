require_either_extension('F', EXT_ZR)
require_fp;
require_zvfbfa;

VI_VFP_VF_LOOP_ZR
({
  float16_t acc = i32_to_f16(READ_ZRVFREG(0));
  acc = VFP_MULADD_16(rs1, vs2, acc);
  WRITE_ZRVFREG(0, f16_to_i32(acc, softfloat_roundingMode, true));
},
{
  float32_t acc = i32_to_f32(READ_ZRVFREG(0));
  acc = f32_mulAdd(rs1, vs2, acc);
  WRITE_ZRVFREG(0, f32_to_i32(acc, softfloat_roundingMode, true));
},
{
  float64_t acc = i64_to_f64(READ_ZRVFREG(0));
  acc = f64_mulAdd(rs1, vs2, acc);
  WRITE_ZRVFREG(0, f64_to_i64(acc, softfloat_roundingMode, true));
})



set_fp_exceptions;
