/*
 * BlinkLed.c
 *
 * Created: 1/21/2022 9:00:05 PM
 * Author : Derek Chang and David Lieu
 */  

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#define KeypadDirectionRegister DDRC
#define KeypadPortControl PORTC
#define KeypardPortValue PINC

#define LEDDirectionRegister DDRD
#define LEDPort PORTD

const char DAYSINMONTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


#include <avr/io.h>
#include <util/delay.h>
#include "avr.h"
#include "lcd.h"
#include <stdbool.h>

bool isMilitary = true;
bool isEdit = false;

int get_key() {
	int i,j;
	for(i = 0; i < 4; ++i) {
		for(j=0; j < 4; ++j) {
			if (is_pressed(i,j)) {
				return i * 4 + j + 1;
			}
		}
		
	}
	return 0;
	
}
int is_pressed(int r, int c) {
	
	DDRC = 0;			// all 8 GPIOs to N/C	
	PORTC = 0;				
	
	CLR_BIT(PORTC, r);	// set r to "0"
	SET_BIT(DDRC, r);	
	
	SET_BIT(PORTC, c+4);		// set c to "w1"
	CLR_BIT(DDRC, c+4);	
	
	avr_wait(1);
	if (!GET_BIT(PINC, c+4)) {
		return 1;
	}
	return 0;
}

// Maps int k to keypad character
// Writes keypad character into buf (max 2 characters)
void update_buf(char *buf, int k) {
	switch(k) {
		case 1:
		sprintf(buf, "%2d", 1);
		break;
		case 2:
		sprintf(buf, "%2d", 2);
		break;
		case 3:
		sprintf(buf, "%2d", 3);
		break;
		case 4:
		sprintf(buf, "A");
		break;
		case 5:
		sprintf(buf, "%2d", 4);
		break;
		case 6:
		sprintf(buf, "%2d", 5);
		break;
		case 7:
		sprintf(buf, "%2d", 6);
		break;
		case 8:
		sprintf(buf, "B");
		break;
		case 9:
		sprintf(buf, "%2d", 7);
		break;
		case 10:
		sprintf(buf, "%2d", 8);
		break;
		case 11:
		sprintf(buf, "%2d", 9);
		break;
		case 12:
		sprintf(buf, "C");
		break;
		case 13:
		sprintf(buf, "*");
		break;
		case 14:
		sprintf(buf, "%2d", 0);
		break;
		case 15:
		sprintf(buf, "%2d", 15);
		break;
		case 16:
		sprintf(buf, "D");
		break;
		default:
		sprintf(buf, "EE");
		break;
	}
}

struct dt {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};

void init_dt(struct dt *dt) {
	dt->year = 2022;
	dt->month = 2;
	dt->day = 28;
	dt->hour = 23;
	dt->minute = 59;
	dt->second = 0; 
}

void advance_dt(struct dt *dt) {
	++dt->second;
	if (60 == dt->second) {
		dt->second = 0;
		++dt->minute;
		if (60 == dt->minute) {
			dt->minute = 0;
			++dt->hour;
			if (24 == dt->hour) {
				dt->hour = 0;
				++dt->day;
				if (DAYSINMONTH[dt->month - 1] + 1 == dt->day) {
					dt->day = 1;
					++dt->month; 
					if (12 == dt->month) {
						dt->month = 1;
						++dt->year;
					}
				}
			}
		}
	}
}

void print_dt(const struct dt *dt) {
	char buf[17];
	lcd_pos(0,0);
	sprintf(buf, "%02d / %02d / %04d ",
		dt->month,
		dt->day,
		dt->year);
	lcd_puts2(buf);
	lcd_pos(1,0);
	sprintf(buf, "%02d : %02d : %02d ",
		dt->hour,
		dt->minute,
		dt->second);
	lcd_puts2(buf);
}

void blink_dt(const struct dt *dt, int editField) {
	char buf[17];
	switch (editField) {
		case 1:
			lcd_pos(0,0);
			sprintf(buf, "   / %02d / %04d ",
			dt->day,
			dt->year);
			break;
		case 2:
			lcd_pos(0,0);
			sprintf(buf, "%02d /    / %04d ",
			dt->month,
			dt->year);
			break;
		case 3:
			lcd_pos(0,0);
			sprintf(buf, "%02d / %02d /     ",
			dt->month,
			dt->day);
			break;
		case 4:
			lcd_pos(1,0);
			sprintf(buf, "   : %02d : %02d",
			dt->minute,
			dt->second);
			break;
		case 5:
			lcd_pos(1,0);
			sprintf(buf, "%02d :    : %02d",
			dt->hour,
			dt->second);
			break;
		case 6:
			lcd_pos(1,0);
			sprintf(buf, "%02d : %02d :   ",
			dt->hour,
			dt->minute);
			break;
		default:
			break;
	}
	lcd_puts2(buf);
}
/*
void copy_dt(struct dt *origin, struct dt *dest) {
	dest->year	 = origin->year;
	dest->month	 = origin->month;
	dest->day	 = origin->day;
	dest->hour	 = origin->hour;
	dest->minute = origin->minute;
	dest->second = origin->second;
}*/
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
	
	while(1) {
		avr_wait(500);

		// A
		if (get_key() == 4) {
			if (!isEdit) {
				isEdit = true;
				//copy_dt(dt, dt_buf);
			} else {
				isEdit = false;
				// checkValidInput()
			}
		}
		
		if (isEdit) {
			// B -> goToNextField
			if (get_key() == 8) {
				editField++;
			}
			//if (get_key() == )
			if (editField > 6) { editField = 1; }
			blink_dt(&dt, editField);
		}
		
		// C -> handle military timing toggle and output indicator
		if (!isEdit && get_key() == 12) { isMilitary = isMilitary ? false : true; }
		lcd_pos(1,13);
		if (isMilitary) { lcd_puts2("24H"); }
		else { lcd_puts2(" AM"); }
			
			
		avr_wait(500);
		advance_dt(&dt);
		print_dt(&dt);
	}
	
	return(0);				// should never get here, this is to prevent a compiler warning
}