// Neopixel device side as i2c client (slave)
// by S. Zeng for FRC 226 Arduino LED project 1-25-2018

#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

#include <Wire.h>

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      15

// device ID for i2c client 
#define I2C_DEVICE_ID  8

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void set_pixels_color(int num_pixels, int pix_indices[], uint32_t rgb)
{
  for(int i=0; i<num_pixels; i++) 
  {
    int ix = pix_indices[i];
    if(ix>=0 && ix<NUMPIXELS)
      pixels.setPixelColor(pix_indices[i], rgb); 
  }
}

void set_pixels_color(byte addr, byte count, uint32_t rgb)
{
  for(int i=addr; i<addr+count&&i<NUMPIXELS; i++) 
  {
      pixels.setPixelColor(i, rgb);
  }    
}

void set_pixels_color(uint32_t rgb)
{
  for(int i=0; i<NUMPIXELS; i++) 
  {
      pixels.setPixelColor(i, rgb);
  }  
}

class ColorGradient: public Thread
{
public:
  uint8_t curr_step;
  bool increment;
  byte mode; //0 - Red, 1 - Green, 2 - Blue, 3 - RG, 4 - GB, 5 - RB, 6 - RGB, 7 - R <-> B, 8 - G <-> B, 9 - R <-> B
  byte addr, count; 
  // No, "run" cannot be anything...
  // Because Thread uses the method "run" to run threads,
  // we MUST overload this method here. using anything other
  // than "run" will not work properly...
  void run(){
    switch(mode)
    { 
    case 0: set_pixels_color(addr, count, pixels.Color(curr_step,0,0)); break;
    case 1: set_pixels_color(addr, count, pixels.Color(0,curr_step,0)); break;
    case 2: set_pixels_color(addr, count, pixels.Color(0,0,curr_step)); break;
    case 3: set_pixels_color(addr, count, pixels.Color(curr_step,curr_step,0)); break;
    case 4: set_pixels_color(addr, count, pixels.Color(0,curr_step,curr_step)); break;
    case 5: set_pixels_color(addr, count, pixels.Color(curr_step,0,curr_step)); break;
    case 6: set_pixels_color(addr, count, pixels.Color(curr_step,curr_step,curr_step)); break;    
    case 7: set_pixels_color(addr, count, pixels.Color(curr_step,0xff-curr_step,0)); break;
    case 8: set_pixels_color(addr, count, pixels.Color(0,curr_step,0xff-curr_step)); break;
    case 9: set_pixels_color(addr, count, pixels.Color(curr_step,0,0xff-curr_step)); break;
    }
    pixels.show();
    if(curr_step<0xff && increment)
      curr_step++; 
    else if(curr_step==0xff && increment) 
      increment = false; 
    else if(curr_step>0 && !increment) 
      curr_step--; 
    else if(curr_step==0 && !increment)
      increment = true; 
        
    runned();   
  }

  ColorGradient() 
  {
    enabled = false;
    curr_step = 0;
    increment = true;
    mode = 0;
    addr = 0; 
    count = NUMPIXELS;
  }
};

class ColorBlinker: public Thread
{
public:
  uint32_t color;
  byte mode; //0 - off, 1 - on
  byte addr, count;

  // No, "run" cannot be anything...
  // Because Thread uses the method "run" to run threads,
  // we MUST overload this method here. using anything other
  // than "run" will not work properly...
  void run(){
    switch(mode)
    { 
    case 0: set_pixels_color(addr, count, pixels.Color(0,0,0)); break;
    case 1: set_pixels_color(addr, count, color); break;
    }
    pixels.show();
    mode = mode==0? 1 : 0;        
    runned();   
  }

  ColorBlinker() 
  {
    mode = 0;
    color = pixels.Color(10,10,10);
    enabled = false;
    addr = 0; 
    count = NUMPIXELS;
  }
};

class ColorShifter: public Thread
{
public:
  uint32_t color;
  int curr_start;
  byte count;
  bool increment;

  // No, "run" cannot be anything...
  // Because Thread uses the method "run" to run threads,
  // we MUST overload this method here. using anything other
  // than "run" will not work properly...
  void run(){
    if(curr_start==0 && increment) 
    {
      set_pixels_color(curr_start, count, color);
    }
    
    if(curr_start+count<NUMPIXELS && increment)
    {
      pixels.setPixelColor(curr_start, 0);
      pixels.setPixelColor(curr_start+count, color);      
      curr_start++; 
    }
    else if(curr_start+count==NUMPIXELS && increment) 
      increment = false; 
    else if(curr_start>=0 && !increment) 
    {
      pixels.setPixelColor(curr_start, color);
      pixels.setPixelColor(curr_start+count, 0);      
      curr_start--;
    } 
    else if(curr_start<0 && !increment)
    {
      increment = true;
      curr_start = 0;
    } 
    pixels.show();
    runned();   
  }

  ColorShifter() 
  {
    enabled = false;
    curr_start = 0;
    color = pixels.Color(10,10,10);
    count = 2;
    increment = true;
  }
};

