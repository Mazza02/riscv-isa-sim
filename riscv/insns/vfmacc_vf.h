// vfmacc.vf vd, rs1, vs2, vm    # vd[i] = +(vs2[i] * x[rs1]) + vd[i]

require_zvfbfa

printf("rs1: %ld\n", RS1);
printf("rs2: %ld\n", RS2);
printf("rs3: %ld\n", RS3);

VI_VFP_VF_LOOP
({
  vd = VFP_MULADD_16(rs1, vs2, vd);
},
{
  printf("rs1: %ld\n", rs1);
  printf("vs2: %ld\n", vs2);
  printf("vd: %ld\n", vd);
  vd = f32_mulAdd(rs1, vs2, vd);
  printf("rs1 after: %ld\n", rs1);
  printf("vs2 after: %ld\n", vs2);
  printf("vd after: %ld\n", vd);
},
{
  vd = f64_mulAdd(rs1, vs2, vd);
})
