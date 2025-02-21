#include "../includes/ft_nm.h"

char *prg_name;
struct stat buf;
void *ptr;

/* A FAIRE : REMPLACER TOUTES LES FONCTIONS DE LA GLIBC AVEC CELLES DE LA LIBFT */
/* IL FAUT FAIRE EN SORTE DE JUSTE RENVOYER UNE ERREUR SANS QUITTER LE PRG QUAND IL Y A PLUSIEURS FICHIERS A LIRE*/
int main(int ac, char **av)
{
    ELF_datas_64 elf_datas_64;
    ELF_datas_32 elf_datas_32;
    int nb_of_args = 0;
    int fd;
    int ret;
    int ret2;

    while (++nb_of_args < ac)
    {
        prg_name = (ac == 1 ? ASSEMBLY_OUTPUT:av[nb_of_args]);
        // if (ac > 1)
        // {
        //     write(1, av[nb_of_args], ft_strlen(av[nb_of_args]));
        //     write(1, ":\n", 2);
        // }
        ft_memset(&elf_datas_64, 0, sizeof(elf_datas_64));
        ft_memset(&elf_datas_32, 0, sizeof(elf_datas_32));
        elf_datas_64.current_machine_endianess = check_current_machine_endianess();/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
        elf_datas_32.current_machine_endianess = elf_datas_64.current_machine_endianess;/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
        fd = open(prg_name,  O_RDONLY);
        if (fd == -1)
            return print_err(errno, NULL);
        if (fstat(fd, &buf) == -1)
            return print_err(errno, NULL);
        ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED)
            return print_err(errno, NULL);
        ret = is_valid_elf_file(&elf_datas_64, &elf_datas_32);
        if (ret == ERROR)
            goto FREE_AND_CLOSE;
        else if (ret == 64)
            ret2 = parse_symbols_64(&elf_datas_64);
        else
            ret2 = printf("32\n");
        if (ret2 == ERROR)
            goto FREE_AND_CLOSE;
        else if (ret == 64)
            continue;//sort_symbol_64(&elf_datas_64);
        else
            printf("print sym 32\n"); 
    FREE_AND_CLOSE:
        munmap(ptr, buf.st_size);/* on libere l'espace memoire alloue par le kernel */
        ft_clear_lst(elf_datas_32.symbol_list);
        ft_clear_lst(elf_datas_64.symbol_list);
        close(fd);
    }
    exit(EXIT_SUCCESS);
}