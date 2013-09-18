#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define GPIO17 0
#define GPIO18 1
#define GPIO27 2

#define DI    0
#define LED   1
#define RESET 2

#define CMDDISPLOFF 0xAE
#define CMDDISPLON  0xAF
// startline address + line (0 to 63) 
#define CMDSETSTARTLINE 0x40
// +pagenumber (0 to 7)
#define CMDSETPAGEADR 0xB0
// + colnr_hi
#define CMDSETCOLADRHI 0x10
// + colnr_lo
#define CMDSETCOLADRLO 0x00
#define CMDADCNORMAL 0xA0
#define CMDADCREVERSE 0xA1
#define CMDDISPLAYNORMAL 0xA6
#define CMDDISPLAYREVERSE 0xA7
#define CMDALLPOINTSOFF 0xA4
#define CMDALLPOINTSON 0xA5
#define CMDSETLCDBIAS 0xA2
#define CMDRESET 0xE2
#define CMDCOMOUTMODE 0xC0
#define CMDSETPOWERCTRL 0x2F
// + ratio (0 to 7)
#define CMDSETRESISTORRATIO 0x20
#define CMDSETVOLMODE 0x81
// + val (0 to 63)
#define CMDSETVOLMODEVAL 0x00
#define CMDINDICATOROFF 0xAC
#define CMDINDICATORVAL 0x00
#define CMDSETBOOSTERRATIO 0xF8
// + val (0 to 3)
#define CMDSETBOOSTERRATIOVAL 0x00
#define CMDNOP 0xE3

void clear(int display[64][128])
{
  int y = 0;
  int x = 0;

  for (y=0;y<64;y++)
  {
    for (x=0;x<128;x++)
    {
      display[y][x] = 0;
    }
  }
}

void writeMode()
{
  digitalWrite(DI, 1);
}

void cmdMode()
{
  digitalWrite(DI, 0);
}

void setAdr(page, colhi, collo)
{
  char cmd;
  cmdMode();
  cmd = CMDSETPAGEADR + page;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETCOLADRHI + colhi;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETCOLADRLO + collo;
  wiringPiSPIDataRW(0, &cmd, 1);
}

void print(int display[64][128])
{

  int page, p, byte;
  int x = 0;

  char pixelstowrite[8];

  unsigned char line[8][128];

  int bit;

  for (page = 0; page < 8; page++)
  {
    for (x = 0;x < 128; x++)
    {
      byte = 0;
      for (bit = 0; bit < 8; bit++)
      {
        byte = byte + (display[page * 8 + bit][x] << bit);
      }
      line[page][x] = byte;
    }
  }

  for (page = 0;page<8;page++)
  {
    setAdr(page, 0, 0);
    writeMode();
    wiringPiSPIDataRW(0, line[page], 128);
  }
}

void init()
{
  char cmd;

  wiringPiSetup();

  pinMode(DI, OUTPUT);
  pinMode(LED, PWM_OUTPUT);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, 1);
  wiringPiSPISetup(0, 10000000);
  cmdMode();
  cmd = CMDSETSTARTLINE;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDADCREVERSE;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDCOMOUTMODE;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDDISPLAYNORMAL;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETLCDBIAS;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETPOWERCTRL;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETBOOSTERRATIO;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETBOOSTERRATIOVAL;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETRESISTORRATIO + 7;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETVOLMODE;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDSETVOLMODEVAL + 13;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDINDICATOROFF;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDINDICATORVAL;
  wiringPiSPIDataRW(0, &cmd, 1);
  cmd = CMDDISPLON;
  wiringPiSPIDataRW(0, &cmd, 1);
  
}

int main()
{
  int display[64][128];
//  char display[64][128];
 

  init();
  pwmWrite(LED, 150);
  clear(display);

  print(display);

  display[5][5] = 1;
  display[6][5] = 1;
  display[7][5] = 1;
  display[8][5] = 1;
  display[9][5] = 1;
  display[10][5] = 1;
  display[11][5] = 1;
  display[50][5] = 1;
  display[13][20] = 1;
  display[14][5] = 1;
  display[15][5] = 1;
  display[16][5] = 1;
  display[17][5] = 1;
  display[18][5] = 1;
  display[5][8] = 1;
  display[6][9] = 1;
  display[7][10] = 1;
  display[8][11] = 1;
  display[9][12] = 1;
  display[10][13] = 1;
  display[11][14] = 1;
  display[12][15] = 1;
  display[13][16] = 1;
  display[14][17] = 1;
  display[15][18] = 1;
  display[16][19] = 1;
  display[17][20] = 1;
  display[18][21] = 1;
  print(display);
  

  return 0;
}
