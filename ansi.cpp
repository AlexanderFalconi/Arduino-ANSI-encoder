#include "ansi.h"
#include <HardwareSerial.h>

ANSI::ANSI (HardwareSerial *serial) 
{ 
	_HardSerial = serial;
	//{27,91,0x30,0x6d,0}        /*  esc [0m  */
	s_init[0] = 27;
	s_init[1] = 91;
	s_init[2] = 0x30;
	s_init[3] = 0x6d;
	s_init[4] = 0;
	//{27,91,0x32,0x4a,0}        /*  esc[2J   */
	s_cls[0] = 27;
	s_cls[1] = 91;
	s_cls[2] = 0x32;
	s_cls[3] = 0x4a;
	s_cls[4] = 0;
	//{27,91,0x32,0x4b,0}        /*  esc[2K   */
	s_cln[0] = 27;
	s_cln[1] = 91;
	s_cln[2] = 0x32;
	s_cln[3] = 0x4b;
	s_cln[4] = 0;
	//{27,0x45,0}
	s_newl[0] = 27;                
	s_newl[1] = 0x45;                
	s_newl[2] = 0;                
	//{27,91,0x30,0x30,0x3b,0x30,0x30,0x48,0}                /* esc[00;00H */
	s_rc[0] = 27;
	s_rc[1] = 91;
	s_rc[2] = 0x30;
	s_rc[3] = 0x30;
	s_rc[4] = 0x3b;
	s_rc[5] = 0x30;
	s_rc[6] = 0x30;
	s_rc[7] = 0x48;
	s_rc[8] = 0;
	//{27,91,0x30,59,0x32,59,0x33,0x37,59,0x34,0x30,59,0x32,0x6d,0}  /*esc[0;2;37;40;2m    */
	s_atr[0] = 27;
	s_atr[1] = 91;
	s_atr[2] = 0x30;
	s_atr[3] = 59;
	s_atr[4] = 0x32;
	s_atr[5] = 59;
	s_atr[6] = 0x33;
	s_atr[7] = 0x37;
	s_atr[8] = 59;
	s_atr[9] = 0x34;
	s_atr[10] = 0x30;
	s_atr[11] = 59;
	s_atr[12] = 0x32;
	s_atr[13] = 0x6d;
	s_atr[14] = 0;
}

char *ANSI::PC_DispClrScr (char color)
{
    setatr(color);
    putstr(s_atr);
    putstr(s_cls);
	return buffer;
}

char *ANSI::PC_DispClrLine (char y, char color)
{
	y++;
	setrc(y,0);
	putstr(s_rc);
	setatr(color);
	putstr(s_atr);
	putstr(s_cln);
	return buffer;
}

char *ANSI::PC_GetDateTime (int day, int month, int year, int hour, int minute, int second, char *s)
{
	sprintf(s, "%02d-%02d-%02d  %02d:%02d:%02d", day, month, year, hour, minute, second);
	return s;
}	

void ANSI::truncate_str_if_too_long(char *s, char x)
{
  int i = 0;
  do
  {
		if (s[i] == 0) break;
		if (x > 78) s[i] = 0; /*truncate*/
		x++;
  } while (++i);
}

char *ANSI::PC_DispStr (char x, char y, char *s, char color)
{
	if(y > 22) /* If displaying string on lowest row, test for column length */
		truncate_str_if_too_long(s, x); /* Make sure we don't print on lower-right-most position */
	if(s[0] == 0)
		return s; /* whole string eliminated */
	y++;
	x++;
	setrc(y, x);
	putstr(s_rc);
	setatr(color);
	putstr(s_atr);
	putstr(s);
	return buffer;
}

void ANSI::putstr(char *s)
{
	int i = 0;
	do
	{
		if (s[i] == 0) 
			break;
		_HardSerial->print(s[i]);
	} while (++i);
}//***this is overwriting the buffer***use write instead of print, with the buffer option and buffer length...

void ANSI::setrc(char r, char c)
{
	s_rc[3] = ((r % 10) + 0x30);
	if ((r /= 10) > 0) s_rc[2] = ((r % 10) + 0x30);
		else s_rc[2] = 0x30;
	s_rc[6] = ((c % 10) + 0x30);
	if ((c /= 10) > 0) s_rc[5] = ((c % 10) + 0x30);
		else s_rc[5] = 0x30;
}
	
void ANSI::setatr(char color)
{
	char ctemp;
	ctemp = (color & 0x0f);
	if (ctemp < 8) s_atr[4] = 0x32;
		else {s_atr[4] = 0x31; ctemp = ctemp - 8;}
	s_atr[7] = (0x30 | ctemp);
	ctemp = ((color & 0x70) >> 4);
	s_atr[10] = (0x30 | ctemp);
	if (color & 0x80) s_atr[12] = 0x35;
		else s_atr[12] = s_atr[4];
}    /*esc[0;2;37;40;2m    */
