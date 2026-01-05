require_either_extension('F', EXT_ZR);
require_fp;
softfloat_roundingMode = RM;
float32_t acc = f32(READ_ZRFREG(0));
if ((acc.v & 0x7FFFFFFF) > 0x7F800000) {
    acc = i32_to_f32(0);
}
WRITE_ZRFREG(0, f32_mulAdd(FRS1_F, FRS2_F, acc));

set_fp_exceptions;