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

void print_err(int e, char *prg_name, char *err_string)
{
    char *s = strerror(e);
    write(2, ERR_PRES, 7);
    write(2, prg_name, strlen(prg_name));
    write(2, PRG_NAME_ENDING_ERR, 3);
    err_string ? write(2, err_string, strlen(err_string)) : write(2, s, strlen(s));
    write(2, LF, 1);
    exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
    int fd;
    int err;
    char *prg_name;
    struct stat buf;
    void *ptr;
    ELF_datas elf_data;

    memset(&elf_data, 0, sizeof(elf_data));
    if (ac == 1)
        prg_name = ASSEMBLY_OUTPUT;// a.out
    else
        prg_name = av[1];
    fd = open(prg_name,  O_RDONLY);
    if (fd == -1)
        print_err(errno, prg_name, NULL);
    if (fstat(fd, &buf) == -1)
        print_err(errno, prg_name, NULL);
    ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED)
        print_err(errno, prg_name, NULL);
    elf_data.hdr_64 = (Elf64_Ehdr *)ptr;
    if (((Elf64_Ehdr *)ptr)->e_ident[EI_CLASS] == ELFCLASSNONE)
    {
        print_err(0, prg_name, FILE_FORMAT_NOT_RECOGNIZED);
    }
    // si pas  0x7fELF, ou avec un format (normalement 32 ou 64) non reconnu -> file format not recognized.
    if (strncmp(ELFMAG, (const char *)((Elf64_Ehdr *)ptr)->e_ident, SELFMAG) != 0 \
        || ((Elf64_Ehdr *)ptr)->e_ident[EI_CLASS] == ELFCLASSNONE)
    {
        printf("This is an elf. w00t\n");
    }
    munmap(ptr, buf.st_size);/* on libere l'espace memoire alloue par le kernel */
    close(fd);
    return (EXIT_SUCCESS);
}