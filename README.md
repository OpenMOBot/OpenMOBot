# OpenMOBot

## Educational Robot System

### Review

This robot system is dedicated to focus on a few different aspects of education of robotics electronics and programming.
The system provides modular robotics hardware that can be separated on a modular education blocks. Robot system covers:

 - Mechanical part that helps youngsters to understand how to build the robot and assemble the robotic parts together
 - Modular electronics that is extendable of various parts.
 - Modular software that is open source and easy to rewrite.

### Electronic schematic

In this section we will describe the by schematic diagram how the electrical connection of the robot are made.

![](https://github.com/OpenMOBot/OpenMOBot/blob/create_reademe/schematic/OpenBot_bb.png?raw=true)

### Installing the Arduino library
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

### Getting started

In this particular section we will describe the examples that are provided by this library.

 - [blink_led](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/blink_led/blink_led.ino)

This example is just blinking the build in LED. The purpose is to heat you up and see fas results with maximum safety before the run.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_blink_led.png)

 - [line_sensor](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/line_sensor/line_sensor.ino)

Line sensor example is created to understand and test line sensor functionalities of the robot. This example also helps the user to understand does the electrical connections are good enough and is it connected properly in general way. For this kind of sensors is vary often to swap cables and pins, thats why this example is very good for test.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_line_sensor.png)

 - [us_servo](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/us_servo/us_servo.ino)

The example simply runs the ultra-sonic sensor servo motor to see does it work properly or is it connected properly.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_us_servo.png)

 - [us_servo_2](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/us_servo_2/us_servo_2.ino)

Second example are provided but this time it includes the ultra-sonic sensor distance information.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_us_servo_2.png)

 - [run_motors](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors/run_motors.ino)

After we move our first motor, the servo motor it is time to move the main motors or engines of our robot. The example is focused to do this job, and provide nice information about its position, and passed way. <u>This example also provide a test for correctly connected motors and photo encoders.</u>

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors.png)

 - [run_motors_2](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors_2/run_motors_2.ino)

Great, if are on this stage of tests you are successfully run your motors.
Likely the previous example we did read the motor position information, this time we will get more deep and will reed motor speed information.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors_2.png)

 - [run_motors_3](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors_3/run_motors_3.ino)

Unfortunately because it is educational and hobby robot. This example only runs the left engine with PID regulator. We believe that if you pass trough this example it is time you to start writing your own code.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors_3.png)

  - [line_follower](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/line_follower/line_follower.ino)

By this time (2023 5-th of November) we agree that this is the last example.
We believe that you as enthusiast will start write your application using our API. After all it is your turn to show yourself what can you do with this robot. <u>The example will provide a line following robot.</u>

# Contributing

If you'd like to contribute to this project, please follow these steps:

1. Fork the repository on GitHub.
2. Clone your forked repository to your local machine.
3. Create a new branch for your changes: `git checkout -b my-new-feature`.
4. Make your modifications and write tests if applicable.
5. Commit your changes: `git commit -am 'Add some feature'`.
6. Push the branch to your forked repository: `git push origin my-new-feature`.
7. Create a pull request on the main repository.

We appreciate your contributions!

# License

This project is licensed under the MIT License. See the [MIT](https://www.mit.edu/~amini/LICENSE.md) file for more details.