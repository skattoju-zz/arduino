/* Serial Communications Interface */
#ifndef SERIAL_H
#define SERIAL_H

void serial_init(unsigned int BAUD_PRESCALE);
void serial_write(const char c);
char serial_read();
void serial_out(const char* str);

#endif
