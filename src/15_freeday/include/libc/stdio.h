#pragma once


void scroll();

void update_cursor();

void write_char(int colour, char character);

void write_string(int colour, const char *string);

char * itoa( int value, char * str, int base );

void write_int(int value);

void my_printf(const char *format, ...);