bool serial_display = false;
bool command_parser_busy = false;

ColorGradient gradient;
ColorBlinker blinker;
ColorShifter shifter;
StaticThreadController<3> controller (&gradient,&blinker,&shifter);

void parse_master_command()
{
  if(serial_display)
  {
    while (1 < Wire.available()) { // loop through all but the last
      char c = Wire.read(); // receive byte as a character
      Serial.print((int)c);         // print the character
    }
    int x = Wire.read();    // receive byte as an integer
    Serial.println(x);         // print the integer
  }
  
  if(Wire.available()>0)
  {
    command_parser_busy = true; 
    byte c = Wire.read();
    byte address=0, r, g, b, count=0;
    switch(c) 
    {
      case 0: // 0, addr, count, mode, interval(millisecond)
        gradient.enabled = true;
        blinker.enabled = false;
        shifter.enabled = false;
        {
          address = Wire.read();  
          count = Wire.read();
          byte mode = Wire.read(); 
          byte interval = Wire.read(); 
          gradient.mode = mode; 
          gradient.addr = address; 
          gradient.count = count;
          gradient.setInterval(interval);
        }
        break;
      case 1: // 1, addr, r, g, b
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = false;
        address = Wire.read(); 
        r = Wire.read(); 
        g = Wire.read(); 
        b = Wire.read();
        pixels.setPixelColor(address, pixels.Color(r,g,b));
        break;
      case 2: // 2 
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = false;
        pixels.show();
        break;
      case 3: // 3
        serial_display = true; 
        break;
      case 4: // 4
        serial_display = false; 
        break;
      case 5: // 5, reset, black out all pixels 
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = false;
        set_pixels_color(pixels.Color(0,0,0));
        pixels.show();
        break;
      case 6: // 6, r, g, b, set color for the all pixels 
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = false;
        r = Wire.read(); 
        g = Wire.read(); 
        b = Wire.read(); 
        set_pixels_color(pixels.Color(r,g,b));
        pixels.show();
        break;
      case 7: // 7, addr, count, r, g, b 
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = false;
        address = Wire.read(); 
        count = Wire.read();
        r = Wire.read(); 
        g = Wire.read(); 
        b = Wire.read(); 
        for(int i=address; i<address+count; i++) 
        {
          pixels.setPixelColor(i, pixels.Color(r,g,b));
        }
        pixels.show();
        break;
      case 8: // 8, addr, count, r, g, b, interval(0.1s unit) : pixel blinker
        gradient.enabled = false;
        blinker.enabled = true;
        shifter.enabled = false;        
        address = Wire.read();
        count = Wire.read();
        r = Wire.read(); 
        g = Wire.read(); 
        b = Wire.read(); 
        blinker.setInterval(Wire.read()*100); // in 0.1 second       
        blinker.addr = address;
        blinker.count = count; 
        blinker.color = pixels.Color(r,g,b);
        break;
      case 9: // 9, count, r, g, b, interval(millisecond) : pixel shifter
        gradient.enabled = false;
        blinker.enabled = false;
        shifter.enabled = true;
        count = Wire.read();
        r = Wire.read(); 
        g = Wire.read(); 
        b = Wire.read(); 
        shifter.count = count; 
        shifter.setInterval(Wire.read());       
        shifter.color = pixels.Color(r,g,b);
        break;                
    }
    command_parser_busy = false; 
  }  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  //parse_master_command();
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(command_parser_busy? 1 : 0); // respond with the status command_parser
  // as expected by master
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  Wire.begin(I2C_DEVICE_ID);
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); 
  Serial.begin(9600);           // start serial for output  
  
  pixels.begin(); // This initializes the NeoPixel library.

  gradient.setInterval(10);
  blinker.setInterval(1000);
  shifter.setInterval(100);
}

void loop() {

  parse_master_command();   
  controller.run();
 
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
//  for(int i=0;i<255;i++)
//  {
//    set_pixels_color(pixels.Color(i,0,0));
//    pixels.show();
//    delay(20);
//  }

//  pixels.setPixelColor(0, pixels.Color(0,10,0));
//  pixels.show();
    
//  for(int i=0;i<NUMPIXELS;i++){
//
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(NUMPIXELS-i-1, pixels.Color(100,0,0)); // Moderately bright green color.
//    pixels.show(); // This sends the updated pixel color to the hardware.
//    delay(167); // Delay for a period of time (in milliseconds).
//  }
//
//  for(int i=NUMPIXELS - 1;i>=0;i--){
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
//    pixels.show(); // This sends the updated pixel color to the hardware.
//    delay(167); // Delay for a period of time (in milliseconds).
//  }
//
//  
//  for(int i=0;i<NUMPIXELS;i++){
//
//    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(NUMPIXELS-i-1, pixels.Color(0,0,100)); // Moderately bright green color.
//    pixels.show(); // This sends the updated pixel color to the hardware.
//    delay(167); // Delay for a period of time (in milliseconds).
//  }  
}
