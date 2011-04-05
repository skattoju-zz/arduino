#AVR-GCC Makefile

PROJECT=robot
SOURCES=main.c motors.c sensors.c serial.c strategy.c util.c adc.c
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=atmega328p

CFLAGS= -mmcu=$(MMCU) -Wall -O
$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).out: $(SOURCES) 
	$(CC) $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)
#	$(CC) $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p m8 -c avrusb500 -e -U flash:w:$(PROJECT).hex:a

flash: $(PROJECT).hex
	avrdude -p m328p -c avrispmkII -P usb -U flash:w:$(PROJECT).hex:a

flash2: $(PROJECT).hex
	avrdude -p m328p -c avrisp -P /dev/ttyUSB0 -b 19200 -F -U flash:w:$(PROJECT).hex:a

flash3: $(PROJECT).hex
	avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:$(PROJECT).hex:a

rfuse: $(PROJECT).hex
	avrdude -c avrispmkII -p m328p -P usb -U hfuse:r:high.txt -U lfuse:r:low.txt

clean:
	rm -f $(PROJECT).o
	rm -f $(PROJECT).out
	rm -f $(PROJECT).map
	rm -f $(PROJECT).hex

serial:
	$(CC) $(CFLAGS) -I./ -o justserial.out justserial.c led.c
	$(OBJCOPY) -j .text -O ihex justserial.out justserial.hex
	avrdude -p m8 -c avrisp -P COM4 -b 19200 -U flash:w:justserial.hex:a

win: $(PROJECT).hex
	avrdude -p m8 -c avrisp -P COM4 -b 19200 -U flash:w:$(PROJECT).hex:a
