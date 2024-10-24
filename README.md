# OpenMOBot

## Educational Robot System

### Review

This robot system is focused on a few different aspects from education on robotics, electronics, and programming.
The system provides modular robotics hardware that can be separated on a modular education blocks. The robot system covers:

 - Mechanical part that helps youngsters understand how to build the robot and assemble the robotic parts together.
 - Modular electronics that is extendable of various parts.
 - Modular software that is open source and easy to rewrite.

### Electronic schematic

In this section, we will describe, through a schematic diagram, how the electrical connections of the robot are made.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/development/schematic/OpenMOBot_png.png)

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

In this particular section, we will describe the examples that are provided by this library.

 - [blink_led](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/blink_led/blink_led.ino)

This example is just blinking the built-in LED. It aims to prepare you and motivate you with fast results, while maintaining maximum safety, for the run.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_blink_led.png)

 - [line_sensor](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/line_sensor/line_sensor.ino)

The line sensor example is created to understand and test the line sensor functionalities of the robot. This example also helps the user to double-check if the electrical connections are in check and if they are properly connected. It is typical for this kind of sensors to swap cables and pins. That's why this example is very good for test.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_line_sensor.png)

 - [us_servo](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/us_servo/us_servo.ino)

This example simply runs the servo motor of the ultrasonic sensor to see if it works properly or whether it is connected properly.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_us_servo.png)

 - [us_servo_2](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/us_servo_2/us_servo_2.ino)

A second example is provided, but this time it includes the ultrasonic sensor distance information.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_us_servo_2.png)

 - [run_motors](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors/run_motors.ino)

After we move our first motor (the servo motor), it is time to move the main motors or engines of our robot. This is the purpose of the current example. Furthermore, it provides good information about the motors/engine position, and distance travelled. <u>This example also provides a test for correctly connected motors and photo encoders.</u>

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors.png)

 - [run_motors_2](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors_2/run_motors_2.ino)

Great, if you are at this stage of the tests, you have successfully run your motors.
As in the previous example, we read the motor position information, but this time we will dive deeper and will read the motor speed information.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors_2.png)

 - [run_motors_3](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/run_motors_3/run_motors_3.ino)

Unfortunately, because it is an educational and a 'hobby' robot, this example only runs the left engine with PID regulator. We believe that when you pass through this example, it is time that you to start writing your own code.

![](https://raw.githubusercontent.com/OpenMOBot/OpenMOBot/create_reademe/images/img_1_run_motors_3.png)

  - [line_follower](https://github.com/OpenMOBot/OpenMOBot/blob/development/examples/line_follower/line_follower.ino)

As of this date (2023 5-th of November), this is the last example provided.
We believe that you as an enthusiast will begin writing your application using our API. After all, it is your turn to show to yourself what you can do with this robot. <u>The example will provide a line following functionality.</u>

# Custom Things

### Pin definitons

The actual definitions can be seen in the [following file](https://github.com/OpenMOBot/OpenMOBot/blob/main/src/OpenMOBot.h) @ lines 47 to 238

| Definition | ATmega328p (Uno) | ATmega2560 (Mega) | ESP32 (D1 R32) |
|----------|:-------------|:------|:------|
| PIN_LS_1 | A0 | 8 | 2 |
| PIN_LS_2 | A1 | 9 | 4 |
| PIN_LS_3 | A2 | 10 | 35 |
| PIN_LS_4 | A3 | 11 | 34 |
| PIN_LS_5 | A4 | 12 | 36 |
| PIN_LS_6 | A5 | 13 | 39 |
| PIN_LS_7 | - | 14 | - |
| PIN_LS_8 | - | 15 | - |
| PIN_LEFT_ENCODER | 3 | 3 | 25 |
| PIN_RIGHT_ENCODER | 2 | 2 | 26 |
| PIN_L_F | 7 | 7 | 27 |
| PIN_L_B | 6 | 6 | 14 |
| PIN_L_PWM | 10 | 10 | 5 |
| PIN_R_F | 5 | 5 | 17 |
| PIN_R_B | 4 | 4 | 16 |
| PIN_R_PWM | 9 | 9 | 13 |
| PIN_US_SERVO | 11 | 11 | 23 |
| PIN_USER_LED | 13 | 13 | 18 |
| PIN_US_TRIG | 8 |  | 12 |
| PIN_US_ECHO | 12 |  | 19 |

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
