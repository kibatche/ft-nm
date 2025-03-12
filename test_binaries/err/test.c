#include "ft_nm.h"

void	fill_space(char *s, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		((char*)s)[i] = ' ';
		i++;
	}
}

void	fill_type_name(char *str, char type, char *name, int arch)
{
	int i;
    int j;
    int max;

	i = 19;
    if (arch == 0)
        i = 11;
    j = 0;
    max = i + ft_strlen(name);
    str[i - 2] = type;
	while (i < max)
	{
		str[i] = name[j];
		i++;
        j++;
	}
}

void print_elf_header(Elf64_Ehdr *elf_header)
{
    printf("Elf header :\nELFMAG0 = %c (should be delete character)\n", elf_header->e_ident[EI_MAG0]);
    printf("ELFMAG1 = %c (should be 'E' character)\n", elf_header->e_ident[EI_MAG1]);
    printf("ELFMAG2 = %c (should be 'L' character)\n", elf_header->e_ident[EI_MAG2]);
    printf("ELFMAG3 = %c (should be 'F' character)\n", elf_header->e_ident[EI_MAG3]);
    printf("EI_CLASS = %d\n", elf_header->e_ident[EI_CLASS]);
    printf("EI_DATA = %d\n", elf_header->e_ident[EI_DATA]);
    printf("EI_VERSION = %d\n", elf_header->e_ident[EI_VERSION]);
    printf("e_type = %d\n", elf_header->e_type);
    printf("e_version = %d\n", elf_header->e_version);
    printf("e_shoff = %d\n", elf_header->e_shoff);
    printf("e_ehsize = %d\n", elf_header->e_ehsize);
    printf("e_shnum = %d\n", elf_header->e_shnum);
    printf("e_shstrndx = %d\n", elf_header->e_shstrndx);
}

void print_elf_section(Elf64_Shdr elf_section)
{
    printf("This is the section header :\n");
    printf("sh_name = %s\n", &elf_section.sh_name);
    printf("sh_type = %d\n", elf_section.sh_type);
    printf("sh_addr = %p\n", elf_section.sh_addr);
    printf("sh_offset = %d\n", elf_section.sh_offset);
    printf("sh_size = %d\n", elf_section.sh_size);
    printf("sh_link = %p\n", elf_section.sh_link);
    printf("SHT_SYMTAB = %d and SHT_DYNSYM = %d\n", SHT_SYMTAB, SHT_DYNSYM);
}

