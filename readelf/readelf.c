/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Phdr *phdr = NULL;

        u_char *ptr_ph_table = NULL;
        Elf32_Half ph_entry_count;
        Elf32_Half ph_entry_size;


        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }

        // get section table addr, section header number and section header size.
	ptr_ph_table = binary + ehdr->e_phoff;
	ph_entry_count = ehdr->e_phnum;
	ph_entry_size = ehdr->e_phentsize;
	int find = 1;//find = 1 means it is not wrong
     	u_char *ptr_ph_table_i = ptr_ph_table;
	Elf32_Phdr *phdr_i = NULL;
	Elf32_Phdr *phdr_j = NULL;
	for (int i = 0;i < ph_entry_count;i++) {
		phdr_i = (Elf32_Phdr*) ptr_ph_table_i;
		u_char *ptr_ph_table_j = ptr_ph_table;
		for (int j = 0;j < ph_entry_count;j++) {
			phdr_j = (Elf32_Phdr*) ptr_ph_table_j;
			Elf32_Addr l2 = phdr_i->p_vaddr + phdr_i->p_memsz;
			Elf32_Addr r1 = phdr_j->p_vaddr;
			if ( l2/4096 == r1/4096 ) {
					printf("Overlay at page va : 0x%x\n",l2/4096 * 4096);
				find = 0;
				break;	
			}
			if (l2 > r1) {
                                printf("Conflict at page va : 0x%x\n",r1/4096 * 4096);
                        	find = 0;
                                break;         
			}
			ptr_ph_table_j += ph_entry_size;
		}	
		ptr_ph_table_i += ph_entry_size;
		if (!find)	break;
	}
	
	if (find) {
     		ptr_ph_table = binary + ehdr->e_phoff;
	  	for (int i = 0;i < ph_entry_count;i++) {
			phdr = (Elf32_Phdr*) ptr_ph_table;
			printf("%d:0x%x,0x%x\n", i, phdr->p_filesz, phdr->p_memsz);
			ptr_ph_table += ph_entry_size;
		}
	}

        return 0;
}

