#include "../includes/ft_nm.h"

extern char *prg_name;
extern struct stat buf;
extern int fd;
extern void *ptr;

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