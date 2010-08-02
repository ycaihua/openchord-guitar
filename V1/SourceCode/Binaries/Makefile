###############################################################################
# Makefile for the project OpenChordV1
###############################################################################

## General Flags
PROJECT = OpenChordV1
MCU = atmega168
TARGET = V1-Vista7.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                                                           -DF_CPU=12000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=V1-Vista7.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = processStringState.o guitarInitFunctions.o main.o ps3Interface.o usbdrvasm.o usbdrv.o wiimote.o wiiInterface.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) V1-Vista7.hex V1-Vista7.eep V1-Vista7.lss size

## Compile
processStringState.o: ../processStringState.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

guitarInitFunctions.o: ../guitarInitFunctions.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ps3Interface.o: ../PS3/ps3Interface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usbdrvasm.o: ../PS3/usbdrv/usbdrvasm.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

usbdrv.o: ../PS3/usbdrv/usbdrv.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiimote.o: ../Wii/wiimote.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiiInterface.o: ../Wii/wiiInterface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) V1-Vista7.elf dep/* V1-Vista7.hex V1-Vista7.eep V1-Vista7.lss V1-Vista7.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

