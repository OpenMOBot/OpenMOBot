# OpenMOBot

## Educational Robot System

**Review**

This robot system is dedicated to focus on a few different aspects of education of robotics electronics and programming.
The system provides modular robotics hardware that can be separated on a modular education blocks. Robot system covers:

 - Mechanical part that helps youngsters to understand how to build the robot and assemble the robotic parts together
 - Modular electronics that is extendable of various parts.
 - Modular software that is open source and easy to rewrite.

**Installing the Arduino library**
 - Go to the [main branch](https://github.com/OpenMOBot/OpenMOBot).
 - Click first on Clone or Download > [Download ZIP](https://github.com/OpenMOBot/OpenMOBot/archive/refs/heads/main.zip).
 - Unzip it and place it in Arduino Libraries folder.

Windows users:
```
Documents > Arduino > libraries
```
 - Reopen Arduino IDE and you should have the library examples in:
```
File > Examples > OpenMOBot
```

Linux users:

 - Create content.

MAC users:

 - Create content.

**Getting started**

In this particular section we will describe the examples that are provided by this library.

 - blink_led

This example is just blinking the build in LED. The purpose is to heat you up and see fas results with maximum safety before the run.

 - line_sensor

Line sensor example is created to understand and test line sensor functionalities of the robot. This example also helps the user to understand does the electrical connections are good enough and is it connected properly in general way. For this kind of sensors is vary often to swap cables and pins, thats why this example is very good for test.

 - us_servo

The example simply runs the ultra-sonic sensor servo motor to see does it work properly or is it connected properly.

 - us_servo_2

Second example are provided but this time it includes the ultra-sonic sensor distance information.

 - run_motors

After we move our first motor, the servo motor it is time to move the main motors or engines of our robot. The example is focused to do this job, and provide nice information about its position, and passed way. <u>This example also provide a test for correctly connected motors and photo encoders.</u>

 - run_motors_2

Great, if are on this stage of tests you are successfully run your motors.
Likely the previous example we did read the motor position information, this time we will get more deep and will reed motor speed information.

 - run_motors_3

Unfortunately because it is educational and hobby robot. This example only runs the left engine with PID regulator. We believe that if you pass trough this example it is time you to start writing your own code.

  - line_follower

By this time (2023 5-th of November) we agree that this is the last example.
We believe that you as enthusiast will start write your application using our API. After all it is your turn to show yourself what can you do with this robot. The example will provide a line following robot.