#ifdef TFTLCD_WRITE

#include <Arduino.h>
#include <SPI.h>

// Setup LCD pin
#define LCD_MOSI_PIN (23)
#define LCD_MISO_PIN (-1)
#define LCD_SCK_PIN  (18)
#define LCD_CS_PIN   (5)
#define LCD_DC_PIN   (12)
#define LCD_RST_PIN  (13)
#define LCD_BL_PIN   (14)

#define LCD_INACTIVE() digitalWrite(LCD_CS_PIN, HIGH)
#define LCD_ACTIVE() digitalWrite(LCD_CS_PIN, LOW)

void LCD_write_command(uint8_t data) {
  digitalWrite(LCD_DC_PIN, LOW);
  SPI.transfer(data);
}

void LCD_write_data(uint8_t data) {
  digitalWrite(LCD_DC_PIN, HIGH);
  SPI.transfer(data);
}

void LCD_command(uint8_t cmd) {
  LCD_ACTIVE();
  LCD_write_command(cmd);
  LCD_INACTIVE();
}

typedef enum {
    NOP = 0x00,
    SWRESET = 0x01,   /* Software Reset */
    RDDID = 0x04,     /* Read Display ID */
    RDDST = 0x09,     /* Read Display Status */
    RDDPM = 0x0a,     /* Read Display Power Mode */
    RDDMADCTL = 0x0b, /* Read Display MADCTL */
    RDDCOLMOD = 0x0c, /* Read Display Pixel Format */
    RDDIM = 0x0d,     /* Read Display Image Mode */
    RDDSM = 0x0e,     /* Read Display Signal Mode */
    RDDSDR = 0x0f,    /* Read Display Self-Diagnostic Result */
    SLPIN = 0x10,     /* Sleep In */
    SLPOUT = 0x11,    /* Sleep Out */
    PTLON = 0x12,     /* Partial Display Mode On */
    NORON = 0x13,     /* Normal Display Mode On */
    INVOFF = 0x20,    /* Display Inversion Off */
    INVON = 0x21,     /* Display Inversion On */
    GAMSET = 0x26,    /* Gamma Set */
    DISPOFF = 0x28,   /* Display Off */
    DISPON = 0x29,    /* Display On */
    CASET = 0x2a,     /* Column Address Set */
    RASET = 0x2b,     /* Row Address Set */
    RAMWR = 0x2c,     /* Memory Write */
    RGBSET = 0x2d,    /* Color Setting 4k, 65k, 262k */
    RAMRD = 0x2e,     /* Memory Read */
    PTLAR = 0x30,     /* Partial Area */
    SCRLAR = 0x33,    /* Scroll Area Set */
    TEOFF = 0x34,     /* Tearing Effect Line OFF */
    TEON = 0x35,      /* Tearing Effect Line ON */
    MADCTL = 0x36,    /* Memory Data Access Control */
    VSCSAD = 0x37,    /* Vertical Scroll Start Address of RAM */
    IDMOFF = 0x38,    /* Idle Mode Off */
    IDMON = 0x39,     /* Idle Mode On */
    COLMOD = 0x3a,    /* Interface Pixel Format */
    RDID1 = 0xda,     /* Read ID1 Value */
    RDID2 = 0xdb,     /* Read ID2 Value */
    RDID3 = 0xdc,     /* Read ID3 Value */
    FRMCTR1 = 0xb1,   /* Frame Rate Control in normal mode, full colors */
    FRMCTR2 = 0xb2,   /* Frame Rate Control in idle mode, 8 colors */
    FRMCTR3 = 0xb3,   /* Frame Rate Control in partial mode, full colors */
    INVCTR = 0xb4,    /* Display Inversion Control */
    PWCTR1 = 0xc0,    /* Power Control 1 */
    PWCTR2 = 0xc1,    /* Power Control 2 */
    PWCTR3 = 0xc2,    /* Power Control 3 in normal mode, full colors */
    PWCTR4 = 0xc3,    /* Power Control 4 in idle mode 8colors */
    PWCTR5 = 0xc4,    /* Power Control 5 in partial mode, full colors */
    VMCTR1 = 0xc5,    /* VCOM Control 1 */
    VMOFCTR = 0xc7,   /* VCOM Offset Control */
    WRID2 = 0xd1,     /* Write ID2 Value */
    WRID3 = 0xd2,     /* Write ID3 Value */
    NVFCTR1 = 0xd9,   /* NVM Control Status */
    NVFCTR2 = 0xde,   /* NVM Read Command */
    NVFCTR3 = 0xdf,   /* NVM Write Command */
    GMCTRP1 = 0xe0,   /* Gamma '+'Polarity Correction Characteristics Setting */
    GMCTRN1 = 0xe1,   /* Gamma '-'Polarity Correction Characteristics Setting */
    GCV = 0xfc,       /* Gate Pump Clock Frequency Variable */
} ST7735S_Command;

