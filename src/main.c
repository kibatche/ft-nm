#include <sys/mman.h> /* munmap, mmap */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
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

void isValidElfFile(void *ptr, char *prg_name, ELF_datas *elf_datas)
{
    unsigned char *tmp = ((unsigned char *)ptr);// on cast en unsigned tant qu'on ne sait si 32 ou 64 bits
    if (strncmp(ELFMAG, (const char *)tmp, SELFMAG) != 0)
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] != ELFCLASS64 && tmp[EI_CLASS] != ELFCLASS32)
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);// ni 64 ni 32 bits
    else if (tmp[EI_DATA] != ELFDATA2LSB \
        && tmp[EI_DATA] != ELFDATA2MSB)// ni LSB ni MSB
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    else if (tmp[EI_VERSION] != EV_CURRENT)// check de la version
        print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
    if (tmp[EI_CLASS] == ELFCLASS64)// cast dans la bonne struct
    {
        elf_datas->hdr_64 = (Elf64_Ehdr *)ptr;
        elf_datas->proc_bits = 64;
    }
    else
    {        
        elf_datas->hdr_32 = (Elf32_Ehdr *)ptr;
        elf_datas->proc_bits = 32;
    }
    tmp = NULL;
    switch(elf_datas->proc_bits)
    {
        case 64:
            if (elf_datas->hdr_64->e_type != ET_REL \
                && elf_datas->hdr_64->e_type != ET_EXEC \
                && elf_datas->hdr_64->e_type != ET_DYN \
                && elf_datas->hdr_64->e_type != ET_CORE)
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            if (elf_datas->hdr_64->e_ehsize != 0x40)
                print_err(0, BAD_HEADER_SIZE);
            break;
        case 32 :
            if (elf_datas->hdr_32->e_type != ET_REL \
                && elf_datas->hdr_32->e_type != ET_EXEC \
                && elf_datas->hdr_32->e_type != ET_DYN \
                && elf_datas->hdr_32->e_type != ET_CORE)
                print_err(0, FILE_FORMAT_NOT_RECOGNIZED);
            if (elf_datas->hdr_32->e_ehsize != 0x34)
                print_err(0, BAD_HEADER_SIZE);
            break;
        default:
            print_err(0, UNKNOWN_ERR);
    }
}

int main(int ac, char **av)
{
    int err;
    ELF_datas elf_datas;

    memset(&elf_datas, 0, sizeof(elf_datas));
    if (ac == 1)
        prg_name = ASSEMBLY_OUTPUT;// a.out
    else
        prg_name = av[1];
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