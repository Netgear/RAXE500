/***********************************************************************
 * <:copyright-BRCM:2017:DUAL/GPL:standard
 * 
 *    Copyright (c) 2017 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 ************************************************************************/

#include "boot_block_housekeeping.h"

static int verbose=0;
#define PRINTF(a,...) if (verbose) { printf(a, ##__VA_ARGS__);}

typedef void (*sighandler_t)(int);

static int keep_running=1, read_boot_block_check_info=0;
static unsigned char *saved_valid_cfe=NULL;
static int saved_valid_cfe_size=0;
static NVRAM_DATA l_nvram_data;
static BOOT_BLOCK_MIRROR_INFO nmi;

void sig_usr1(int code)
{
	keep_running=0; 
}

void sig_usr2(int code)
{
	verbose=1;
	read_boot_block_check_info=1;
}




char *img_type_str[]={"", "nvram", "cferom"};

static time_t get_uptime()
{
	struct sysinfo sys_info;

	if(sysinfo(&sys_info) == 0)
	{
		return sys_info.uptime;
	}
	return 0;
}

static int update_bootblock_check_info(bootblock_check_info *bci)
{
	int fd;

	fd = open(BOOT_BLOCK_CHECK_INFO_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0644);

	if(fd >= 0)
	{
		write(fd, bci, sizeof(*bci));
		close(fd);
		sync();
	}
	return 0;
}

static void init_bootblock_check_info(bootblock_check_info *bci)
{
	if(bci != NULL)
	{
		bci->boot_count=1;
		bci->last_check_time=HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK;
	}
}

static int read_bootblock_check_info(bootblock_check_info *bci)
{
	int fd=0;
	int rc=-1;

	fd = open(BOOT_BLOCK_CHECK_INFO_FILE, O_RDONLY);

	if(fd >= 0)
	{
		if(read(fd, bci, sizeof(*bci)) != sizeof(*bci))
		{
			init_bootblock_check_info(bci);
			update_bootblock_check_info(bci);
		}
		else
			close(fd);
	}
	else
	{
		init_bootblock_check_info(bci);
		update_bootblock_check_info(bci);
	}
	return rc;
}

static int time_to_check_nvram_validity(bootblock_check_info *bci)
{
	int rc=0;
	static int last_uptime=0,ut=0;

	if(bci != NULL)
	{

		ut=get_uptime();
		PRINTF("current uptime %x last_saved_uptime %x bci last_check_time %x\n", ut, last_uptime, bci->last_check_time);

		PRINTF("bci->boot_count  %d %d\n", bci->boot_count, HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS );
		if(bci->last_check_time-(ut-last_uptime) <=0)
		{
			PRINTF("Time to check, %x %x \n", bci->last_check_time, (ut-last_uptime)); 
			bci->last_check_time=HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK;
			bci->boot_count=1;
			last_uptime=ut;
			rc=1;
		}
		else if(bci->boot_count >= HOUSEKEEPING_BOOTBLOCK_CHECK_BOOT_COUNTS)
		{
			bci->last_check_time=HOUSEKEEPING_BOOTBLOCK_TIME_UNTIL_NEXT_CHECK;
			bci->boot_count=1;
			last_uptime=ut;
			rc=1;
		}
		if(ut-last_uptime >= HOUSEKEEPING_BOOTBLOCK_CHECK_FILE_SYNC_TIME)
		{
			PRINTF("reduce lct in bci %x %x %x\n", bci->last_check_time,ut,last_uptime);
			//update bci file
			bci->last_check_time-=(ut-last_uptime);
			PRINTF("reduce lct in bci %x %x %x\n", bci->last_check_time,ut,last_uptime);
			last_uptime=ut;
			update_bootblock_check_info(bci);
		}
	}

	return rc;
}

static int get_hour_of_the_day(void )
{

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	PRINTF("get_hour_of_the_day %d\n", tm.tm_hour);


	return tm.tm_hour;
}

static int mread(int mtd_fd, void *data_ptr, int data_len)
{
	struct mtd_ecc_stats ecc_stats;
	int status, fails;

	ioctl(mtd_fd, ECCGETSTATS, &ecc_stats);
	fails = ecc_stats.failed;

	status = read(mtd_fd, data_ptr, data_len);
	if (status < 0)
		return(status);

	ioctl(mtd_fd, ECCGETSTATS, &ecc_stats);

	if (fails != ecc_stats.failed)
		return(-1);

	return(status);
}


