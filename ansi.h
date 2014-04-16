#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <conio.h>
#include    <HardwareSerial.h>

class ANSI {
public:
    ANSI (HardwareSerial *serial);//constructor
    char *get_buffer()       { return buffer; }
	char *PC_DispClrScr(char bgnd_color);
	char *PC_DispClrLine(char y, char bgnd_color);
	char *PC_DispStr(char x, char y, char *s, char color);
	void setrc(char r, char c);
	void setatr(char color);
	void putstr(char *s);
	void truncate_str_if_too_long(char *s, char x);
	char *PC_GetDateTime (int day, int month, int year, int hour, int minute, int second, char *s);
	HardwareSerial * _HardSerial; 

protected:
    char buffer[80];
	char s_init[5];
	char s_cls[5];
	char s_cln[5];
	char s_newl[3];
	char s_rc[8];
	char s_atr[15];
};

/*
*********************************************************************************************************
*                                               CONSTANTS
*                                    COLOR ATTRIBUTES FOR VGA MONITOR
*
* Description: These #defines are used in the PC_Disp???() functions.  The 'color' argument in these
*              function MUST specify a 'foreground' color, a 'background' and whether the display will
*              blink or not.  If you don't specify a background color, BLACK is assumed.  You would
*              specify a color combination as follows:
*
*              PC_DispChar(0, 0, 'A', DISP_FGND_WHITE + DISP_BGND_BLUE + DISP_BLINK);
*
*              To have the ASCII character 'A' blink with a white letter on a blue background.
*********************************************************************************************************
*/
#define DISP_FGND_BLACK           0x00
#define DISP_FGND_RED             0x01
#define DISP_FGND_GREEN           0x02
#define DISP_FGND_BROWN           0x03
#define DISP_FGND_BLUE            0x04
#define DISP_FGND_PURPLE          0x05
#define DISP_FGND_CYAN            0x06
#define DISP_FGND_LIGHT_GRAY      0x07
#define DISP_FGND_DARK_GRAY       0x08
#define DISP_FGND_LIGHT_RED       0x09
#define DISP_FGND_LIGHT_GREEN     0x0A
#define DISP_FGND_YELLOW          0x0B
#define DISP_FGND_LIGHT_BLUE      0x0C
#define DISP_FGND_LIGHT_PURPLE    0x0D
#define DISP_FGND_LIGHT_CYAN      0x0E
#define DISP_FGND_WHITE           0x0F

#define DISP_BGND_BLACK           0x00
#define DISP_BGND_RED             0x10
#define DISP_BGND_GREEN           0x20
#define DISP_BGND_BROWN           0x30
#define DISP_BGND_BLUE            0x40
#define DISP_BGND_PURPLE          0x50
#define DISP_BGND_CYAN            0x60
#define DISP_BGND_LIGHT_GRAY      0x70

#define DISP_BLINK                0x80

#define  VECT_TICK                    0x08       /* Vector number for 82C54 timer tick                 */
#define  VECT_DOS_CHAIN               0x81       /* Vector number used to chain DOS                    */
#define  INTERVAL							  90         /* 5 second interval											 */
#define  OS_ENTER_CRITICAL  asm {PUSHF; CLI}            /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL   asm POPF                   /* Enable  interrupts                        */

#define  DISP_BASE                  0xB800       /* Base segment of display (0xB800=VGA, 0xB000=Mono)  */
#define  DISP_MAX_X                     80       /* Maximum number of columns                          */
#define  DISP_MAX_Y                     25       /* Maximum number of rows                             */
