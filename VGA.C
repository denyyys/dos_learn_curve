#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h> /* default console library */
#include <string.h>

/* video interupts codes */
#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03

typedef unsigned char byte;

void set_mode(byte mode)
{
 union REGS regs; /* manipulate with registers */
 regs.h.ah = SET_MODE;
 regs.h.al = mode;
 int86( VIDEO_INT, &regs, &regs );
}

int main(void)
{
	set_mode( VGA_256_COLOR_MODE );

	clrscr();
	printf("Hello DOS!\n");
	system("pause");

	set_mode(TEXT_MODE);
	return 0;
}