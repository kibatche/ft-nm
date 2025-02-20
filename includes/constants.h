#ifndef CONSTANTS_H
# define CONSTANTS_H

# define REEF(x){if (x){free(x);x = NULL;}}
# define ERROR -1
# define SUCCESS 0
# define L_ENDIAN 1
# define B_ENDIAN 2
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
# define BAD_INDEX_FOR_STRING_TABLE "bad index for e_shstrndx entry"
# define NO_SYMBOL "no symbol"
# define NO_SHSTRTAB "no string table"

//symbol's letters
# define ABSOLUTE 'A' // st_shndx == SHN_ABS
# define BSS_GLOB 'B' //.bss
# define BSS_LOC 'b' // .bss
# define COMMON_GLOB 'C' // STT_OBJECT STB_GLOBAL symbol whose st_shndx field holds SHN_COMMON
# define DATA_GLOB 'D' // .data 
# define DATA_LOC 'd' // .data
# define SDATA_GLOB 'G' // .sdata
# define SDATA_LOC 'g'// .sdata
# define INDIRECT_FUN 'i' // st_shndx == STT_GNU_IFUNC (10) && STB_GLOB
# define NODATA_NOCODE_NODEBUG_RO 'n' /* type == STT_SECTION && st_shndx != .data && st_shndx != .text && st_shndx != .debug
&& section->type != SHF_WRITE && section->type != SHF_EXECINSTR*/
# define DEBUG_SYM 'N'// st_shndx == .debug
# define STACK_UNWIND 'p' // st_shndx == .eh_frame / .eh_frame_hdr
# define READ_ONLY_GLOB 'R' // st_shndx == .rodata || .rodata1 || 
# define READ_ONLY_LOC 'r' // st_shndx == .rodata || .rodata1 || .note.ABI-tag (SHT_note)
# define SMALL_BSS_GLOB 'S' // .sbss
# define SMALL_BSS_LOC 's' // .sbss
# define TEXT_GLOB 'T' // .text
# define TEXT_LOC 't' // .text
# define UNDEFINED 'U' // section_type = SHT_NULL && bind  != STB_WEAK
# define UNIQUE 'u' // bind == STB_GNU_UNIQUE
# define WEAK_OBJ 'V' // bind == STB_WEAK && type == STT_OBJECT  aka objet ET weak
# define WEAK_OBJ_SHTNULL 'v' // STB_WEAK && STT_OBJECT && section_type = SHT_NULL
# define WEAK_NOTYPE_SHTNULL 'w' // STB_WEAK && STT_NOTYPE && section_type = SHT_NULL
# define WEAK_NOTYPE 'W' // STT_WEAK && STT_NOTYPE
# define UNKNOWN '?' // rien de tout au dessus
/* DONOTUSEIT_SCOMMON : IMPOSSIBLE de nos jours. gcc/clang transforment 
tous les symboles de .scommon en .bss / .data (cf. COMMON_GLOB). */
# define DONOTUSEIT_SCOMMON 'c' 
# define DONOTUSEIT_INDIRECT_SYM 'I' // Ce n'est pas à implémenter car c'est pour le format a.out
# define DONOTUSEIT_STAB '-' // .stab section Pas à implémenter car c'est pour le format a.out

#endif