static int nandReadBlk(mtd_info_t *mtd, int blk_addr, int data_len,
                       unsigned char *data_ptr, int mtd_fd)
{
	int err;
	if (lseek(mtd_fd, blk_addr, SEEK_SET) < 0)
	{
		//fprintf(stderr, "ERROR!!! Could not block seek to 0x%x!\n", blk_addr);
		return(-1);
	}

	if ((err=mread(mtd_fd, data_ptr, data_len)) < 0)
	{
		//fprintf(stderr, "ERROR!!! Could not read block at offset 0x%x => %d!\n", blk_addr, err);
		if(mtd->size != data_len)
			return(-1);
	}

	return(0);
}

void read_nvram_from_kernel(void)
{
	BOARD_IOCTL_PARMS ctrlParms;
	int fd;

	fd = open("/dev/brcmboard", O_RDONLY);
	if(fd >= 0)
	{
		memset(&l_nvram_data, '\0', sizeof(l_nvram_data));
		memset(&ctrlParms, '\0', sizeof(ctrlParms));
		ctrlParms.action=NVRAM;
		ctrlParms.strLen=sizeof(l_nvram_data);
		ctrlParms.string=(char*)&l_nvram_data;
		ctrlParms.offset=0;
		ctrlParms.buf=NULL;
		ctrlParms.result=-1;
		if(ioctl(fd, BOARD_IOCTL_FLASH_READ, &ctrlParms) != 0)
		{
			fprintf(stderr, "NVRAM read failure, thats odd!\n");
		}
		close(fd);
	}
		
}

int scan_mirror_info(int mtd_fd, mtd_info_t *mtd)
{
	unsigned char *buffer=NULL;

	buffer=malloc(mtd->size);

	if(buffer != NULL)
	{
		memset(&nmi, '\0', sizeof(nmi));
		//read block
		if(nandReadBlk(mtd,  0,  mtd->size, buffer, mtd_fd) == 0)
		{
			create_boot_block_info_file((char*)buffer, mtd->size, &l_nvram_data, mtd);
		}
		
		free(buffer);
	}

	return 0;
}

static int read_mirror_info()
{

	int fd=0;
	int rc=-1;

	fd = open(BOOT_BLOCK_MIRROR_INFO_FILE, O_RDONLY);

	if(fd >= 0)
	{
		memset(&nmi, '\0', sizeof(nmi));
		if(read(fd, &nmi, sizeof(nmi)) != sizeof(nmi))
		{
			PRINTF("corrupt %s file\n", BOOT_BLOCK_MIRROR_INFO_FILE);
			memset(&nmi, '\0', sizeof(nmi));
		}
		else
			rc=0;
		close(fd);
	}
	return rc;
}

static int update_mirror_info(BOOT_BLOCK_MIRROR_INFO *nmi)
{

	int fd=0;
	int rc=-1;

	fd = open(BOOT_BLOCK_MIRROR_INFO_FILE, O_CREAT|O_WRONLY|O_TRUNC, 0644);

	if(fd >= 0)
	{
		if(write(fd, nmi, sizeof(*nmi)) != sizeof(*nmi))
		{
			fprintf(stderr, " write %s file failed\n", BOOT_BLOCK_MIRROR_INFO_FILE);
		}
		else
			rc=0;
		close(fd);
	}
	return rc;
}

static  int update_nvram(int mtd_fd, mtd_info_t *mtd, unsigned long offset, int len, unsigned char *b)
{
	int err=0;

	PRINTF(" Nvram syncing at %lx....\n", offset);
	err += nandEraseBlk(mtd, offset, mtd_fd);
	err += nandWriteBlk(mtd, offset, len, b, mtd_fd, 0);

	return err;

}

