#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main(void)
{
	char key = 0;

	clrscr();
	printf("Hello DOS!\n");

	/* loop until ESC */
	while(key != 0x1b)
	{
		if(kbhit()){
			key = getch();
			printf("Key pressed: %02x\n",key);
		}
	}

	return 0;
}