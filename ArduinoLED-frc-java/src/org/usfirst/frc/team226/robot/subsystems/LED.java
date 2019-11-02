/**
*  LED subsystem implementation 
* 
*  Change/Revision Logs: 
*   2018-03-01: Created by Christine Zeng (christinezeng03@gmail.com)
*
* @author Christine Zeng (christinezeng03@gmail.com), 2018-01-30
*/


package org.usfirst.frc.team226.robot.subsystems;
import org.usfirst.frc.team226.robot.Robot;
import org.usfirst.frc.team226.robot.RobotMap;
import org.usfirst.frc.team226.robot.commands.ShowBlinker;
import org.usfirst.frc.team226.robot.commands.ShowColor;
import org.usfirst.frc.team226.robot.commands.ShowGradient;
import org.usfirst.frc.team226.robot.commands.ShowShifting;

import edu.wpi.first.wpilibj.I2C;

import edu.wpi.first.wpilibj.command.Subsystem;
/**
 *
 */
public class LED extends Subsystem {
	I2C i2c;
	byte nDeviceId; 
	
	public LED(byte led_devId) {
		 nDeviceId = led_devId;
		 i2c = new I2C(I2C.Port.kOnboard, nDeviceId);
		 
		 reset_pixels();
	}	
	
	/*
	 *   Check if the LED module is idle and ready to receive command from the master 
	 *   @return boolean, true if the LED module is idle 
	 */
	public boolean check_idle()  // return true if the LED module is in idle state
	{
		byte data[] = new byte[1];
		i2c.readOnly(data, 1);
		boolean busy = (data[0]!=0); 
		boolean status = !busy;
		return status; // true: idle, false: busy
	}
	
	/*
	 *   poll LED module until it becomes idle and is ready to receive new command from master 
	 *   @return None
	 */
	public void wait_for_command_completion() // wait (looping) until the LED is in idle state
	{
		boolean idle = false; 
		while(!idle)
		{
			idle = check_idle();
		}	
	}
	
   /**
   * This method is used to set a neopixel no to a RGB value
   * @param no Neopixel number, the first pixel starts with no 0 
   * @param r  Red (0-255)
   * @param g  green (0-255)
   * @param b  blue (0-255)
   * @return None
   */	
	public void write_pixel(byte no, byte r, byte g, byte b)
	// set the color (r,g,b) to the pixel with address #no
	{
		byte data[] = new byte[5]; 
		data[0] = 1; 
		data[1] = no; 
		data[2] = r; 
		data[3] = g;
		data[4] = b; 		
		i2c.writeBulk(data);
	}
	
	   /**
	   * This method is used to show the pixels preset by the method write_pixel
	   * @return None
	   */	
	public void show_pixels()
	// trigger signal to show all pixels to their set colors 
	{
		byte data[] = new byte[1];
		data[0] = 2; 
		i2c.writeBulk(data);
	}
	
	   /**
	   * This method is used to show the animation gradually transit from a color to another 
	   * @param mode 0 - Red to black 
	   * 			 1 - Green to black 
	   * 			 2 - Blue to black 
	   *             3 - Yellow to black 
	   *             4 - Purple to black 
	   *             5 - Cyan to black 
	   *             6 - White to black
	   *             7 - Red to green 
	   *             8 - Green to blue 
	   *             9 - Blue to red
	   *             
	   * @param interval The cycle time for a animatin step, in unit of millisecond 
	   * @param addr The start Neopixel number involved in this command 
	   * @param count The number of pixels involved 
	   * @return None
	   */	
	public void show_gradient(byte mode, byte interval, byte addr, byte count)
	// show color gradient animation for the #count pixels starting from the address #addr
	// the cycle time for each animation step is #interval milliseconds
	{
		byte data[] = new byte[5]; 
		data[0] = 0; 
		data[1] = addr; 
		data[2] = count; 
		data[3] = mode;
		data[4] = interval; 		
		i2c.writeBulk(data);
	}

