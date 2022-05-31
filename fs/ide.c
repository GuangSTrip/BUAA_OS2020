/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int dev = 0x13000000;
	int tmp;
	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		if (syscall_write_dev(&diskno, (dev + 0x10), 4) < 0) {
         	       user_panic("ide_read wrong!!");
        	}
		tmp = offset_begin + offset;
		if (syscall_write_dev(&tmp, dev, 4) < 0) {
			user_panic("ide_read wrong!!");
		}		
		tmp = 0;
		if (syscall_write_dev(&tmp, (dev + 0x20), 1) < 0) {
			user_panic("ide_read wrong!!");
		}
		if (syscall_read_dev(&tmp, (dev + 0x30), 1) < 0) {
			user_panic("ide_read wrong!!");
		}
		if (tmp == 0) [
			user_panic("ide_read wrong at read!!");
		}
		if (syscall_read_dev((u_int)(dst + offset), (dev + 0x4000), 0x200) < 0) {
			user_panic("ide_read wrong!!");
		}
		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here
	// int offset_begin = ;
	int offset_begin = secno * 0x200;
	// int offset_end = ;
	int offset_end = offset_begin + nsecs * 0x200;
	// int offset = ;
	int offset = 0;
	int dev = 0x13000000;
	int tmp;
	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		if (syscall_write_dev((u_int)(src + offset), (dev + 0x4000), 0x200) < 0) {
                        user_panic("ide_read wrong!!");
                }
		if (syscall_write_dev(&diskno, (dev + 0x10), 4) < 0) {
                       user_panic("ide_read wrong!!");
                }
                tmp = offset_begin + offset;
                if (syscall_write_dev(&tmp, dev, 4) < 0) {
                        user_panic("ide_read wrong!!");
                }
                tmp = 1;
                if (syscall_write_dev(&tmp, (dev + 0x20), 1) < 0) {
                        user_panic("ide_read wrong!!");
                }
                if (syscall_read_dev(&tmp, (dev + 0x30), 1) < 0) {
                        user_panic("ide_read wrong!!");
                }
                if (tmp == 0) [
                        user_panic("ide_read wrong at read!!");
                }		
		offset += 0x200;
		// if error occur, then panic.
	}

}
