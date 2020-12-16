TARGET = ultrasonic

CC = avr-gcc
AVRDUDE = avrdude
OBJCOPY = avr-objcopy
PROGRAMMER = usbtiny

F_CPU = 1000000UL
MCU = atmega32
BAUD = 9600UL

CFLAGS = -Wall -g -Os -std=gnu99
CPPFLAGS = -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)
LDFLAGS = -Wl,-Map,$(TARGET).map
TARGET_ARCH = -mmcu=$(MCU)


$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $<

$(TARGET).elf: $(TARGET).o
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

.PHONY: all flash clean

all: $(TARGET).hex

flash: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -U flash:w:$<

clean:
	rm -f *.o *.elf *.hex *.map


