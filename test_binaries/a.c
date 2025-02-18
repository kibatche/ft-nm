void *yolo() {write(1, "test",4);}
const int RODATA_GLOB_CANARY; // R .rodata
static int __attribute__((common)) COMON_VAR_LOC_CANARY;