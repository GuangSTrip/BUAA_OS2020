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

int raid4_valid(u_int diskno) {
	u_char r = 0;
	u_char tmp = 0;
	u_int offset = 0;
	int dev = 0x13000000;
	if (syscall_write_dev((u_int)&diskno, (dev + 0x10), 4) < 0) {
		user_panic("ide_read wrong!!");
        }
	if (syscall_write_dev((u_int)&offset, dev, 4) < 0) {
                user_panic("ide_read wrong!!");
        }
	syscall_write_dev(&tmp, (dev + 0x20), 1);
	syscall_read_dev(&r, (dev + 0x30), 1);
	if (r == 0) return 0;
	else return 1;
}

int raid4_write(u_int blockno, void *src) {
	int i;
	int dino;
	int seno;
	int ans = 0;
	u_int offset;
	for (i = 0; i < 8; i++) {
		dino = (i % 4) + 1;
		seno = 2 * blockno + (i / 4);
		offset = src + i * 0x200;
		if (raid4_valid(dino)) {
			ide_write(dino, seno, (void *)offset, 1);
		} else {
			if (i < 4) {
				ans++;
			}
		}
	}
	int jo[128];
	int *a, *b, *c, *d;
	a = src;
	b = src + 512;
	c = src + 1024;
	d = src + 1536;
	for (i = 0; i < 16; i++) {
		jo[i] = a[i] | b[i] | c[i] | d[i];
	}
	dino = 5;
	seno = 2 * blockno;
	offset = &jo;
	if (raid4_valid(dino)) {
                        ide_write(dino, seno, (void *)offset, 1);
                } else {
                     	ans++;
                }
	//ide_write(dino, seno, (void *)offset, 1);
	a = src + 2048;
        b = src + 2560;
        c = src + 3072;
        d = src + 3584;
        for (i = 0; i < 16; i++) {
                jo[i] = a[i] | b[i] | c[i] | d[i];
        }
	dino = 5;
        seno = 2 * blockno + 1;
        offset = &jo;
        //ide_write(dino, seno, (void *)offset, 1);
	if (raid4_valid(dino)) {
		ide_write(dino, seno, (void *)offset, 1);
	}
	return ans;
}

int raid4_read(u_int blockno, void *dst) {
	int i;
	int j;
	int k;
	int count = 0;
	int brodisk;
	int ans = 0;
	u_int offset;
	int dino;
	int seno;
	int jo[128];
	int addr[5];
	for (i = 1; i <= 5; i++) {
		if (raid4_valid(i) == 0) {
			count++;
			brodisk = i;
		}
	}
	if (count > 1) {
		return count;
	}
	if (count == 0) {
		for (i = 0; i < 8; i++) {
                	dino = (i % 4) + 1;
        	        seno = 2 * blockno + (i / 4);
	                offset = dst + i * 0x200;
                	ide_read(dino, seno, (void *)offset, 1);
		}
		dino = 5;
                seno = 2 * blockno;
                offset = &jo;
                ide_read(dino, seno, (void *)offset, 1);
                addr[1] = dst; addr[2] = dst + 512; addr[3] = dst + 1024; addr[4] = dst + 1536;
		for (i = 0; i < 16; i++) {
			k = 0;
			for (j = 1; j < 5; j++) k |= *(int *)(addr[j] + i * 32);
			if (k != jo[i]) {
				return -1;
			}
		}
		dino = 5;
                seno = 2 * blockno + 1;
                offset = &jo;
                ide_read(dino, seno, (void *)offset, 1);
                //addr[1] = dst; addr[2] = dst + 512; addr[3] = dst + 1024; addr[4] = dst + 1536;
                addr[1] = dst + 2048; addr[2] = dst + 2560; addr[3] = dst + 3072; addr[4] = dst + 3584;
		for (i = 0; i < 16; i++) {
                        k = 0;
                        for (j = 1; j < 5; j++) k |= *(int *)(addr[j] + i * 32);
                        if (k != jo[i]) {
                                return -1;
                        }
                }
	} else if (brodisk == 5) {
		for (i = 0; i < 8; i++) {
                        dino = (i % 4) + 1;
                        seno = 2 * blockno + (i / 4);
                        offset = dst + i * 0x200;
                        ide_read(dino, seno, (void *)offset, 1);
                }
		ans = 1;
	} else {
		for (i = 0; i < 8; i++) {
                        dino = (i % 4) + 1;
                        seno = 2 * blockno + (i / 4);
                        offset = dst + i * 0x200;
                        if (raid4_valid(dino)) {
				ide_read(dino, seno, (void *)offset, 1);
			}
                }
		dino = 5;
		seno = 2 * blockno;
		offset = &jo;
		ide_read(dino, seno, (void *)offset, 1);
		addr[1] = dst; addr[2] = dst + 512; addr[3] = dst + 1024; addr[4] = dst + 1536;
		for (i = 0; i < 16; i++) {
			for (j = 1;j < 5;j++) {
				if (raid4_valid(j)) {
					jo[i] |= *(int *)(addr[j]);
				}	
			}
		}
		user_bcopy((void *)&jo, (void *)(dst + (brodisk - 1) * 512), 512);
		dino = 5;
                seno = 2 * blockno + 1;
                offset = &jo;
                ide_read(dino, seno, (void *)offset, 1);
                addr[1] = dst + 2048; addr[2] = dst + 2560; addr[3] = dst + 3072; addr[4] = dst + 3584;
                for (i = 0; i < 16; i++) {
                        for (j = 1;j < 5;j++) {
                                if (raid4_valid(j)) {
                                        jo[i] |= *(int *)(addr[j]);
                                } 
                        }
                }
                user_bcopy((void *)&jo, (void *)(dst + (brodisk + 3) * 512), 512); 
		ans = 1;
	}
	return ans;
}

