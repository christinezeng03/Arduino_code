/**
*  ShowColor command
* 
*  Change/Revision Logs: 
*   2018-03-01: Created by Christine Zeng (christinezeng03@gmail.com)
*
* @author Christine Zeng (christinezeng03@gmail.com), 2018-01-30
*/

package org.usfirst.frc.team226.robot.commands;
import org.usfirst.frc.team226.robot.subsystems.LED;

import edu.wpi.first.wpilibj.command.Command;

/**
 *
 */
public class ShowShifting extends Command {
	boolean executed = false;
	boolean finished = false;
	byte _count, _r, _g, _b, _interval;
	LED _led;

	   /**
	   * Command constructor  
	   * @param addr The start Neopixel No involved in this command 
	   * @param count The number of pixels involved 
	   * @param r  Red (0-255)
	   * @param g  green (0-255)
	   * @param b  blue (0-255)	
	   * @param interval, the duration to show the color (r,g,b) in unit of 0.1 second     
	   * @param duration The maximum duration for the command, in unit of second  
	   */		
    public ShowShifting(LED led, byte count, byte r, byte g, byte b, byte interval, double duration) {
        // Use requires() here to declare subsystem dependencies
        requires(led);
        
        _led = led;
        _count = count; 
        _r = r; 
        _g = g; 
        _b = b;       
        _interval = interval;
        setTimeout(duration);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    	_led.show_shift(_count, (byte)_r, (byte)_g, (byte)_b, _interval);
		executed = true;
    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
    	if(executed && _led.check_idle())
    		finished = true;    	
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
       	if(finished && isTimedOut())
    		return true;    	
        return false;
    }

    // Called once after isFinished returns true
    protected void end() {
    	finished = false; 
    	executed = false;
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
    	end();
    }
}
