#include "../includes/ft_nm.h"

char *prg_name;
struct stat buf;
int fd;
void *ptr;

/* A FAIRE : REMPLACER TOUTES LES FONCTIONS DE LA GLIBC AVEC CELLES DE LA LIBFT */
int main(int ac, char **av)
{
    ELF_datas elf_datas;
    int nb_of_args = 0;
    while (++nb_of_args < ac)
    {
        prg_name = (ac == 1 ? ASSEMBLY_OUTPUT:av[nb_of_args]);
        memset(&elf_datas, 0, sizeof(elf_datas));
        elf_datas.current_machine_endianess = check_current_machine_endianess();/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
        fd = open(prg_name,  O_RDONLY);
        if (fd == -1)
            print_err(errno, NULL);
        if (fstat(fd, &buf) == -1)
            print_err(errno, NULL);
        ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED)
            print_err(errno, NULL);
        is_valid_elf_file(&elf_datas);
        // fill_string_and_sym_struct(&elf_datas);
        munmap(ptr, buf.st_size);/* on libere l'espace memoire alloue par le kernel */
        close(fd);
    }
    exit(EXIT_SUCCESS);
}