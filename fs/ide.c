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
	u_char write = 0;
	u_char r = 0;
	u_int tmp;
	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		if (syscall_write_dev((u_int)&diskno, (dev + 0x10), 4) < 0) {
         	       user_panic("ide_read wrong!!");
        	}
		tmp = offset_begin + offset;
		if (syscall_write_dev((u_int)&tmp, dev, 4) < 0) {
			user_panic("ide_read wrong!!");
		}		
		if (syscall_write_dev(&write, (dev + 0x20), 1) < 0) {
			user_panic("ide_read wrong!!");
		}
		if (syscall_read_dev(&r, (dev + 0x30), 1) < 0) {
			user_panic("ide_read wrong!!");
		}
		if (r == 0) {
			user_panic("ide_read wrong at read!!");
		}
		if (syscall_read_dev((int)(dst + offset), (dev + 0x4000), 0x200) < 0) {
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
	u_char write = 1;
	u_char r = 0;
	u_int tmp;
	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		if (syscall_write_dev((int)(src + offset), (dev + 0x4000), 0x200) < 0) {
                        user_panic("ide_read wrong!!");
                }
		if (syscall_write_dev((u_int)&diskno, (dev + 0x10), 4) < 0) {
                       user_panic("ide_read wrong!!");
                }
                tmp = offset_begin + offset;
                if (syscall_write_dev((u_int)&tmp, dev, 4) < 0) {
                        user_panic("ide_read wrong!!");
                }
                tmp = 1;
                if (syscall_write_dev(&write, (dev + 0x20), 1) < 0) {
                        user_panic("ide_read wrong!!");
                }
                if (syscall_read_dev(&r, (dev + 0x30), 1) < 0) {
                        user_panic("ide_read wrong!!");
                }
                if (r == 0) {
                        user_panic("ide_read wrong at read!!");
                }		
		offset += 0x200;
		// if error occur, then panic.
	}
}

//lab5-1-exam
int time_read() {
	u_char up = 1;
	int time;
	int addr = 0x15000000;
	if (syscall_write_dev(&up, addr, 1) < 0) {
		user_panic("time_read error!!");
	}
	if (syscall_read_dev(&time, (addr + 0x0010), 4) < 0) {
		user_panic("time_read error!!");
	}
	return time;
}

void raid0_write(u_int secno, void *src, u_int nsecs) {
	//int offset_begin = secno * 0x200;
	//int offset_end = offset_begin + nsecs * 0x200;
	int i;
	int offset;
	for (i = secno; i < secno + nsecs; i++) {
		offset = src + (i - secno) * 0x200;
		if (i % 2 == 0) {
			ide_write(1, i / 2, (void *)offset, 1);
		} else {
			ide_write(2, i / 2, (void *)offset, 1);
		}
	}
}

void raid0_read(u_int secno, void *dst, u_int nsecs) {
	int i;
        int offset;
        for (i = secno; i < secno + nsecs; i++) {
                offset = dst + (i - secno) * 0x200;
                if (i % 2 == 0) {
                        ide_write(1, i / 2, (void *)offset, 1);
                } else {
                        ide_write(2, i / 2, (void *)offset, 1);
                }
        }
}


