require_either_extension('F', EXT_ZR)
require_fp;
require_zvfbfa;

VI_VFP_VF_LOOP_ZR
({
  float16_t acc = f16(READ_ZRVFREG(0));
  if ((acc.v & 0x7FFFFFFF) > 0x7F800000) {
    acc = i32_to_f16(0);
}
  acc = VFP_MULADD_16(rs1, vs2, acc);
  WRITE_ZRVFREG(0, acc);
},
{
  float32_t acc = f32(READ_ZRVFREG(0));
  if ((acc.v & 0x7FFFFFFF) > 0x7F800000) {
    acc = i32_to_f32(0);
}
  acc = f32_mulAdd(rs1, vs2, acc);
  WRITE_ZRVFREG(0, acc);
},
{
  float64_t acc = f64(READ_ZRVFREG(0));
  if ((acc.v & 0x7FFFFFFF) > 0x7F800000) {
    acc = i32_to_f64(0);
}
  acc = f64_mulAdd(rs1, vs2, acc);
  WRITE_ZRVFREG(0, acc);
})



set_fp_exceptions;
