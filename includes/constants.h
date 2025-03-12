#ifndef CONSTANTS_H
# define CONSTANTS_H

# define REEF(x){if (x){free(x);x = NULL;}}
# define ERROR 1
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
# define ABSOLUTE 'A' // OK st_shndx == SHN_ABS
# define BSS_GLOB 'B' // OK .bss
# define BSS_LOC 'b' // OK .bss
# define COMMON_GLOB 'C' // OK STT_OBJECT STB_GLOBAL symbol whose st_shndx field holds SHN_COMMON
# define DATA_GLOB 'D' // OK .data
# define DATA_LOC 'd' // OK .data
# define SDATA_GLOB 'G' // OK .sdata
# define SDATA_LOC 'g'// OK .sdata
# define INDIRECT_FUN 'i' // OK st_shndx == STT_GNU_IFUNC (10) && STB_GLOB
# define READ_ONLY_GLOB 'R' //OK st_shndx == .rodata || .rodata1 ||
# define READ_ONLY_LOC 'r' // OK st_shndx == .rodata || .rodata1 || .note.ABI-tag (SHT_note)
# define SMALL_BSS_GLOB 'S' // OK .sbss
# define SMALL_BSS_LOC 's' // OK .sbss
# define TEXT_GLOB 'T' // OK .text
# define TEXT_LOC 't' // OK .text
# define UNDEFINED 'U' // OK section_type = SHT_NULL && bind  != STB_WEAK
# define UNIQUE 'u' // OK bind == STB_GNU_UNIQUE
# define WEAK_OBJ 'V' // OK bind == STB_WEAK && type == STT_OBJECT  aka objet ET weak
# define WEAK_OBJ_SHTNULL 'v' // OK STB_WEAK && STT_OBJECT && section_type = SHT_NULL
# define WEAK_NOTYPE_SHTNULL 'w' // OK STB_WEAK && STT_NOTYPE && section_type = SHT_NULL
# define WEAK_NOTYPE 'W' // OK STT_WEAK && STT_NOTYPE
# define UNKNOWN '?' // OK rien de tout au dessus
/* DONOTUSEIT_SCOMMON : IMPOSSIBLE de nos jours. gcc/clang transforment
tous les symboles de .scommon en .bss / .data (cf. COMMON_GLOB). */
# define DONOTUSEIT_DEBUG_SYM 'N'// st_shndx == .debug
/*les symboles de type p (.eh_frame, .eh_framehdr)
ne sont pas pris en compte par nm sans option*/
# define DONOTUSEIT_STACK_UNWIND 'p' // st_shndx == .eh_frame / .eh_frame_hdr
# define DONOTUSEIT_NODATA_NOCODE_NODEBUG_RO 'n' /* type == STT_SECTION && st_shndx != .data && st_shndx != .text && st_shndx != .debug
&& section->type != SHF_WRITE && section->type != SHF_EXECINSTR*/
# define DONOTUSEIT_SCOMMON 'c'
# define DONOTUSEIT_INDIRECT_SYM 'I' // Ce n'est pas à implémenter car c'est pour le format a.out
# define DONOTUSEIT_STAB '-' // .stab section Pas à implémenter car c'est pour le format a.out

#endif
