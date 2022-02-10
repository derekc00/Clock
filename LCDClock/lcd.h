/*
 * lcd.h
 *
 * Created: 2/3/2022 7:58:41 PM
 *  Author: derek
 */ 

#ifndef _LCD_H
#define _LCD_H
#include <stdbool.h>
#include <string.h>

void lcd_init(void);

void lcd_clr(void);

void lcd_pos(unsigned char r, unsigned char c);

void lcd_put(char c);

void lcd_puts1(const char *s);

void lcd_puts2(const char *s);

int is_pressed(int r, int c);

int get_key();

void keypadIntToString(char *buf, int k);

bool isNumberPressed(const char *buf);
#endif /* _LCD_H */