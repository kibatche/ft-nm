#ifndef FT_NM_H
# define FT_NM_H
# include "constants.h"
# include "../libft/libft.h"
# include <elf.h>
# include <endian.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/mman.h> /* munmap, mmap */

typedef struct ELF_datas
{
    Elf64_Ehdr  *hdr_64;/*elf header pour les elf 64 bits */
    Elf64_Shdr  *shdr_64;/*section header pour les elf 64 bits */
    Elf64_Shdr  *shstrtab_64;
    Elf64_Shdr   *shsymtab_64;
    Elf32_Ehdr  *hdr_32;/*elf header pour les elf 32 bits */
    Elf32_Shdr  *shdr_32;/*elf header pour les elf 32 bits */
    Elf32_Shdr  *shstrtab_32;
    Elf32_Shdr   *shsymtab_32;
    void        *string_table;
    int         proc_bits;/*64 ou 32 bits*/
    int         current_machine_endianess;
    uint16_t    type;/*LSB ou MSB*/
    uint16_t    header_size;/* taille du elf header*/
    uint16_t    shstrndx;/* index de la string table dans la table des sections*/
    uint32_t    offset_section_table_32;/*offset de la table des sections*/
    uint16_t    nb_of_entries_section_table_32;/*nombre d'entrée dans la table des section*/
    uint16_t    size_of_entry_section_table_32;/*taille de chaque entrée dans la table des sections*/
    uint64_t    offset_section_table_64;
    uint16_t    nb_of_entries_section_table_64;
    uint32_t    size_of_entry_section_table_64;
} ELF_datas;

void    is_valid_elf_file(ELF_datas *elf_datas);
void    convert_and_fill_to_right_endianess_32(ELF_datas *elf_datas, int file_endianess);
void    convert_and_fill_to_right_endianess_64(ELF_datas *elf_datas, int file_endianess);
void    print_err(int e, char *err_string);
int     check_current_machine_endianess();

#endif