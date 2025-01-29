#include <sys/mman.h> /* munmap, mmap */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <endian.h>
#include <sys/stat.h>
#include <string.h>
#include <elf.h>
#include "../libft/libft.h"
#include "../includes/ft_nm.h"
#include "../includes/constants.h"

char *prg_name;
struct stat buf;
int fd;
void *ptr;

void print_err(int e, char *err_string)
{
    char *s = strerror(e);
    write(2, ERR_PRES, 7);
    write(2, prg_name, strlen(prg_name));
    write(2, PRG_NAME_ENDING_ERR, 3);
    err_string ? write(2, err_string, strlen(err_string)) : write(2, s, strlen(s));
    write(2, LF, 1);
    munmap(ptr, buf.st_size);
    close(fd);
    exit(EXIT_FAILURE);
}

int check_current_machine_endianess()
{
    int i = 1;
    char *p = (char *)&i;

    if (p[0] == 1)
        return L_ENDIAN;
    else
        return B_ENDIAN;
}

void convert_and_fill_to_right_endianess_64(ELF_datas *elf_datas, int file_endianess)
{
    unsigned char *tmp = (unsigned char *)ptr;

    elf_datas->hdr_64 = (Elf64_Ehdr *)ptr;
    elf_datas->proc_bits = 64;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        if (file_endianess == ELFDATA2MSB)
        {
            elf_datas->offset_section_table_64 = be64toh(elf_datas->hdr_64->e_shoff);
            elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->hdr_64->e_shnum);
            elf_datas->size_of_entry_section_table_64 = be16toh(elf_datas->hdr_64->e_shentsize);
            elf_datas->type = be16toh(elf_datas->hdr_64->e_type);
            elf_datas->header_size = be16toh(elf_datas->hdr_64->e_ehsize);
            elf_datas->shdr_64 = (Elf64_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_64);
            if (elf_datas->nb_of_entries_section_table_64 == 0)
            {
                if (be16toh(elf_datas->shdr_64->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_64 = be16toh(elf_datas->shdr_64->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
                }
            }
        }
        else if (file_endianess == ELFDATA2LSB)
        {
            elf_datas->offset_section_table_64 = le64toh(elf_datas->hdr_64->e_shoff);
            elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->hdr_64->e_shnum);
            elf_datas->size_of_entry_section_table_64 = le16toh(elf_datas->hdr_64->e_shentsize);
            elf_datas->type = le16toh(elf_datas->hdr_64->e_type);
            elf_datas->header_size = le16toh(elf_datas->hdr_64->e_ehsize);
            elf_datas->shdr_64 = (Elf64_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_64);
            if (elf_datas->nb_of_entries_section_table_64 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
            {
                if (le16toh(elf_datas->shdr_64->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_64 = le16toh(elf_datas->shdr_64->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
                }
            }
        }
    }
    else
    {
        elf_datas->offset_section_table_64 = elf_datas->hdr_64->e_shoff;
        elf_datas->nb_of_entries_section_table_64 = elf_datas->hdr_64->e_shnum;
        elf_datas->size_of_entry_section_table_64 = elf_datas->hdr_64->e_shentsize;
        elf_datas->type = elf_datas->hdr_64->e_type;
        elf_datas->header_size = elf_datas->hdr_64->e_ehsize;
        elf_datas->shdr_64 = (Elf64_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_64);
        if (elf_datas->nb_of_entries_section_table_64 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
        {
            if (elf_datas->shdr_64->sh_size != 0)
            {
                elf_datas->nb_of_entries_section_table_64 = elf_datas->shdr_64->sh_size;
            }
            else
            {
                print_err(0, WRONG_NB_OF_ENTRIES);
            }
        }
    }
}

uint32_t convert_and_fill_to_right_endianess_32(ELF_datas *elf_datas, int file_endianess)
{
    unsigned char *tmp = (unsigned char *)ptr;

    elf_datas->hdr_32 = (Elf32_Ehdr *)ptr;
    elf_datas->proc_bits = 32;
    if (elf_datas->current_machine_endianess != file_endianess)
    {
        if (file_endianess == ELFDATA2MSB)
        {
            elf_datas->offset_section_table_32 = be32toh(elf_datas->hdr_32->e_shoff);
            elf_datas->nb_of_entries_section_table_32 = be16toh(elf_datas->hdr_32->e_shnum);
            elf_datas->size_of_entry_section_table_32 = be16toh(elf_datas->hdr_32->e_shentsize);
            elf_datas->type = be16toh(elf_datas->hdr_32->e_type);
            elf_datas->header_size = be16toh(elf_datas->hdr_32->e_ehsize);
            elf_datas->shdr_32 = (Elf32_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_32);
            if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
            {
                if (be16toh(elf_datas->shdr_32->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_32 = be16toh(elf_datas->shdr_32->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
                }
            }
        }
        else if (file_endianess == ELFDATA2LSB)
        {
            elf_datas->offset_section_table_32 = le32toh(elf_datas->hdr_32->e_shoff);
            elf_datas->nb_of_entries_section_table_32 = le16toh(elf_datas->hdr_32->e_shnum);
            elf_datas->size_of_entry_section_table_32 = le16toh(elf_datas->hdr_32->e_shentsize);
            elf_datas->type = le16toh(elf_datas->hdr_32->e_type);
            elf_datas->header_size = le16toh(elf_datas->hdr_32->e_ehsize);
            elf_datas->shdr_32 = (Elf32_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_32);
            if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
            {
                if (le16toh(elf_datas->shdr_32->sh_size) != 0)
                {
                    elf_datas->nb_of_entries_section_table_32 = le16toh(elf_datas->shdr_32->sh_size);
                }
                else
                {
                    print_err(0, WRONG_NB_OF_ENTRIES);
                }
            }
        }
    }
    else
    {
        elf_datas->offset_section_table_32 = elf_datas->hdr_32->e_shoff;
        elf_datas->nb_of_entries_section_table_32 = elf_datas->hdr_32->e_shnum;
        elf_datas->size_of_entry_section_table_32 = elf_datas->hdr_32->e_shentsize;
        elf_datas->type = elf_datas->type;
        elf_datas->header_size = elf_datas->hdr_32->e_ehsize;
        elf_datas->shdr_32 = (Elf32_Shdr *)((unsigned char *)tmp + elf_datas->offset_section_table_32);
        if (elf_datas->nb_of_entries_section_table_32 == 0)// check si nb entry > SHN_LORESERVE (0xff00)
        {
            if (elf_datas->shdr_32->sh_size != 0)
            {
                elf_datas->nb_of_entries_section_table_32 = elf_datas->shdr_32->sh_size;
            }
            else
            {
                print_err(0, WRONG_NB_OF_ENTRIES);
            }
        }
    }
}

void isValidElfFile(void *ptr, char *prg_name, ELF_datas *elf_datas)
{
    unsigned char *tmp = (unsigned char *)ptr;// on cast en unsigned tant qu'on ne sait si 32 ou 64 bits

    if (strncmp(ELFMAG, (const char *)tmp, SELFMAG) != 0)// est-ce que les 4 premiers bytes == \177ELF ?
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] != ELFCLASS64 && tmp[EI_CLASS] != ELFCLASS32)
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);// ni 64 ni 32 bits
    else if (tmp[EI_DATA] != ELFDATA2LSB \
        && tmp[EI_DATA] != ELFDATA2MSB)// ni LSB ni MSB
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] == ELFCLASS64)// cast dans la bonne struct
        convert_and_fill_to_right_endianess_64(elf_datas, tmp[EI_DATA]);
    else
        convert_and_fill_to_right_endianess_32(elf_datas, tmp[EI_DATA]);
    switch (elf_datas->proc_bits)
    {
        case 64:
            if (elf_datas->header_size != 0x40)
                print_err(0, BAD_HEADER_SIZE);
            else if (elf_datas->type == ET_CORE)// nb : on ne gère pas le type core
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            else if ((elf_datas->offset_section_table_64 + (elf_datas->size_of_entry_section_table_64 * elf_datas->nb_of_entries_section_table_64)) != buf.st_size)
                print_err(0, SIZE_OF_THE_FILE_MISMATCH);// offset de la section hdr sect tab + taille de la section hdr tab n'est pas égal à la taille du fichier. 
            break;
        case 32 :
            if (elf_datas->header_size != 0x34)
                print_err(0, BAD_HEADER_SIZE);
            if (elf_datas->type == ET_CORE)
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            else if ((elf_datas->offset_section_table_32 + (elf_datas->size_of_entry_section_table_32 * elf_datas->nb_of_entries_section_table_32)) != buf.st_size)
                print_err(0, SIZE_OF_THE_FILE_MISMATCH);
            break;
        default:
            print_err(0, UNKNOWN_ERR);
    }
}
/* A FAIRE : 
e_shstrndx
              This member holds the section header table index of the
              entry associated with the section name string table.  If
              the file has no section name string table, this member
              holds the value SHN_UNDEF.

              If the index of section name string table section is
              larger than or equal to SHN_LORESERVE (0xff00), this
              member holds SHN_XINDEX (0xffff) and the real index of the
              section name string table section is held in the sh_link
              member of the initial entry in section header table.
              Otherwise, the sh_link member of the initial entry in
              section header table contains the value zero.
*/
int main(int ac, char **av)
{
    int err;
    ELF_datas elf_datas;

    memset(&elf_datas, 0, sizeof(elf_datas));
    if (ac == 1)
        prg_name = ASSEMBLY_OUTPUT;// a.out
    else
        prg_name = av[1];
    elf_datas.current_machine_endianess = check_current_machine_endianess();/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
    fd = open(prg_name,  O_RDONLY);
    if (fd == -1)
        print_err(errno, NULL);
    if (fstat(fd, &buf) == -1)
        print_err(errno, NULL);
    ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED)
        print_err(errno, NULL);
    isValidElfFile(ptr, prg_name, &elf_datas);
    munmap(ptr, buf.st_size);/* on libere l'espace memoire alloue par le kernel */
    close(fd);
    exit(EXIT_SUCCESS);
}