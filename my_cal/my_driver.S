#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

.space 0x10000000
_my_getchar:
	li $t1, 0x00
	lb $t0, 0($t1)
	
_my_putchar:
	la $t0, 0xB0000000
	sb $a0, 0($t0)

_my_exit:
	.space 0x10000000
	li $t1, 0x10
	li $t0, 1
	sb $t0, 0($t1)


