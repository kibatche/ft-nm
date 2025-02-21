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

typedef struct Symbol
{
    char            *addr;
    char            letter;
    char            *name;
    struct Symbol   *next;
    struct Symbol   *prev; 
}   Symbol;

typedef struct Sym_list
{
    Symbol *first_sym;
    Symbol *last_sym;
}   Sym_list;

typedef struct ELF_datas_64
{
    Elf64_Ehdr  *hdr_64;/*elf header pour les elf 64 bits */
    Elf64_Shdr  *shdr_64;/*section header pour les elf 64 bits */
    Elf64_Shdr  *shsymtab_64;/*section relative aux symboles*/
    Elf64_Sym   *symtab_hdr_64;/*table des symboles*/
    uint64_t    nb_of_symbols_64;/*nombre de symboles*/
    void        *section_strtab_64;/*table des strings pour le nom des sections*/
    void        *string_table;
    int         proc_bits;/*64 ou 32 bits*/
    int         current_machine_endianess;
    uint16_t    type;/*LSB ou MSB*/
    uint16_t    header_size;/* taille du elf header*/
    uint16_t    shstrndx;/* index de la string table dans la table des sections*/
    uint64_t    offset_section_table_64;/*offset de la table des sections*/
    uint16_t    nb_of_entries_section_table_64;/*nombre d'entrées dans la table des section*/
    uint32_t    size_of_entry_section_table_64;/*taille de chaque entrée dans la table des sections*/
    Sym_list    *symbol_list;
} ELF_datas_64;

typedef struct ELF_datas_32
{
    Elf32_Ehdr  *hdr_32;/*elf header pour les elf 32 bits */
    Elf32_Shdr  *shdr_32;/*elf header pour les elf 32 bits */
    void        *section_strtab_32;
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
    Sym_list    *symbol_list;
} ELF_datas_32;

/*parsing elf généraliste*/
int     is_valid_elf_file(ELF_datas_64 *elf_datas_64, ELF_datas_32 *elf_datas_32);
int     check_current_machine_endianess();

/*fonctions 64 bits : Parsing basique*/
int     convert_and_fill_to_right_endianess_64(ELF_datas_64 *elf_datas, int file_endianess);
int     fill_64(ELF_datas_64 *elf_datas);
int     convert_and_fill_to_LSB_64(ELF_datas_64 *elf_datas);
int     convert_and_fill_to_MSB_64(ELF_datas_64 *elf_datas);

/*fonctions 64 bits : Parsing des symboles*/
char        *parse_symbol_address_64(Elf64_Sym *symbol_to_parse);
char        parse_symbol_letter_64(ELF_datas_64 *elf_datas, Elf64_Sym *symbol_to_parse);
int         parse_symbols_64(ELF_datas_64 *elf_datas);
Elf64_Shdr  *get_section_by_name_64(ELF_datas_64 *elf_datas, const char *name);
Elf64_Shdr  *get_section_by_idx_64(ELF_datas_64 *elf_datas, unsigned int idx);
char        *get_section_name_by_idx_64(ELF_datas_64 *elf_datas, unsigned int idx);

/*fonctions 32 bits*/
int     convert_and_fill_to_right_endianess_32(ELF_datas_32 *elf_datas, int file_endianess);
int     fill_32(ELF_datas_32 *elf_datas);
int     convert_and_fill_to_LSB_32(ELF_datas_32 *elf_datas);
int     convert_and_fill_to_MSB_32(ELF_datas_32 *elf_datas);

/*fonctions pour liste des symboles*/
int     new_symbol_pushback(Sym_list *symbol_lst);
void	ft_clear_lst(Sym_list *symbol_lst);

/*fonction d'erreur*/
int     print_err(int e, char *err_string);

#endif