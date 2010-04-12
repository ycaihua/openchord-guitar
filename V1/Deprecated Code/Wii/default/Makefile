###############################################################################
# Makefile for the project OpenChord_V1_Wii
###############################################################################

## General Flags
PROJECT = OpenChord_V1_Wii
MCU = atmega328p
TARGET = OpenChord_V1_Wii.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99                                                         -DF_CPU=12000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=OpenChord_V1_Wii.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Documents and Settings\Administrator\Desktop\OpenChord\SVN\V1\Deprecated Code\Wii\." 

## Objects that must be built in order to link
OBJECTS = V1_Wii.o wiimote.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) OpenChord_V1_Wii.hex OpenChord_V1_Wii.eep OpenChord_V1_Wii.lss size

## Compile
V1_Wii.o: ../V1_Wii.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wiimote.o: ../wiimote.c
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
	-rm -rf $(OBJECTS) OpenChord_V1_Wii.elf dep/* OpenChord_V1_Wii.hex OpenChord_V1_Wii.eep OpenChord_V1_Wii.lss OpenChord_V1_Wii.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