	   /**
	   * This method is used to show blinking animation  
	   * @param addr The start Neopixel number involved in this command 
	   * @param count The number of pixels involved 
	   * @param r  Red (0-255)
	   * @param g  green (0-255)
	   * @param b  blue (0-255)	
	   * @param interval, the duration to show the color (r,g,b) in unit of 0.1 second     
	   * @return None
	   */		
	public void show_blink(byte addr, byte count, byte r, byte g, byte b, byte interval)
	// show #count pixels from address #addr to the color (r,g,b) with #interval time units (0.1 second)
	// then black out all pixels for #interval time units
	{
		byte data[] = new byte[7]; 
		data[0] = 8; 
		data[1] = addr; 
		data[2] = count; 
		data[3] = r;
		data[4] = g;
		data[5] = b; 
		data[6] = interval;
		i2c.writeBulk(data);
	}
	
	   /**
	   * This method is used to show lit pixels in color (r,g,b) shift left and backwards   
	   * @param count The number of pixels to be lit 
	   * @param r  Red (0-255)
	   * @param g  green (0-255)
	   * @param b  blue (0-255)	
	   * @param interval, the duration for each animation step, in unit millisecond    
	   * @return None
	   */		
	public void show_shift(byte count, byte r, byte g, byte b, byte interval)
	// show #count pixels to the color (r,g,b) and shift forward and backward, with interval time (ms) for each animation step 
	{
		byte data[] = new byte[6]; 
		data[0] = 9; 
		data[1] = count; 
		data[2] = r;
		data[3] = g;
		data[4] = b; 
		data[5] = interval;
		i2c.writeBulk(data);
	}
	
	   /**
	   * This method is used to light all pixels in color (r,g,b)   
	   * @param r  Red (0-255)
	   * @param g  green (0-255)
	   * @param b  blue (0-255)	
	   * @return None
	   */			
	public void show_color(byte r, byte g, byte b)
	// show color (r,g,b) to all pixels 
	{
		byte data[] = new byte[4]; 
		data[0] = 6; 
		data[1] = r;
		data[2] = g;
		data[3] = b; 
		i2c.writeBulk(data);
	}

	   /**
	   * This method is used to light the chosen 'count' pixels from pixel no 'addr' in color (r,g,b)   
	   * @param addr The start Neopixel number involved in this command 
	   * @param count The number of pixels involved 
	   * @param r  Red (0-255)
	   * @param g  green (0-255)
	   * @param b  blue (0-255)	
	   * @return None
	   */			
	public void show_color(byte addr, byte count, byte r, byte g, byte b) 
	// show color (r,g,b) to #count pixels to from address #addr 
	{
		byte data[] = new byte[6]; 
		data[0] = 7; 
		data[1] = addr;
		data[2] = count;
		data[3] = r;
		data[4] = g;
		data[5] = b; 
		i2c.writeBulk(data);
	}
		
	   /**
	   * This method is to turn off all pixels    
	   * @return None
	   */			
	public void reset_pixels() // clear out all pixels to be black
	{
		byte data[] = new byte[1];
		data[0] = 5; 
		i2c.writeBulk(data);
	}

    public void initDefaultCommand() {
        // Set the default command for a subsystem here.
        //setDefaultCommand(new ShowColor(this, (byte)0, RobotMap.NUM_NEOPIXELS, (byte)0, (byte)0, (byte)10, 1.0)); 
    	//setDefaultCommand(new ShowGradient(Robot.led1, (byte)5, (byte)20, (byte)0, RobotMap.NUM_NEOPIXELS, 10));
    	//setDefaultCommand(new ShowBlinker(Robot.led1, (byte)0, RobotMap.NUM_NEOPIXELS, (byte)0, (byte)50, (byte)50, (byte)10, 10));
    	setDefaultCommand(new ShowShifting(Robot.led1, (byte)8, (byte)50, (byte)50, (byte)50, (byte)20, 5));
    }
}