int update_image(int mtd_fd, mtd_info_t *mtd, unsigned long offset, unsigned char *image, int size)
{

	unsigned long block_start_address=0, block_offset=0;
	int to_write,err=0;
	unsigned char *buffer=NULL;


	buffer=malloc(mtd->erasesize);

	PRINTF("update_image:\n");
	if(buffer != NULL)
	{
		while(size > 0 && err ==0 )
		{
			// get block number
			block_start_address=offset&~(mtd->erasesize-1);

			PRINTF("block_start %lx offset %lx size %d[%x]\n", block_start_address, offset, size, size);
			//read block
			if((err+=nandReadBlk(mtd,  block_start_address,  mtd->erasesize, buffer, mtd_fd)) == 0 || 1)
			{
				block_offset=offset-block_start_address;

				to_write=mtd->erasesize-block_offset;
				PRINTF("block offset %lx, to_write %x[%d] , size %x[%d] \n", block_offset, to_write, to_write, size, size);
				if(to_write > size)
					to_write=size;
				PRINTF("\nblock_offset %lx write %x\n", block_offset, to_write);
				//copy image
				memcpy(buffer+block_offset, image, to_write);

				PRINTF("image [%02x] [%02x] \n", image[0], image[1]);
				//write block
				err += nandEraseBlk(mtd, block_start_address, mtd_fd);

				err += nandWriteBlk(mtd, block_start_address, mtd->erasesize, buffer, mtd_fd, 0);

				PRINTF("Block write %d\n", err);
				size=size-to_write;
				image+=to_write;
				offset+=to_write;
			}
		}
		free(buffer);
	}
	else
		err=-2;

	return err;
}

int sync_mirrors(int mtd_fd, mtd_info_t *mtd, int dry_run)
{

	int i=0, rom_size=0;
	unsigned char *buf=NULL;
	char *sign=NVRAM_DATA_SIGN;
	int check_sign=1, img_type, repeat=0;
	int update_counter=0, check_once=1, cferom_crc=0;

	if( read_mirror_info() != 0)
	{
		scan_mirror_info(mtd_fd, mtd);
		read_mirror_info();
		return 0;
	}

	if(mtd >= 0)
	{
		buf=malloc(mtd->size);
		if(buf != NULL)
		{
			do
			{
				repeat=0;
				i=0;
				if(nandReadBlk(mtd,  0,  mtd->size, buf, mtd_fd) == 0)
				{
					while(i<MAX_MIRRORS)
					{
						if(nmi.offset[i] != 0 && nmi.offset[i] < mtd->size )
						{
							PRINTF("Search %d [%s]\tat\t%lx wa [%d]\t", nmi.image_type[i], img_type_str[nmi.image_type[i]&3], nmi.offset[i], nmi.write_fail_count[i]);
							if(nmi.offset[i] < 2048)
							{
								PRINTF("Skipping precferom at %lx\n",  nmi.offset[i]);
							}
							if(nmi.image_type[i] == IMG_NVRAM)
							{

								check_sign=1;
								sign=NVRAM_DATA_SIGN;
								if(nmi.offset[i] == nvramDataOffset(mtd))
								{
									check_sign=0;
									sign="";
								}

								if((img_type=is_nvram_offset(buf, nmi.offset[i], check_sign, &rom_size, &l_nvram_data)) != IMG_NVRAM)
								{
									if(img_type == CRC_MISMATCH || img_type == IMG_MISSING)
									{
										PRINTF("\n\nStale/Missing NVRAM %lx \n\n", nmi.offset[i]);

										if(!dry_run)
										{

											if(nmi.write_fail_count[i] < MAX_MIRROR_RECOVERY_ATTEMPTS)
											{
												nmi.write_fail_count[i]++;
												//nandReadBlk(mtd,  nmi.offset[i]&~(mtd->erasesize-1),  mtd->erasesize, buf, mtd_fd);
												strncpy((char *)buf+nmi.offset[i], sign, strlen(sign));
												memcpy((char *)buf+nmi.offset[i]+strlen(sign), (char*)&l_nvram_data, sizeof(l_nvram_data));
												update_nvram(mtd_fd, mtd, nmi.offset[i]&~(mtd->erasesize-1), mtd->erasesize, buf+(nmi.offset[i]&~(mtd->erasesize-1)));
											}
											else
												fprintf(stderr, "Exceed recovery attempts on %lx(%lx) block %ld\n", nmi.offset[i], nmi.offset[i]&~(mtd->erasesize-1), nmi.offset[i]/mtd->erasesize);
										}
										update_counter++;
									}
								}
								else
									PRINTF("Found\tat\t%lx block\t%ld\n", nmi.offset[i], nmi.offset[i]/mtd->erasesize);
							}
							else if(nmi.image_type[i] == IMG_CFEROM)
							{
								//check cferom integrity
								if(is_cferom_offset(buf, nmi.offset[i], &rom_size, &cferom_crc, &saved_valid_cfe) == IMG_CFEROM)
								{
									saved_valid_cfe_size=rom_size;
									if(check_once == 1 && nmi.cferom_crc != cferom_crc)
									{
										fprintf(stderr, "version mismatch, recreating %s [%x] [%x]\n", BOOT_BLOCK_MIRROR_INFO_FILE, nmi.cferom_crc, cferom_crc);
										update_counter=BOOT_BLOCK_MIRROR_INFO_VERSION_MISMATCH;
										repeat=0;
										break;
									}
									// check only for first cferom found, as flashed image might contain mix cferom binary (secure, unsecure?)
									check_once=0;
									PRINTF("found\tat\t%lx block\t%ld %x[%d] \n", nmi.offset[i], nmi.offset[i]/mtd->erasesize,rom_size, rom_size);
								}
								else
								{
									PRINTF("missing, update \n");

									if(saved_valid_cfe == NULL)
										repeat=1;
									else
									{
										memcpy((char *)buf+nmi.offset[i], (char*)saved_valid_cfe, saved_valid_cfe_size);
										if(!dry_run)
										{
											if(nmi.write_fail_count[i] < MAX_MIRROR_RECOVERY_ATTEMPTS)
											{
												nmi.write_fail_count[i]++;
												update_image(mtd_fd, mtd, nmi.offset[i], saved_valid_cfe, saved_valid_cfe_size);
											}
											else
												fprintf(stderr, "Exceed recovery attempts on %lx(%lx) block %ld\n", nmi.offset[i], nmi.offset[i]&~(mtd->erasesize-1), nmi.offset[i]/mtd->erasesize);
										}
										update_counter++;
									}
								}
							}

						}
						else
							break;
						i++;
					}

				}
			} while(repeat);
			free(buf);

			if(cferom_crc == 0) // we didn't find any cferom at offset specified in the bootblock mirror info file.
			{
				fprintf(stderr, "end, version mismatch, recreating %s\n", BOOT_BLOCK_MIRROR_INFO_FILE);
				update_counter=BOOT_BLOCK_MIRROR_INFO_VERSION_MISMATCH;
				repeat=0;
			}

			if(update_counter > 0)
			{
				update_mirror_info(&nmi);
				sync();
			}
		}
	}


	return update_counter;

}

