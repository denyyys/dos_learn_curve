#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <string.h>

#define NUM_COLORS 256

/*interrupt kody */
#define SET_MODE 0x00
#define VIDEO_INT 0x10
#define VGA_256_COLOR_MODE 0x13
#define TEXT_MODE 0x03

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320

#define PALETTE_INDEX 0x3C8
#define PALETTE_DATA 0x3C9

typedef unsigned char byte;

byte far *VGA=(byte far *)0xA0000000L;

#define SETPIX(x,y,c) *(VGA+(x)+(y)*SCREEN_WIDTH)=c
#define GETPIX(x,y) *(VGA+(x)+(y)*SCREEN_WIDTH)
#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))

void set_mode(byte mode)
{
  union REGS regs;
  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int86( VIDEO_INT, &regs, &regs );
}

void draw_background()
{
  int x, y;

  for( y = 0; y < SCREEN_HEIGHT; ++y ) {
    for( x = 0; x < SCREEN_WIDTH; ++x ) {
      SETPIX( x, y, y );
    }
  }
}

byte *get_sky_palette()
{
  byte *pal;
  int i;
  pal = malloc( NUM_COLORS * 3 ); /* RGB */

  for( i = 0; i < 100; ++i ) {
    pal[ i*3 + 0 ] = MIN( 63, i ); /* RED */
    pal[ i*3 + 1 ] = MIN( 63, i ); /* GREEN */
    pal[ i*3 + 2 ] = 63; /* BLUE */
  }
  for( i = 100; i < 200; ++i ) {
    pal[ i*3 + 0 ] = 5; /* RED */
    pal[ i*3 + 1 ] = (i - 100) / 2; /* GREEN */
    pal[ i*3 + 2 ] = 5; /* BLUE */
  }

  return pal;
}

void set_palette(byte *palette)
{
  int i;

  outp( PALETTE_INDEX, 0 );
  for( i = 0; i < NUM_COLORS * 3; ++i ) {
    outp( PALETTE_DATA, palette[ i ] );
  }
}

int main()
{
  char key = 0;
  char s[255];
  byte *pal;

  set_mode( VGA_256_COLOR_MODE );

  pal = get_sky_palette();
  set_palette( pal );

  clrscr();
  /* printf("Hello DOS!\n"); */
  draw_background();

  /* loop until ESC pressed */
  while( key != 0x1b )
  {
    if(kbhit()) {
      key = getch();
      if( key == (char)0 ) {
	key = getch();
	/* special key handling */
	switch( key )
	{
	case 0x48: /* up arrow */
	  strcpy(s, "PTK");
	  break;
	case 0x50: /* down arrow */
	  strcpy(s, "SMACK ONE");
	  break;
	default: /* other special keys */
	  sprintf(s, "00 %02x", key);
	  break;
	}
      } else {
	sprintf(s, "%02x", key);
      }
      printf("Key pressed: %s\n", s);
    }
  }

  set_mode( TEXT_MODE );

  return 0;
}

