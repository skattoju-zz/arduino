/* Serial Communications Interface */
#ifndef SERIAL_H
#define SERIAL_H

void serial_init(unsigned int BAUD_PRESCALE);
void _serial_write(const char c);
char serial_read();
void _serial_out(const char* str);
void serial_print(const char* str);

#endif
