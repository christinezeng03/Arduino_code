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
* Command constructor  
* @param led Subsystem for LED module
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
*/	
public class ShowGradient extends Command {
	boolean executed = false;
	boolean finished = false;
	byte _addr, _count, _mode, _interval;
	LED _led;

    public ShowGradient(LED led, byte mode, byte interval, byte addr, byte count, double duration) {
        // Use requires() here to declare subsystem dependencies
        requires(led);
        _mode = mode; 
        _interval = interval;
        _addr = addr; 
        _count = count; 
        
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    	_led.show_gradient(_mode, _interval, _addr, _count);
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
    }
}
