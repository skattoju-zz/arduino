# AVR-GCC Makefile
PROJECT=tech2
SOURCES=tech2.c led.c serial.c 
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=atmega8

CFLAGS=-mmcu=$(MMCU) -Wall -O


$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex


$(PROJECT).out: $(SOURCES) 
	$(CC) $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)
#	$(CC) $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p m8 -c avrusb500 -e -U flash:w:$(PROJECT).hex
clean:
	rm -f $(PROJECT).o
	rm -f $(PROJECT).out
	rm -f $(PROJECT).map
	rm -f $(PROJECT).hex
