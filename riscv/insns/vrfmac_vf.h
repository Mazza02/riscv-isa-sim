#include "v_ext_macros.h"
require_either_extension('F', EXT_ZR)
require_fp;
require_zvfbfa;
//printf("vrfmac.vf P.VU.vsew: %d\n", P.VU.vsew);
printf("rs1: %ld\n", RS1);
printf("rs2: %ld\n", RS2);
printf("rs3: %ld\n", RS3);
printf("\n");

VI_VFP_VF_LOOP_ZR
({
  float16_t acc = i32_to_f16(READ_ZRVFREG(0));
  acc = VFP_MULADD_16(rs1, vs2, acc);
  WRITE_ZRVFREG(0, f16_to_i32(acc, softfloat_roundingMode, true));
},
{
  float32_t acc = i32_to_f32(READ_ZRVFREG(0));
  //printf("gets here pt2! 2 \n");
  printf("rs1: %ld\n", rs1.v);
  printf("vs2: %ld\n", vs2.v);
  printf("acc: %ld\n", acc.v);
  acc = f32_mulAdd(rs1, vs2, acc);
  printf("rs1 after: %ld\n", rs1.v);
  printf("vs2 after: %ld\n", vs2.v);
  printf("acc after: %ld\n", acc.v);
  WRITE_ZRVFREG(0, f32_to_i32(acc, softfloat_roundingMode, true));
},
{
  float64_t acc = i64_to_f64(READ_ZRVFREG(0));
  acc = f64_mulAdd(rs1, vs2, acc);
  
  WRITE_ZRVFREG(0, f64_to_i64(acc, softfloat_roundingMode, true));
})



set_fp_exceptions;
