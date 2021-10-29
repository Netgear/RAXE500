// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

/* #define	DEBUG	*/

#include <common.h>
#include <autoboot.h>
#include <cli.h>
#include <console.h>
#include <version.h>
#if defined(CONFIG_BCMBCA_BUTTON)
#include "bcmbca_button.h"
#endif

/*
 * Board-specific Platform code can reimplement show_boot_progress () if needed
 */
__weak void show_boot_progress(int val) {}

static void run_preboot_environment_command(void)
{
#ifdef CONFIG_PREBOOT
	char *p;

	p = env_get("preboot");
	if (p != NULL) {
		int prev = 0;

		if (IS_ENABLED(CONFIG_AUTOBOOT_KEYED))
			prev = disable_ctrlc(1); /* disable Ctrl-C checking */

		run_command_list(p, -1, 0);

		if (IS_ENABLED(CONFIG_AUTOBOOT_KEYED))
			disable_ctrlc(prev);	/* restore Ctrl-C checking */
	}
#endif /* CONFIG_PREBOOT */
}

int g_restore_default;

void check_restore_default(void)
{
#define Button_Reg 0xff800528	
#define Reset_GPIO_Mask 0x00200000	
	unsigned int *addr_button = (unsigned int*)Button_Reg;
	int i;
	
	g_restore_default=1;
	for( i=0; i<50; i++ )
	{
		  if( ((*addr_button) & Reset_GPIO_Mask) != 0 )  // Reset button not pressed
		  {
			    g_restore_default=0;
			    break;
		  }
			
      udelay(100000); //delay 100ms
	}
	
	if( g_restore_default==1 )
	{	
		  printf("===== Restore default =====\n");
		  run_command("sdk restoredefault", 0);

    	for( i=0; i<20; i++ )
    	{
		      if ((i % 2) == 0) {
			        board_test_led(0);
		       } else {
			        board_test_led(1);
		       }
			     udelay(250000);
       }
       board_test_led(0);
	  }
	  return;
}

void bootup_turn_on_power_led(void)
{
	//power up, turn on power led
	printf("===== POWER ON TURN ON POWER LED =====\n");
	board_test_led(0);
}

/* We come here after U-Boot is initialised and ready to process commands */
void main_loop(void)
{
	const char *s;

	bootstage_mark_name(BOOTSTAGE_ID_MAIN_LOOP, "main_loop");

	if (IS_ENABLED(CONFIG_VERSION_VARIABLE))
		env_set("ver", version_string);  /* set version variable */

	cli_init();
	/*Foxconn add start*/
	bootup_turn_on_power_led();
	/*Foxconn add end*/
	run_preboot_environment_command();

  check_restore_default();
  
#ifdef CONFIG_SYS_NMRP
    StartNmrpClient();
#endif

	if (IS_ENABLED(CONFIG_UPDATE_TFTP))
		update_tftp(0UL, NULL, NULL);

#if defined(CONFIG_BCMBCA_BUTTON)
        btn_poll_block();
#endif	
	s = bootdelay_process();
	if (cli_process_fdt(&s))
		cli_secure_boot_cmd(s);

	autoboot_command(s);

	cli_loop();
	panic("No CLI available");
}