int main(int argc, char *argv[])
{

	mtd_info_t *mtd=NULL;
	int mtd_fd,  sleep_time=HOUSEKEEPING_SLEEP_DURATION;
	bootblock_check_info bci;

	if(argc == 2 )
	{
		if(strcmp(argv[1], "-v") == 0)
			verbose=1;
	}

	if((mtd_fd = open(VERBOSE_FILE, O_RDONLY)) >= 0 )
	{
		verbose=1;
		close(mtd_fd);
	}

	setbuf(stderr, NULL);
	setbuf(stdout, NULL);

	signal (SIGUSR1, sig_usr1);
	signal (SIGUSR2, sig_usr2);

	mtd=get_mtd_device_handle("nvram", &mtd_fd, 0);

	if(mtd)
	{

		//read bootblock_check_info
		read_bootblock_check_info(&bci);
		bci.boot_count++;
		update_bootblock_check_info(&bci);
		while(keep_running)
		{
			PRINTF("Keep running %d\n", keep_running);
			if(get_hour_of_the_day() == BOOTBLOCK_CHECK_TIME) //check only at 1am
			{
				if(time_to_check_nvram_validity(&bci))
				{
					read_nvram_from_kernel();
					if(sync_mirrors(mtd_fd, mtd, 0) == BOOT_BLOCK_MIRROR_INFO_VERSION_MISMATCH)
						scan_mirror_info(mtd_fd, mtd);
					update_bootblock_check_info(&bci);
				}
			}
			sleep(sleep_time);
			if(read_boot_block_check_info == 1) //did we receive SIGUSR2?
			{
				read_bootblock_check_info(&bci);
				read_boot_block_check_info=0;
			}
		}
	}
	else
	{
		PRINTF("Error: NVRAM boot block not present\n");
	}

	return 0;
}
