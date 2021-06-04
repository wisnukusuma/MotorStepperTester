# MotorStepperTester
Testing motor stepper and encoder using arduino uno 
This is an arduino code for testing motor stepper, all pins connection already commented on the script, to use it you need a a4988 motor driver. And to give a command open the terminal and using following format
*...,...,...#
... is the parameters
first parameter is the dirrection by giving 0,1,2. 0 is stop, 1 is right, 2 is left relatively.
second parameter is the step you want in integer.
third parameter is the speed you want, is integer from 0 to 100

Example : 
*1,1000,50#
it means that move the stepper to the right a thousand step with 50% speed 
