#ifndef CONSTANTS_H
# define CONSTANTS_H

# define REEF(x){if (x){free(x);x = NULL;}}
#define L_ENDIAN 1
#define B_ENDIAN 2
# define ASSEMBLY_OUTPUT "a.out"
# define LF "\n"
# define ERR_PRES "ft_nm: "
# define SIMP_QUOTE "'"
# define PRG_NAME_ENDING_ERR ": "
# define UNKNOWN_ERR "unknown error"
# define FILE_FORMAT_NOT_RECOGNIZED "file format not recognized"
# define BAD_HEADER_SIZE "bad header size"
# define SIZE_OF_THE_FILE_MISMATCH "size of the file mismatch"
# define WRONG_NB_OF_ENTRIES "wrong number of entries inside section table"
#endif