char            get_type_x64(Elf64_Sym symtab, Elf64_Shdr *elf_section)
{
    char  c;


    if (ELF64_ST_BIND(symtab.st_info) == STB_GNU_UNIQUE)
        c = 'u';
    else if (ELF64_ST_BIND(symtab.st_info) == STB_WEAK && ELF64_ST_TYPE(symtab.st_info) == STT_OBJECT)
    {
        c = 'V';
        if (symtab.st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (ELF64_ST_BIND(symtab.st_info) == STB_WEAK)
    {
        c = 'W';
        if (symtab.st_shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (symtab.st_shndx == SHN_UNDEF)
        c = 'U';
    else if (symtab.st_shndx == SHN_ABS)
        c = 'A';
    else if (symtab.st_shndx == SHN_COMMON)
        c = 'C';
    else if (elf_section[symtab.st_shndx].sh_type == SHT_NOBITS
    && elf_section[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'B';
    else if ((elf_section[symtab.st_shndx].sh_type == SHT_PROGBITS || elf_section[symtab.st_shndx].sh_type == SHT_NOTE)
    && elf_section[symtab.st_shndx].sh_flags == SHF_ALLOC)
        c = 'R';
    else if ((elf_section[symtab.st_shndx].sh_type == SHT_PROGBITS || elf_section[symtab.st_shndx].sh_type == SHT_INIT_ARRAY || elf_section[symtab.st_shndx].sh_type == SHT_FINI_ARRAY)
    && elf_section[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'D';
    else if (elf_section[symtab.st_shndx].sh_type == SHT_PROGBITS
    && elf_section[symtab.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
        c = 'T';
    else if (elf_section[symtab.st_shndx].sh_type == SHT_DYNAMIC)
        c = 'D';
    else
    {
        //printf("ST_SHNDX = %d, SH_TYPE = %d, SH_FLAGS = %d\n", symtab.st_shndx, elf_section[symtab.st_shndx].sh_type, elf_section[symtab.st_shndx].sh_flags);
        c = '?';
    }
    if (ELF64_ST_BIND(symtab.st_info) == STB_LOCAL && c != '?')
        c += 32;
    return (c);
}

int hex_to_int(char *hex)
{
    uint32_t val;
    int i;
    uint8_t byte;

    i = 0;
    val = 0;
    while(hex[i] == '0')
        i++;
    while (hex[i] && hex[i] != ' ')
    {
        // get current character then increment
        byte = hex[i]; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <='f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F')
            byte = byte - 'A' + 10;    
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = (val << 4) | (byte & 0xF);
        i++;
    }
    return val;
}

void int_to_hex(long unsigned int n, char *value, int arch)
{
    if (arch == 1)
        arch = 16;
    else
        arch = 8;
    for (int i = 0; i < arch; i++) {
        if (n > 0)
        {
            value[arch - i - 1] = "0123456789abcdef"[n & 0xF];
            n = n >> 4;
        }
        else
        {
            value[arch - i - 1] = 48;
        }
    }
}

int compare_string(char *s1, char *s2, int arch)
{
    int i;
    int j;
    int k;
    int l;
    int ret;
    unsigned char a;
    unsigned char b;

    i = 19;
    j = 19;
    if (arch == 0)
    {
        i = 11;
        j = 11;
    }
    k = 0;
    l = 0;

    //printf("s1 = %s, and s2 = %s\n", s1, s2);
    if (s1[i] == '\0' || (s1[i] != '$' && s2[i] == '$'))
        return (1);
    if (s2[j] == '\0'|| (s1[i] == '$' && s2[i] != '$'))
        return (-1);
    while(s1[i + k] && s2[j + l])
    {
        if (ft_isalnum(s1[i + k]) == 1 && ft_isalnum(s2[j + l]) == 1)
        {
            a = s1[i + k];
            b = s2[j + l];
            /*if (ft_isalnum(a) == 0 && ft_isalnum(b) == 0 && a != b)
                return (b - a);
            if (ft_isalnum(a) == 0 && ft_isalnum(b))
                return (1);
            if (ft_isalnum(b) == 0 && ft_isalnum(a))
                return (-1);*/
            ret = ft_toupper(a) - ft_toupper(b);
            if(ret != 0)
            {
               // printf("s1 = %s, s2 = %s et ret = %d\n", s1, s2, ret);
                return(ret);
            }
        }
    
        if (ft_isalnum(s1[i + k]) == 0)
            k++;
        else if (ft_isalnum(s2[j + l]) == 0)
            l++;
        else
        {
            i++;
            j++;
        }
    }
    //printf("ret = %d\n", s1[i + j] - s2[i + k]);
    //if (i == j)
    //   return(s1[i] - s2[j]);
    //k -= ft_strlen(s1);
    //l -= ft_strlen(s2);
    i = 17;
    if (arch == 0)
        i = 9;
    if (ft_strlen(s1) - k != ft_strlen(s2) - l)
        return ((ft_strlen(s1) - k) - (ft_strlen(s2) - l));
    if (ft_toupper(s1[i]) != ft_toupper(s2[i]))
        return (ft_toupper(s1[i]) - ft_toupper(s2[i]));
    if (k != l)
        return (l - k);
    k = 19;
    while (s1[k])
    {
        if (s1[k] == s2[k] - 32)
            return (1);
        if (s2[k] == s1[k] - 32)
            return (-1);
        k++;
    }
    k = hex_to_int(s1);
    l = hex_to_int(s2);
    if (k != l)
        return (k - l);
    //printf("l = %d k = %d s1 = %s, s2 = %s\n", l, k, s1, s2);
    return (ft_toupper(s2[i]) - ft_toupper(s1[i]));
    //return (s1[i + k] && s2[j + l]);
    //printf("wow s1 = |%s| and s2 = |%s|\n", s1, s2);
}

void sort_array(int data[], int size, char **value, int arch)
{
    int temp;
    int i;
    int j;

    i = 0;
    while (i < size)
    {
        data[i] = i;
        i++;
    }
    for (int interval = size / 2; interval > 0; interval /= 2)
    {
        //printf("test1\n");
        for (i = interval; i < size; i += 1)
        {
           // printf("test2\n");
            temp = data[i];
            for (j = i; j >= interval && compare_string(value[data[j - interval]], value[temp], arch) > 0; j -= interval)
            {
                data[j] = data[j - interval];
            }
            data[j] = temp;
        }
    }
}

int elf_x64(char *elf_file)
{
    Elf64_Ehdr elf_header;
    Elf64_Shdr *elf_section;
    Elf64_Sym  *symtab;
    Elf64_Shdr *strtab;
    char *section_name;
    char *name_symbol;
    size_t size;
    int tmp;
    int test;
    int i;

    i = 0;
    size = elf_file[0x34];
    ft_memcpy(&elf_header, elf_file, size);
    //print_elf_header(&elf_header);
    elf_section = (Elf64_Shdr*)(elf_file + elf_header.e_shoff);
    section_name = (char *) (elf_file + elf_section[elf_header.e_shstrndx].sh_offset);  //e_shstrndx is the index for the name table.
    while (i < elf_header.e_shnum)                                           //And offset is the number of byte since the begining of the elf file. So str = the begining of the name table
    {
        //write(1, "2", 1);

       /* //print_elf_section(elf_section[i]);
        if (elf_section[i].sh_type == SHT_SYMTAB)
        {
            //symtable_x64(elf_section[i]);
            print_elf_section(elf_section[i]);
            string_symbol_tables(elf_section[i]);
        }
        if (elf_section[i].sh_type == SHT_DYNSYM)
        {
            printf("lol");
            //symtable_x64(elf_section[i]);
        }*/
        
        if (elf_section[i].sh_size)
        {
            
            //write(1,"5",1);
            //printf("%d :%s\n", i,&section_name[elf_section[i].sh_name]);
            if (ft_strcmp(&section_name[elf_section[i].sh_name], ".symtab") == 0)
            {
               // write(1, "3", 1);
                symtab = (Elf64_Sym *) (elf_file + elf_section[i].sh_offset);
                tmp = elf_section[i].sh_size;
            }
            if (ft_strcmp(&section_name[elf_section[i].sh_name], ".strtab") == 0)
            {
              //  write(1, "4", 1);
                strtab = (Elf64_Shdr *) &elf_section[i];
            }
        }
        i++;
    }
    char type;
    char **value;
    int size2 = (tmp / sizeof(Elf64_Sym));
    int length;
    int order_of_print[size2];
    name_symbol = (char*) (elf_file + strtab->sh_offset);
    value = malloc(sizeof(char *) * size2);
   // printf("SIZE = %d and SIZE2 = %d\n", size2, tmp / sizeof(Elf64_Sym));
    i = 0;
   // printf("lol\n");
    //printf("ok\n");
    //printf("SIZE = %d and SIZE2 = %d\n", size2, tmp / sizeof(Elf64_Sym));
    //printf("SYMTAB st_shndx, SHN_UNDEF = %d\nSHN_ABS = %d\nSHN_COMMON = %d\n", SHN_UNDEF, SHN_ABS, SHN_COMMON);
    //printf("ELF_SECTION sh_type, SHT_NOBITS = %d\nSHT_PROGBITS = %d\nSHT_DYNAMIC = %d\n", SHT_NOBITS, SHT_PROGBITS, SHT_DYNAMIC);
    //printf("ELF_SECTION sh_flag, SHF_ALLOC = %d\nSHF_WRITE = %d\nSHF_EXECINSTR = %d\n", SHF_ALLOC, SHF_WRITE, SHF_EXECINSTR);

    while (i < (tmp / sizeof(Elf64_Sym)))
    {
        type = get_type_x64(symtab[i], elf_section);
        length = 20 + ft_strlen(name_symbol + symtab[i].st_name);
        value[i] = malloc(sizeof(char) * length);
        value[i][length - 1] = '\0';
        fill_space(value[i], length - 1);
        //if (type != 'a' && symtab[order_of_print[i]].st_name)
        //{
        if (type != 'w' && type != 'U')
        {
            //value = malloc(sizeof(char) * 17);
            int_to_hex(symtab[i].st_value, value[i], 1);
            //printf("%s\n", value[i]);
            //ft_putstr(value);
            //write(1, " ", 1);
        }
        fill_type_name(value[i], type, name_symbol + symtab[i].st_name, 1);
        //else
           // write(1, "                 ", 17);
        //write(1, &type, 1);
        //write(1, " ", 1);
        //ft_putstr(name_symbol + symtab[order_of_print[i]].st_name);
        //write(1, "\n", 1);

            
            
            //printf("%s %c %s\n", value,  type, name_symbol + symtab[order_of_print[i]].st_name);
        //if (value)
        //    free(value);
        //value = NULL;
        //}
        //printf("%s\n", value[i]);
        i++;
        
    }
    //exit(0);
    sort_array(order_of_print, size2, value, 1);
    i = 0;
    while (i < (tmp / sizeof(Elf64_Sym)))
    {
        //printf("i = %d, and order of print = %d and value = %s\n", i , order_of_print[i], value[order_of_print[i]]);
        type = value[order_of_print[i]][17];
        if (type != 'a' && symtab[order_of_print[i]].st_name)
        {
            ft_putstr(value[order_of_print[i]]);
            write(1, "\n", 1);
        }
        free(value[order_of_print[i]]);
        i++;
    }
    free(value);
    return (0);
}