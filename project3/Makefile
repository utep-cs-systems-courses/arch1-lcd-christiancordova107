# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:project.elf

#additional rules for files
project.elf: ${COMMON_OBJECTS} main.o wdt_handler.o argument_for_update_shape1.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: project.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