void setup() {
  Serial.begin(115200);
  
  SPI.begin(LCD_SCK_PIN, LCD_MISO_PIN, LCD_MOSI_PIN, -1);
  SPI.setFrequency(10E6); // 1 MHz
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(SPI_MSBFIRST);
  
  pinMode(LCD_RST_PIN, OUTPUT);
  pinMode(LCD_BL_PIN, OUTPUT);
  pinMode(LCD_DC_PIN, OUTPUT);
  pinMode(LCD_CS_PIN, OUTPUT);
  LCD_INACTIVE();

  // Reset LCD with set RST = 1 => 0 => 1
  digitalWrite(LCD_RST_PIN, HIGH);
  delay(5);
  digitalWrite(LCD_RST_PIN, LOW);
  delay(5);
  digitalWrite(LCD_RST_PIN, HIGH);
  delay(300); // Wait LCD ready

  // ST7735S init
  LCD_command(SWRESET);
  LCD_command(SLPOUT);
  LCD_command(DISPOFF);

  LCD_ACTIVE();
  LCD_write_command(FRMCTR1);
  LCD_write_data(0x00);
  LCD_write_data(0b111111);
  LCD_write_data(0b111111);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(FRMCTR2);
  LCD_write_data(0b1111);
  LCD_write_data(0x01);
  LCD_write_data(0x01);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(FRMCTR3);
  LCD_write_data(0x05);
  LCD_write_data(0x3c);
  LCD_write_data(0x3c);
  LCD_write_data(0x05);
  LCD_write_data(0x3c);
  LCD_write_data(0x3c);
  LCD_INACTIVE();
  
  LCD_ACTIVE();
  LCD_write_command(INVCTR);
  LCD_write_data(0x03);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(PWCTR1);
  LCD_write_data(0b11111100);
  LCD_write_data(0x08);
  LCD_write_data(0b10);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(PWCTR2);
  LCD_write_data(0xc0);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(PWCTR3);
  LCD_write_data(0x0d);
  LCD_write_data(0x00);
  LCD_INACTIVE();
  
  LCD_ACTIVE();
  LCD_write_command(PWCTR4);
  LCD_write_data(0x8d);
  LCD_write_data(0x2a);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(PWCTR5);
  LCD_write_data(0x8d);
  LCD_write_data(0xee);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(GCV);
  LCD_write_data(0b11011000);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(NVFCTR1);
  LCD_write_data(0b01000000);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(VMCTR1);
  LCD_write_data(0b001111);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(VMOFCTR);
  LCD_write_data(0b10000);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(GAMSET);
  LCD_write_data(0x08);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(MADCTL);
  LCD_write_data(0b01100000);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(COLMOD);
  LCD_write_data(0x05);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(GMCTRP1);
  LCD_write_data(0x02);
  LCD_write_data(0x1c);
  LCD_write_data(0x07);
  LCD_write_data(0x12);
  LCD_write_data(0x37);
  LCD_write_data(0x32);
  LCD_write_data(0x29);
  LCD_write_data(0x2c);
  LCD_write_data(0x29);
  LCD_write_data(0x25);
  LCD_write_data(0x2b);
  LCD_write_data(0x39);
  LCD_write_data(0x00);
  LCD_write_data(0x01);
  LCD_write_data(0x03);
  LCD_write_data(0x10);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(GMCTRN1);
  LCD_write_data(0x03);
  LCD_write_data(0x1d);
  LCD_write_data(0x07);
  LCD_write_data(0x06);
  LCD_write_data(0x2E);
  LCD_write_data(0x2C);
  LCD_write_data(0x29);
  LCD_write_data(0x2c);
  LCD_write_data(0x2e);
  LCD_write_data(0x2e);
  LCD_write_data(0x37);
  LCD_write_data(0x3f);
  LCD_write_data(0x00);
  LCD_write_data(0x00);
  LCD_write_data(0x02);
  LCD_write_data(0x10);
  LCD_INACTIVE();

  // LCD_command(INVON);
  LCD_command(IDMOFF);
  LCD_command(NORON);
  LCD_command(DISPON);

  digitalWrite(LCD_BL_PIN, HIGH);
}

void fill_screen(uint16_t color) {
  LCD_ACTIVE();
  LCD_write_command(CASET);
  LCD_write_data(0);
  LCD_write_data(0);
  LCD_write_data(0);
  LCD_write_data(128-1);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(RASET);
  LCD_write_data(0);
  LCD_write_data(0);
  LCD_write_data(0);
  LCD_write_data(128-1);
  LCD_INACTIVE();

  LCD_ACTIVE();
  LCD_write_command(RAMWR);
  digitalWrite(LCD_DC_PIN, HIGH);
  for (uint16_t i=0;i<(128 * 128);i++) {
    /*
    LCD_write_data(color >> 8);
    LCD_write_data(color & 0xFF);
    */
    SPI.transfer(color >> 8);
    SPI.transfer(color & 0xFF);
  }
  LCD_INACTIVE();
}

void loop() {
  fill_screen(0xF800); // Red
  delay(100);
  fill_screen(0x07E0); // Green
  delay(100);
  fill_screen(0x001F); // Blue
  delay(100);
}

#endif