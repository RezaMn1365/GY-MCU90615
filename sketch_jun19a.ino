#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float TO=0,TA=0;
void setup() {
   Serial.begin(9600);
  delay(1);
  Serial.write(0XA5);
  Serial.write(0X45);
  Serial.write(0XEA);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  unsigned char i=0,sum=0;

  if(sign)
  {
     sign=0;
     for(i=0;i<8;i++)
      sum+=Re_buf[i];
     if(sum==Re_buf[i] )
     {
           TO=(float)(Re_buf[4]<<8|Re_buf[5])/100;
           Serial.print("TO:");
           Serial.println(TO);
           TA=(float)(Re_buf[6]<<8|Re_buf[7])/100;
           Serial.print("TA:");
           Serial.println(TA);
   }

    display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println("Temp:");
   display.display();
  display.println(TO);
   display.display();
  }

}
void serialEvent() {
  while (Serial.available()) {
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0x5A) return;
    counter++;
    if(counter==9)
    {
       counter=0;
       sign=1;
    }
  }
}
