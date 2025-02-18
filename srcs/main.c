#include "../includes/ft_nm.h"

char *prg_name;
struct stat buf;
int fd;
void *ptr;

/* A FAIRE : REMPLACER TOUTES LES FONCTIONS DE LA GLIBC AVEC CELLES DE LA LIBFT */
/* IL FAUT FAIRE EN SORTE DE JUSTE RENVOYER UNE ERREUR SANS QUITTER LE PRG QUAND IL Y A PLUSIEURS FICHIERS A LIRE*/
int main(int ac, char **av)
{
    ELF_datas_64 elf_datas_64;
    ELF_datas_32 elf_datas_32;
    Symbol_list symbol_list;
    int nb_of_args = 0;
    while (++nb_of_args < ac)
    {
        prg_name = (ac == 1 ? ASSEMBLY_OUTPUT:av[nb_of_args]);
        memset(&elf_datas_64, 0, sizeof(elf_datas_64));
        memset(&elf_datas_32, 0, sizeof(elf_datas_32));
        elf_datas_64.current_machine_endianess = check_current_machine_endianess();/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
        elf_datas_32.current_machine_endianess = elf_datas_64.current_machine_endianess;/* important pour calculs et checks. https://developer.ibm.com/articles/au-endianc/ */
        fd = open(prg_name,  O_RDONLY);
        if (fd == -1)
            print_err(errno, NULL);
        if (fstat(fd, &buf) == -1)
            print_err(errno, NULL);
        ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (ptr == MAP_FAILED)
            print_err(errno, NULL);
        int proc_bits = is_valid_elf_file(&elf_datas_64, &elf_datas_32);
        if (proc_bits == 64)
            printf("64\n");
        else
            printf("32\n");
        // FREE_AND_CLOSE:
        munmap(ptr, buf.st_size);/* on libere l'espace memoire alloue par le kernel */
        close(fd);
    }
    exit(EXIT_SUCCESS);
}