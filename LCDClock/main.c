/*
 * BlinkLed.c
 *
 * Created: 1/21/2022 9:00:05 PM
 * Author : Derek Chang and David Lieu
 */  

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "avr.h"
#include "lcd.h"
#include "dt.h"

int main(void)
{
	lcd_init();
	lcd_pos(0,0);
	lcd_clr();
	lcd_puts2("starting...");
	avr_wait(500);
	
	struct dt dt;
	struct dt dt_buf;
	init_dt(&dt);
	int editField = 1;
	bool isMilitary = true;
	bool isEdit = false;
	
	while(1) {
		avr_wait(500);

		// A
		if (get_key() == 4) {
			if (!isEdit) {
				isEdit = true;
				copy_dt(&dt, &dt_buf); // copy into buffer dt struct
			} else {
				isEdit = false;
				editField = 1;			// reset editField back to top right (month)
				if (isValid(&dt_buf)) { // if it is a valid time edit, set it as the main time
					copy_dt(&dt_buf, &dt);
				}
			}
		}
		
		if (isEdit) {
			
			int keyPressed = get_key();
			char temp[2];
			keypadIntToString(temp, keyPressed);
			
			avr_wait(1);
			
			if (keyPressed == 8){
				editField++;
			} else if (isNumberPressed(temp)) {
				editDate(&dt_buf, editField, atoi(temp));
			}
			
			if (editField > 6) { editField = 1; } // wrap editField to 6 max
			
			blink_dt(&dt_buf, editField);
		}
		
		advance_dt(&dt);
		avr_wait(500);


		// C -> handle military timing toggle and output indicator
		if (!isEdit && get_key() == 12) { isMilitary = isMilitary ? false : true; }
		struct dt military_buf;
		char indicator[3];

		copy_dt(&dt, &military_buf);
		convertMilitaryToAMPM(&military_buf, indicator);
		
		lcd_pos(1,13);
		if (isMilitary) { lcd_puts2("24H"); }
		else { lcd_puts2(indicator); }
			
			
		
		if (isEdit) { print_dt(&dt_buf); } // while in edit mode, print buf instead of main dt
		else { 
			if (isMilitary) {
				print_dt(&dt);
			} else {
				print_dt(&military_buf);
			}
		}
	}
	
	return(0);				// should never get here, this is to prevent a compiler warning
}