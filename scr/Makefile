CC = arm-none-eabi-gcc
CFLAG = -mcpu=cortex-m4 -mthumb

all: startup.o main.o

main.o: main.c
	arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb main.c -o main.o

startup.o: startup.c
	arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb startup.c -o startup.o

clean:
	rm -rf *.o