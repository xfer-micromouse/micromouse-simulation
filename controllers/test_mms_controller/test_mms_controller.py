"""test_mms_controller controller."""

# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor
from controller import Robot

# create the Robot instance.
robot = Robot()

# get the time step of the current world.
timestep = int(robot.getBasicTimeStep())

# You should insert a getDevice-like function in order to get the
# instance of a device of the robot. Something like:
#  motor = robot.getDevice('motorname')
#  ds = robot.getDevice('dsname')
#  ds.enable(timestep)

l_motor = robot.getDevice('l_motor')
r_motor = robot.getDevice('r_motor')
df = robot.getDevice('ds_front')


l_motor.setPosition(float('inf'))
r_motor.setPosition(float('inf'))

l_motor.setVelocity(0)
r_motor.setVelocity(0)

df.enable(100);

# Main loop:
# - perform simulation steps until Webots is stopping the controller
while robot.step(timestep) != -1:
    # Read the sensors:
    # Enter here functions to read sensor data, like:
    #  val = ds.getValue()

    # Process sensor data here.

    # Enter here functions to send actuator commands, like:
    #  motor.setPosition(10.0)
    
    l_motor.setVelocity(10)
    r_motor.setVelocity(10)
    
    print(df.getValue())
    pass

# Enter here exit cleanup code.
