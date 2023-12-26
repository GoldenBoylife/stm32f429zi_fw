#include "spi.h"
#include "gpio.h"
#include "lcd/st7735.h"
#include "lcd/st7735_regs.h"


#define _PIN_DEF_BKT    1
#define _PIN_DEF_DC     3
#define _PIN_DEF_CS     2

#define MADCTL_MY       0x80
#define MADCTL_MX       0x40
#define MADCTL_MV       0x20
#define MADCTL_ML       0x10
#define MADCTL_RGB      0x00
#define MADCTL_BGR      0x08
#define MADCTL_MH       0x04



static uint8_t   spi_ch = _DEF_SPI0;

static const int32_t _width  = HW_LCD_WIDTH;
static const int32_t _height = HW_LCD_HEIGHT;
static void (*frameCallBack)(void) = NULL;
volatile static bool  is_write_frame = false;

const uint32_t colstart = 1;
const uint32_t rowstart = 26;

static void writecommand(uint8_t c);
static void writedata(uint8_t d);
static void st7735InitRegs(void);
static void st7735FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
static void st7735SetRotation(uint8_t m);
static bool st7735Reset(void);


/*DMA 다 전송되면 인터럽트에 의해서 이 함수 호출됨*/
static void TransferDoneISR(void)
{
  if (is_write_frame == true)
  {
    is_write_frame = false;
    gpioPinWrite(_PIN_DEF_CS, _DEF_HIGH);
    //칩 select를 high로 줌

    /*콜백함수가 있으면 호출함*/
    if (frameCallBack != NULL)
    {
      frameCallBack();
      //frame data가 완료된것을  상위에서도 알 수 있게 등록해서 씀.
    }
  }
}


bool st7735Init(void)
{
  bool ret;

  ret = st7735Reset();

  return ret;
}
/*lcd_driver_t는 함수포인터 묶음이었고,
InitDriver함수에서 자신이 가진 함수들과 연결한다. */
//공통적인 API인 lcd_driver_t를 만들고, 
//하드웨어가 달라지더라도 쓸수 있도록 이렇게 연결한다. 
//즉 driver 밑에 하드웨어 모듈driver다.
//lcd driver밑에 하드웨어 st7735 dirver
//나중에 lcd가 바뀌면, 여기만 바꿔주면 된다. 
bool st7735InitDriver(lcd_driver_t *p_driver)
{
    /*함수 포인터들에 함수들을 넣는 중*/
  p_driver->init = st7735Init;
  p_driver->reset = st7735Reset;
  p_driver->setWindow = st7735SetWindow;
  p_driver->getWidth = st7735GetWidth;
  p_driver->getHeight = st7735GetHeight;
  p_driver->setCallBack = st7735SetCallBack;
  p_driver->sendBuffer = st7735SendBuffer;
  return true;
}

bool st7735Reset(void)
{
  spiBegin(spi_ch);
  spiSetDataMode(spi_ch, SPI_MODE0);

  spiAttachTxInterrupt(spi_ch, TransferDoneISR);
  //DMA가 다 전송되면 인터럽트가 발생하고, TransferDoneISR이 작동됨

  // gpioPinWrite(_PIN_DEF_BKT, _DEF_LOW); //back light끔


  /*data 전송 완료 : DC는 H, ChipSelect는 H*/
  gpioPinWrite(_PIN_DEF_DC,  _DEF_HIGH);
  gpioPinWrite(_PIN_DEF_CS,  _DEF_HIGH);
  delay(10);

  st7735InitRegs(); 
  //register write함


  st7735SetRotation(3); //LCD방향 
//   st7735FillRect(0, 0, HW_LCD_WIDTH, HW_LCD_HEIGHT, black); //lcd를 black으로 채움
  st7735FillRect(0, 0, HW_LCD_WIDTH, HW_LCD_HEIGHT, red);
  gpioPinWrite(_PIN_DEF_BKT, _DEF_LOW); //LCD back light 다시 끔
//   gpioPinWrite(_PIN_DEF_BKT, _DEF_HIGH); //LCD back light 다시 끔
  return true;
}

uint16_t st7735GetWidth(void)
{
  return HW_LCD_WIDTH;
}

uint16_t st7735GetHeight(void)
{
  return HW_LCD_HEIGHT;
}

void writecommand(uint8_t c)
{
  gpioPinWrite(_PIN_DEF_DC, _DEF_LOW);
  gpioPinWrite(_PIN_DEF_CS, _DEF_LOW);

  spiTransfer8(spi_ch, c);

  gpioPinWrite(_PIN_DEF_CS, _DEF_HIGH);
}

void writedata(uint8_t d)
{
  gpioPinWrite(_PIN_DEF_DC, _DEF_HIGH);
  gpioPinWrite(_PIN_DEF_CS, _DEF_LOW);

  spiTransfer8(spi_ch, d);
  //spi로 1byte를 보냄

  gpioPinWrite(_PIN_DEF_CS, _DEF_HIGH);
}

