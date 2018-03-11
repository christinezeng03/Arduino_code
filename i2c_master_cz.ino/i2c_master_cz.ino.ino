// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

#define NUMPIXELS      60


void check_completion_command() 
{
  bool finish = false;  
  while(!finish)
  {
    Wire.requestFrom(8,1,true);
    while(Wire.available())
    {
      byte busy = Wire.read(); 
      finish = !busy;
      //Serial.println(busy);         // print the integer
    }
  }
}

void write_pixel(byte no, byte r, byte g, byte b)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(1);        // send reset command
  Wire.write(no);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();    // stop transmitting

  check_completion_command();  
}

void clear_pixels() 
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(5);        // send reset command
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();  
}

void show_pixels() 
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(2);        // send show command
  Wire.endTransmission();    // stop transmitting  

  check_completion_command();  
}

void show_gradient(byte mode, byte interval, byte addr=0, byte count=NUMPIXELS)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(0);        // send gradient command
  Wire.write(addr); 
  Wire.write(count);
  Wire.write(mode); 
  Wire.write(interval);
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();  
}

void show_blink(byte r, byte g, byte b, byte interval, byte addr=0, byte count=NUMPIXELS)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(8);        // send blink command
  Wire.write(addr); 
  Wire.write(count);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b); 
  Wire.write(interval);
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();    
}

void show_shift(byte count, byte r, byte g, byte b, byte interval)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(9);        // send blink command
  Wire.write(count);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b); 
  Wire.write(interval);
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();    
}


void show_color(byte r, byte g, byte b)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(6);        
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();  
}

void show_color(byte addr, byte count, byte r, byte g, byte b)
{
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(7);        
  Wire.write(addr);
  Wire.write(count);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();    // stop transmitting    

  check_completion_command();    
}

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);           // start serial for output  

  clear_pixels();
}

void loop() {//rgb,interval,address,count
  
//  show_color(0, 10, 100,0,0);
//  show_color(10, 10, 0,100,0);
//  show_color(20, 10, 0,0,100);
//  show_color(30, 10, 100,0,0);
//  show_color(40,10,0,100,0);
//  show_color(50,10,0,0,100);
//  delay(1000);
//  show_color(0,60,0,0,0);
//  delay(1000);
  
 //show_gradient(9, 5, 2, 58);
 //show_blink(0,15,20, 5, 10, 20);
 //show_shift(1, 10,0,11, 50);
 //delay(1000);

//  for(int i=0;i<NUMPIXELS;i++){
//    write_pixel(NUMPIXELS-i-1, 100,0,0); 
//    show_pixels();
//    delay(167);
//  }
//
//   for(int i=NUMPIXELS - 1;i>=0;i--){
//    write_pixel(i, 0,0,0); 
//    show_pixels(); 
//    delay(167); // Delay for a period of time (in milliseconds).
//  }
// 
//  for(int i=0;i<NUMPIXELS;i++){
//    write_pixel(NUMPIXELS-i-1, 0,0,100); 
//    show_pixels();
//    delay(167);
//  }
//
//  for(int i=NUMPIXELS - 1;i>=0;i--){
//    write_pixel(i, 0,0,0); 
//    show_pixels(); 
//    delay(167); // Delay for a period of time (in milliseconds).
//  }


 for(int i=0;i<60;i=i+3){
    write_pixel(i,50,0,0); 
    }
 for(int i=1;i<60;i=i+3){
    write_pixel(i,0,50,0); 
    }
 for(int i=2;i<60;i=i+3){
    write_pixel(i,0,0,50); 
    }  
 show_pixels();
 delay(1000);
 show_color(0,60,0,0,0);
 delay(1000);     
}
