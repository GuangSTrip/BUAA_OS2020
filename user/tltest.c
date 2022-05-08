#include "lib.h"

void umain() {
    writef(
        "Smashing some kernel codes...\n"
        "If your implementation is correct, you may see some TOO LOW here:\n"
    );
	//syscall_mem_alloc(0, 0x10000000, PTE_V);
	//syscall_mem_unmap(0, 0x10000000);
	//writef("finidsh!\n");
	*(int *) KERNBASE = 0;
    writef("My mission completed!\n");
}
