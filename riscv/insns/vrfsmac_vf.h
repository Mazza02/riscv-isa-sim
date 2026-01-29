require_either_extension('F', EXT_ZR);
require_fp;
WRITE_FRD_ZRVF_S(READ_ZRVFREG(0));
set_fp_exceptions;