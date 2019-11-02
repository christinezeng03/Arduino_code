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
public class ShowColor extends Command {
	boolean executed = false;
	boolean finished = false;
	byte _addr, _count, _r, _g, _b;
	LED _led;
	
	//Constructor: 
	//  @param led: the LED subsystem 
	//  @param addr: the start neopixel no (begins with 0 for the first pixel) 
	//  @param count: the number of pixels
	//  @param  r  : red (0-255) 
	//  @param  g  : green (0-255) 
	//  @param  b  : blue (0-255) 
	//  @param duration: in unit of second, how long the command will be executed 
    public ShowColor(LED led, byte addr, byte count, byte r, byte g, byte b, double duration) {
        requires(led);       
        _led = led;
        _addr = addr; 
        _count = count; 
        _r = r; 
        _g = g; 
        _b = b;       
        setTimeout(duration);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
		_led.show_color(_addr, _count, (byte)_r, (byte)_g, (byte)_b);
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
