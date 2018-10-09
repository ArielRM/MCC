
CC = avr-gcc
CFLAGS = -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=16000000UL -iquoteinclude -iquote../avrlibs/lib
LDLIBS = 
LDFLAGS = -mmcu=atmega328p

SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=obj/%.o)
ELF := bin/$(NAME).elf
HEX := $(ELF:.elf=.hex)

h: hex
e: elf
o: obj
c: clean
hex: $(HEX)
elf: $(ELF)
obj: $(OBJ)

$(HEX): $(ELF)
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(ELF) $(HEX)

$(ELF): $(OBJ)
	if [ ! -d bin ]; then mkdir bin; fi
	$(CC) -o $(ELF) $(OBJ) $(LDFLAGS) $(CFLAGS) -MMD -MP

obj/%.o: src/%.c
	if [ ! -d obj ]; then mkdir obj; fi
	$(CC) -c $< -o $@ $(CFLAGS) -MMD -MP

.PHONY: clean
clean:
	@rm -rf obj bin

-include $(OBJ:%.o=%.d)