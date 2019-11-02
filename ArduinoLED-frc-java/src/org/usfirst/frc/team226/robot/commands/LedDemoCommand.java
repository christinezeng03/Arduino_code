/**
*  A LED command group to demo LED capabilities  
* 
*  Change/Revision Logs: 
*   2018-03-01: Created by Christine Zeng (christinezeng03@gmail.com)
*
* @author Christine Zeng (christinezeng03@gmail.com), 2018-01-30
*/

package org.usfirst.frc.team226.robot.commands;

import org.usfirst.frc.team226.robot.Robot;
import org.usfirst.frc.team226.robot.RobotMap;

import edu.wpi.first.wpilibj.command.CommandGroup;

/**
 *
 */
public class LedDemoCommand extends CommandGroup {

    public LedDemoCommand() {
        // Add Commands here:
        // e.g. addSequential(new Command1());
        //      addSequential(new Command2());
        // these will run in order.

        // To run multiple commands at the same time,
        // use addParallel()
        // e.g. addParallel(new Command1());
        //      addSequential(new Command2());
        // Command1 and Command2 will run in parallel.

        // A command group will require all of the subsystems that each member
        // would require.
        // e.g. if Command1 requires chassis, and Command2 requires arm,
        // a CommandGroup containing them would require both the chassis and the
        // arm. 	
    	addSequential(new ShowColor(Robot.led1, (byte)0, RobotMap.NUM_NEOPIXELS, (byte)50, (byte)0, (byte)00, 3));
    	addSequential(new ShowBlinker(Robot.led1, (byte)0, RobotMap.NUM_NEOPIXELS, (byte)0, (byte)50, (byte)50, (byte)10, 10));
    	addSequential(new ShowGradient(Robot.led1, (byte)5, (byte)20, (byte)0, RobotMap.NUM_NEOPIXELS, 10));
    	addSequential(new ShowShifting(Robot.led1, (byte)8, (byte)50, (byte)50, (byte)50, (byte)20, 10));    	
    }
}
