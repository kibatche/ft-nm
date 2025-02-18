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

typedef struct Symbol_entry
{
    char            sym_rdx[17];
    char                sym_type;
    char                *sym_name;
    struct Symbol_entry       *next;
}   Symbol_entry;

typedef struct Symbol_list
{
    Symbol_entry *first;
}   Symbol_list;

typedef struct ELF_datas_64
{
    Elf64_Ehdr  *hdr_64;/*elf header pour les elf 64 bits */
    Elf64_Shdr  *shdr_64;/*section header pour les elf 64 bits */
    Elf64_Shdr  *shstrtab_64;/*section des string pour les noms*/
    Elf64_Shdr  *shsymtab_64;/*section relative aux symboles*/
    Elf64_Sym   *symtab_hdr_64;/*table des symboles*/
    uint64_t    nb_of_symbols_64;/*nombre de symboles*/
    void        *string_table;
    int         proc_bits;/*64 ou 32 bits*/
    int         current_machine_endianess;
    uint16_t    type;/*LSB ou MSB*/
    uint16_t    header_size;/* taille du elf header*/
    uint16_t    shstrndx;/* index de la string table dans la table des sections*/
    uint64_t    offset_section_table_64;/*offset de la table des sections*/
    uint16_t    nb_of_entries_section_table_64;/*nombre d'entrées dans la table des section*/
    uint32_t    size_of_entry_section_table_64;/*taille de chaque entrée dans la table des sections*/
    Symbol_list *symbol_list;
} ELF_datas_64;

typedef struct ELF_datas_32
{
    Elf32_Ehdr  *hdr_32;/*elf header pour les elf 32 bits */
    Elf32_Shdr  *shdr_32;/*elf header pour les elf 32 bits */
    Elf32_Shdr  *shstrtab_32;
    Elf32_Shdr  *shsymtab_32;
    Elf32_Sym   *symtab_hdr_32;
    uint32_t    nb_of_symbols_32;
    void        *string_table;
    int         proc_bits;/*64 ou 32 bits*/
    int         current_machine_endianess;
    uint16_t    type;/*LSB ou MSB*/
    uint16_t    header_size;/* taille du elf header*/
    uint16_t    shstrndx;/* index de la string table dans la table des sections*/
    uint32_t    offset_section_table_32;/*offset de la table des sections*/
    uint16_t    nb_of_entries_section_table_32;/*nombre d'entrées dans la table des section*/
    uint16_t    size_of_entry_section_table_32;/*taille de chaque entrée dans la table des sections*/
    struct Symbol_entry    *symbol_entry_hdr;
} ELF_datas_32;


int    is_valid_elf_file(ELF_datas_64 *elf_datas_64, ELF_datas_32 *elf_datas_32);
void    convert_and_fill_to_right_endianess_64(ELF_datas_64 *elf_datas, int file_endianess);
void    convert_and_fill_to_right_endianess_32(ELF_datas_32 *elf_datas, int file_endianess);
Symbol_entry *new_symbol();
void    print_err(int e, char *err_string);
int     check_current_machine_endianess();

#endif