/*아두이노쪽 코드 참고함*/
void st7735InitRegs(void)
{
  writecommand(ST7735_SWRESET); //  1: Software reset, 0 args, w/delay
  delay(10);

  writecommand(ST7735_SLPOUT);  //  2: Out of sleep mode, 0 args, w/delay
  delay(10);

  writecommand(ST7735_FRMCTR1); //  3: Frame rate ctrl - normal mode, 3 args:
  writedata(0x01);              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  writedata(0x2C);
  writedata(0x2D);

  writecommand(ST7735_FRMCTR2); //  4: Frame rate control - idle mode, 3 args:
  writedata(0x01);              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
  writedata(0x2C);
  writedata(0x2D);

  writecommand(ST7735_FRMCTR3); //  5: Frame rate ctrl - partial mode, 6 args:
  writedata(0x01);              //     Dot inversion mode
  writedata(0x2C);
  writedata(0x2D);
  writedata(0x01);              //     Line inversion mode
  writedata(0x2C);
  writedata(0x2D);

  writecommand(ST7735_INVCTR);  //  6: Display inversion ctrl, 1 arg, no delay:
  writedata(0x07);              //     No inversion

  writecommand(ST7735_PWCTR1);  //  7: Power control, 3 args, no delay:
  writedata(0xA2);
  writedata(0x02);              //     -4.6V
  writedata(0x84);              //     AUTO mode

  writecommand(ST7735_PWCTR2);  //  8: Power control, 1 arg, no delay:
  writedata(0xC5);              //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD

  writecommand(ST7735_PWCTR3);  //  9: Power control, 2 args, no delay:
  writedata(0x0A);              //     Opamp current small
  writedata(0x00);              //     Boost frequency

  writecommand(ST7735_PWCTR4);  // 10: Power control, 2 args, no delay:
  writedata(0x8A);              //     BCLK/2, Opamp current small & Medium low
  writedata(0x2A);

  writecommand(ST7735_PWCTR5);  // 11: Power control, 2 args, no delay:
  writedata(0x8A);
  writedata(0xEE);

  writecommand(ST7735_VMCTR1);  // 12: Power control, 1 arg, no delay:
  writedata(0x0E);

  writecommand(ST7735_INVON);   // 13: Don't invert display, no args, no delay

  writecommand(ST7735_MADCTL);  // 14: Memory access control (directions), 1 arg:
  writedata(0xC8);              //     row addr/col addr, bottom to top refresh

  writecommand(ST7735_COLMOD);  // 15: set color mode, 1 arg, no delay:
  writedata(0x05);              //     16-bit color


  writecommand(ST7735_CASET);   //  1: Column addr set, 4 args, no delay:
  writedata(0x00);
  writedata(0x00);              //     XSTART = 0
  writedata(0x00);
  writedata(160-1);             //     XEND = 159

  writecommand(ST7735_RASET);   //  2: Row addr set, 4 args, no delay:
  writedata(0x00);
  writedata(0x00);              //     XSTART = 0
  writedata(0x00);
  writedata(80-1);              //     XEND = 79


  writecommand(ST7735_NORON);   //  3: Normal display on, no args, w/delay
  delay(10);
  writecommand(ST7735_DISPON);  //  4: Main screen turn on, no args w/delay
  delay(10);
}

void st7735SetRotation(uint8_t mode)
{
  writecommand(ST7735_MADCTL);

  switch (mode)
  {
   case 0:
     writedata(MADCTL_MX | MADCTL_MY | MADCTL_BGR);
     break;

   case 1:
     writedata(MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     break;

  case 2:
    writedata(MADCTL_BGR);
    break;

   case 3:
     writedata(MADCTL_MX | MADCTL_MV | MADCTL_BGR);
     break;
  }
}

void st7735SetWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  spiSetBitWidth(spi_ch, 8);

  writecommand(ST7735_CASET); // Column addr set
  writedata(0x00);
  writedata(x0+colstart);     // XSTART
  writedata(0x00);
  writedata(x1+colstart);     // XEND

  writecommand(ST7735_RASET); // Row addr set
  writedata(0x00);
  writedata(y0+rowstart);     // YSTART
  writedata(0x00);
  writedata(y1+rowstart);     // YEND

  writecommand(ST7735_RAMWR); // write to RAM
}

void st7735FillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
  uint16_t line_buf[w];
  
  // Clipping
  if ((x >= _width) || (y >= _height)) return;

  if (x < 0) { w += x; x = 0; }
  if (y < 0) { h += y; y = 0; }

  if ((x + w) > _width)  w = _width  - x;
  if ((y + h) > _height) h = _height - y;

  if ((w < 1) || (h < 1)) return;


  st7735SetWindow(x, y, x + w - 1, y + h - 1);
  spiSetBitWidth(spi_ch, 16);

  gpioPinWrite(_PIN_DEF_DC, _DEF_HIGH);
  gpioPinWrite(_PIN_DEF_CS, _DEF_LOW);

  for (int i=0; i<w; i++)
  {
    line_buf[i] = color;
  }
  for (int i=0; i<h; i++)
  {
    spiDmaTxTransfer(_DEF_SPI0, (void *)line_buf, w, 10);
  }
  
  gpioPinWrite(_PIN_DEF_CS, _DEF_HIGH);
  cliPrintf("여긴\r\n");
}

bool st7735SendBuffer(uint8_t *p_data, uint32_t length, uint32_t timeout_ms)
{
  is_write_frame = true;

  spiSetBitWidth(spi_ch, 16);
  //DMA를 16비트 단위로보내기 위해서 바꿈.

  /*data 보낼 준비 완료 : DC는 H, ChipSelect는 L*/
  gpioPinWrite(_PIN_DEF_DC, _DEF_HIGH);
  gpioPinWrite(_PIN_DEF_CS, _DEF_LOW);
  

  spiDmaTxTransfer(_DEF_SPI0, (void *)p_data, length, 0);
  //DMA로 data 보냄(write)

  return true;
}

bool st7735SetCallBack(void (*p_func)(void))
{
  frameCallBack = p_func;

  return true;
}
