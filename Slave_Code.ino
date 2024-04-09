#define     complete_transfer        *spsr & 0x80
#define     LCD_8_bit_mode            0x02
#define     display_on_cursor_off     0x0C
#define     auto_increament_cursor    0x06
#define     clear_display             0x01
#define     cursor_at_home_position   0x80
#define     command_enable_high       4
#define     data_enable_high          12
#define     command_upper_nibble      (cmnd & 0xf0)
#define     command_lower_nibble      (cmnd << 4)
#define     data_upper_nibble         (data & 0xf0)
#define     data_lower_nibble         (data << 4)
#define     enable_low                *outd &= ~4;



volatile char *dirb = 0x24;
volatile char *outb = 0x25;
volatile char *dird = 0x2A;
volatile char *outd = 0x2B;
volatile char *spcr = 0x4C;
volatile char *spdr = 0x4E;
volatile char *spsr = 0x4D;




//------------------------------------------------------------------------------------------------------//
//                                              SPI Functions                                           //
//------------------------------------------------------------------------------------------------------//

void SPI_SlaveInit(void){
*dirb  = 0x10;             // MISO set as Output & SCK, MOSI, SS as Input
*spcr |= 0x40;            // SPE = 1
}

char SPI_SlaveReceive(void){
while(!(complete_transfer));     // Wait until the Serial Transfer is completed
return *spdr;
}




//------------------------------------------------------------------------------------------------------//
//                                               LCD Functions                                          //
//------------------------------------------------------------------------------------------------------//

void LCD_Init (void){
LCD_Command (LCD_8_bit_mode);
LCD_Command (display_on_cursor_off);
LCD_Command (auto_increament_cursor);
LCD_Command (clear_display);
LCD_Command (cursor_at_home_position);
delayMicroseconds(2000);
}

void LCD_Command (volatile uint8_t cmnd){
*outd = command_upper_nibble + command_enable_high;      /* Send upper nibble */
delayMicroseconds(50);
enable_low;
delayMicroseconds(50);
*outd = command_lower_nibble + command_enable_high;     /* Send lower nibble */
delayMicroseconds(50);
enable_low;
delayMicroseconds(50);
}

void LCD_Data (volatile uint8_t data){
*outd = data_upper_nibble + data_enable_high;           /* Send upper nibble */
delayMicroseconds(50);
enable_low;
delayMicroseconds(50);
*outd = data_lower_nibble + data_enable_high;           /* Send lower nibble */
delayMicroseconds(50);
enable_low;
delayMicroseconds(50);
}




//------------------------------------------------------------------------------------------------------//
//                                              Main Function                                           //
//------------------------------------------------------------------------------------------------------//

void setup(){
 *dird = 0xff;                               // PORTD sets as output

 volatile uint8_t data;
 SPI_SlaveInit();
 LCD_Init();                                // Initialization of LCD
while(1){
 data = SPI_SlaveReceive();
 if(data=='c') LCD_Command(clear_display);
 else LCD_Data(data);                       // print the data on LCD
 }
}
