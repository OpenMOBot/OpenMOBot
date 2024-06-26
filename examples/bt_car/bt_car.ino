/*

MIT License

Copyright (c) [2024] [OpenMOBOt]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma region Definitions

/**
 * @brief Enable status LED.
 *
 */
#define ENABLE_STATUS_LED

/**
 * @brief Enable motors of the robot features.
 *
 */
#define ENABLE_MOTORS

/**
 * @brief Enable sonar servo feature.
 *
 */
#define ENABLE_SONAR_SERVO

/**
 * @brief Enable sonar sensor.
 *
 */
// #define ENABLE_SONAR

/**
 * @brief Enable PID regulators.
 *
 */
// #define ENABLE_PID

/**
 * @brief Enable bluetooth interface.
 *
 */
#define ENABLE_BT

/**
 * @brief Time interval for update cycle.
 *
 */
#define UPDATE_INTERVAL_MS 100

/**
 * @brief Time interval for debug update cycle.
 *
 */
#define DEBUG_UPDATE_INTERVAL_MS 100

#if defined(ENABLE_PID)

/**
 * @brief PID update interval.
 *
 */
#define PID_UPDATE_INTERVAL_MS 100

/**
 * @brief P value
 *
 */
#define CONST_P 1.0

/**
 * @brief I value
 *
 */
#define CONST_I 0.00

/**
 * @brief D value
 *
 */
#define CONST_D 0.00

#endif // ENABLE_PID

#if defined(ENABLE_BT)
/**
 * @brief Bluetooth device name.
 *
 */
#define BT_DEVICE_NAME "OpenMOBot"

/**
 * @brief Bluetooth device timeout.
 *
 */
#define BT_TIMEOUT 5

/**
 * @brief Uncomment this to use PIN during pairing. The pin is specified on the line below.
 *
 */
// #define BT_PIN "1234"

/**
 * @brief Bluetooth Forward command.
 *
 */
#define CMD_FORWARD 'F'

/**
 * @brief Bluetooth Left turn command.
 *
 */
#define CMD_LEFT_TURN 'L'

/**
 * @brief Bluetooth Backward command.
 *
 */
#define CMD_BACKWARD 'B'

/**
 * @brief Bluetooth Right turn command.
 *
 */
#define CMD_RIGHT_TURN 'R'

/**
 * @brief Bluetooth Stop command.
 *
 */
#define CMD_STOP 'S'

/**
 * @brief Bluetooth Toggle M command.
 *
 */
#define CMD_TOGGLE_M 'M'

/**
 * @brief Bluetooth Toggle m command.
 *
 */
#define CMD_TOGGLE_m 'm'

/**
 * @brief Bluetooth Toggle N command.
 *
 */
#define CMD_TOGGLE_N 'N'

/**
 * @brief Bluetooth Toggle n command.
 *
 */
#define CMD_TOGGLE_n 'n'

/**
 * @brief Bluetooth Servo J position command.
 *
 */
#define CMD_SERVO_J 'J'

/**
 * @brief Bluetooth Servo K position command.
 *
 */
#define CMD_SERVO_K 'K'

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled!Please run `make menu config` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled.It is only available for the ESP32 chip.
#endif

#endif // ENABLE_BT

#if defined(ENABLE_MOTORS)
/**
 * @brief Motors PWM absolute maximum.
 *
 */
#define PWM_ABSOLUTE_MAX 255

/**
 * @brief Motors PWM software limitation.
 *
 */
#define PWM_MAX 200

/**
 * @brief Motors PWM acceleration step.
 *
 */
#define PWM_STEP 5
#endif // ENABLE_MOTORS

#if defined(ENABLE_SONAR_SERVO) || defined(ENABLE_BT)
/**
 * @brief Sonar default servo position.
 *
 */
#define DEFAULT_SERVO_POS 90
#endif // defined(ENABLE_SONAR_SERVO) || defined(ENABLE_BT)

#pragma endregion

#pragma region Headers

#include "OpenMOBot.h"

#if defined(ENABLE_SONAR_SERVO)
#include <ESP32Servo.h>
#endif // ENABLE_SONAR_SERVO

#if defined(ENABLE_PID)
#include <PID_v1.h>
#endif // ENABLE_PID

#if defined(ENABLE_BT)
#include "BluetoothSerial.h"
#endif // ENABLE_BT

#pragma endregion

#pragma region Functions Prototypes

#if defined(ENABLE_BT)
/**
 * @brief Read sensors.
 *
 */
void send_sensors();

/**
 * @brief Read bluetooth commands.
 *
 */
void read_bt_serial();
#endif // ENABLE_BT

#if defined(ENABLE_MOTORS)
/**
 * @brief Update direction control.
 *
 */
void update_direction_control();

/** @brief Interrupt Service Routine for handling left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder();

/** @brief Interrupt Service Routine for handling right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder();
#endif // ENABLE_MOTORS

#pragma endregion

#pragma region Enums

enum Directions_t : uint8_t
{
    Stop = 0,
    Forward = 1,
    Backward = 2,
    LeftTurn = 4,
    RightTurn = 8
};

#pragma endregion

#pragma region Variables

#if defined(ENABLE_STATUS_LED)
/**
 * @brief Blink led last state flag.
 */
int StateStatusLED_g = LOW;
#endif // ENABLE_STATUS_LED

#if defined(ENABLE_BT)
/**
 * @brief Bluetooth serial interface.
 *
 */
BluetoothSerial SerialBT_g;

/**
 * @brief Received command.
 *
 */
char RecvCmd_g;

/**
 * @brief Bluetooth number parser.
 *
 */
String CmdNumPart_g = "";

/**
 * @brief LED 1 flag.
 *
 */
bool LED1_g = false;

/**
 * @brief LED 2 flag.
 *
 */
bool LED2_g = false;

/**
 * @brief Servo K position.
 *
 */
int ServoKPos_g = DEFAULT_SERVO_POS;

/**
 * @brief Measured temperature.
 *
 */
float Temp_g = 0.0;

#endif // ENABLE_BT

#if defined(ENABLE_MOTORS)
/**
 * @brief Robot direction.
 *
 */
Directions_t Direction_g;
#endif // ENABLE_MOTORS

#if defined(ENABLE_MOTORS) || defined(ENABLE_BT) || defined(ENABLE_PID)
/**
 * @brief Left motor PWM.
 *
 */
double PWMLeft_g = 0;

/**
 * @brief Right motor PWM.
 *
 */
double PWMRight_g = 0;
#endif // defined(ENABLE_MOTORS) || defined(ENABLE_BT) || defined(ENABLE_PID)

#if defined(ENABLE_SONAR_SERVO)
/**
 * @brief Create servo object to control a servo.
 */
Servo UsServo_g;
#endif // ENABLE_SONAR_SERVO

#if defined(ENABLE_SONAR_SERVO) || defined(ENABLE_BT)
/**
 * @brief Servo J position.
 *
 */
int SonarServoPos_g = DEFAULT_SERVO_POS;
#endif // defined(ENABLE_SONAR_SERVO) || defined(ENABLE_BT)

#if defined(ENABLE_SONAR) || defined(ENABLE_BT)
/**
 * @brief Distance from sonar.
 *
 */
float Distance_g = 0.0;
#endif // defined(ENABLE_SONAR) || defined(ENABLE_BT)

/**
 * @brief Update timer instance.
 */
FxTimer *UpdateTimer_g;

/**
 * @brief Send timer instance.
 */
FxTimer *SendTimer_g;

#if defined(ENABLE_SONAR)
/**
 * @brief Ultrasonic sensor.
 */
HCSR04 HCSR04_g;
#endif // ENABLE_SONAR

#if defined(ENABLE_PID)

/**
 * @brief Define Variables we'll be connecting to
 */
double FBLeft_g, FBRight_g, OutLeft_g, OutRight_g;

/**
 * @brief PID regulator for left wheel.
 */
PID *PIDLeft_g;

/**
 * @brief PID regulator for right wheel.
 */
PID *PIDRight_g;

#endif // ENABLE_PID

#pragma endregion

void setup()
{

    // Init serial
    Serial.begin(DEFAULT_BAUD);

#if defined(ENABLE_MOTORS)
    // Attach the Interrupts to their ISR's
    pinMode(PIN_LEFT_ENCODER, INPUT_PULLUP);
    pinMode(PIN_RIGHT_ENCODER, INPUT_PULLUP);

    // Increase counter 1 when speed sensor pin goes High.
    attachInterrupt(digitalPinToInterrupt(PIN_LEFT_ENCODER), ISR_Left_Encoder, RISING);
    // Increase counter 2 when speed sensor pin goes High.
    attachInterrupt(digitalPinToInterrupt(PIN_RIGHT_ENCODER), ISR_Right_Encoder, RISING);

    // Setup the motor driver.
    MotorModel_t MotorModelL = {
        PIN_L_F,
        PIN_L_B,
        PIN_L_PWM,
        PIN_R_F,
        PIN_R_B,
        PIN_R_PWM,
        WHEEL_DIAMETER,
        DISTANCE_BETWEEN_WHEELS,
        ENCODER_TRACKS};

    // Initialize the motor controller.
    MotorController.init(&MotorModelL);
#endif // ENABLE_MOTORS

#if defined(ENABLE_SONAR_SERVO)
    // Attaches servo to the servo object.
    UsServo_g.attach(PIN_US_SERVO);
#endif // ENABLE_SONAR_SERVO

#if defined(ENABLE_SONAR)
    // Initialize the ultrasonic.
    HCSR04_g.init(PIN_US_TRIG, PIN_US_ECHO);
#endif // ENABLE_SONAR

#if defined(ENABLE_STATUS_LED)
    pinMode(PIN_USER_LED, OUTPUT);
#endif // ENABLE_STATUS_LED

#if defined(ENABLE_BT)
    // Bluetooth device name.
    SerialBT_g.begin(BT_DEVICE_NAME);
    SerialBT_g.setTimeout(BT_TIMEOUT);
#ifdef BT_PIN
    SerialBT_g.setPin(BT_PIN);
#endif // BT_PIN
#endif // ENABLE_BT

#if defined(ENABLE_PID)
    // Set the PID regulators.
    PIDLeft_g = new PID(&FBLeft_g, &OutLeft_g, &PWMLeft_g, CONST_P, CONST_I, CONST_D, DIRECT);
    PIDLeft_g->SetMode(AUTOMATIC);
    PIDLeft_g->SetSampleTime(PID_UPDATE_INTERVAL_MS);
    PIDLeft_g->SetOutputLimits(-PWM_MAX, PWM_MAX);

    PIDRight_g = new PID(&FBRight_g, &OutRight_g, &PWMRight_g, CONST_P, CONST_I, CONST_D, DIRECT);
    PIDRight_g->SetMode(AUTOMATIC);
    PIDRight_g->SetSampleTime(PID_UPDATE_INTERVAL_MS);
    PIDRight_g->SetOutputLimits(-PWM_MAX, PWM_MAX);
#endif // ENABLE_PID

    UpdateTimer_g = new FxTimer();
    UpdateTimer_g->setExpirationTime(UPDATE_INTERVAL_MS);
    UpdateTimer_g->updateLastTime();

    SendTimer_g = new FxTimer();
    SendTimer_g->setExpirationTime(DEBUG_UPDATE_INTERVAL_MS);
    SendTimer_g->updateLastTime();
}

void loop()
{

#if defined(ENABLE_BT)
    // Read serial.
    read_bt_serial();
#endif // ENABLE_BT

    // Update the timer.
    UpdateTimer_g->update();
    if (UpdateTimer_g->expired())
    {
        UpdateTimer_g->updateLastTime();
        UpdateTimer_g->clear();

#if defined(ENABLE_SONAR)
        long MicrosecL = HCSR04_g.timing();
        Distance_g = HCSR04_g.convert(MicrosecL, HCSR04::CM);
#endif // ENABLE_SONAR

#if defined(ENABLE_BT)
        // Send feed back.
        send_sensors();
#endif // ENABLE_BT

#if defined(ENABLE_MOTORS)
        // Update the PWM.
        update_direction_control();

        // Update motor speeds.
        MotorController.update();
#endif // ENABLE_MOTORS

#if defined(ENABLE_PID)
        PIDLeft_g->Compute();
        PIDRight_g->Compute();

#if defined(ENABLE_MOTORS)
        // Set the input feedback.
        FBLeft_g = MotorController.GetLeftMotorRPM();   // / 5.0;
        FBRight_g = MotorController.GetRightMotorRPM(); // / 5.0;

        // Set the output from the regulator.
        MotorController.SetPWM(OutLeft_g, OutRight_g);
#endif // ENABLE_MOTORS
#else
#if defined(ENABLE_MOTORS)
        // Set the PWMs to the motors.
        MotorController.SetPWM(PWMLeft_g, PWMRight_g);
#endif // ENABLE_MOTORS
#endif // ENABLE_PID

#if defined(ENABLE_SONAR_SERVO)
        // Set servo J position.
        UsServo_g.write(int(map(SonarServoPos_g, 0, 180, 180, 0)));
#endif // ENABLE_SONAR_SERVO

#if defined(ENABLE_STATUS_LED)
        // set the LED with the StateStatusLED_g of the variable:
        StateStatusLED_g = !StateStatusLED_g;
        digitalWrite(PIN_USER_LED, StateStatusLED_g);
#endif // ENABLE_STATUS_LED
    }

    SendTimer_g->update();
    if (SendTimer_g->expired())
    {
        SendTimer_g->updateLastTime();
        SendTimer_g->clear();

#if defined(ENABLE_PID)
#if defined(ENABLE_MOTORS)
        Serial.print("PWMLeft_g:");
        Serial.print(PWMLeft_g);
        Serial.print(",");
        Serial.print("PWMRight_g:");
        Serial.print(PWMRight_g);
        Serial.print(", ");
        Serial.print("FBLeft_g:");
        Serial.print(FBLeft_g);
        Serial.print(", ");
        Serial.print("FBRight_g:");
        Serial.print(FBRight_g);
        Serial.print(", ");
        Serial.print("OutRight_g:");
        Serial.println(OutRight_g);
        Serial.print(", ");
        Serial.print("OutLeft_g:");
        Serial.print(OutLeft_g);
#endif // ENABLE_MOTORS
#else
#if defined(ENABLE_MOTORS)
        Serial.print("PWMLeft_g:");
        Serial.print(PWMLeft_g);
        Serial.print(",");
        Serial.print("PWMRight_g:");
        Serial.print(PWMRight_g);
#endif // ENABLE_MOTORS
#endif // ENABLE_PID
        Serial.println();
    }
}

#pragma region Functions

#if defined(ENABLE_BT)
/**
 * @brief Read sensors.
 *
 */
void send_sensors()
{
    SerialBT_g.print("D");
    SerialBT_g.println(Distance_g);
    SerialBT_g.print("T");
    SerialBT_g.println(Temp_g);
}

/**
 * @brief Read bluetooth commands.
 *
 */
void read_bt_serial()
{
    if (Serial.available())
    {
        SerialBT_g.write(Serial.read());
    }

    while (SerialBT_g.available() > 0)
    {
        String LineL = SerialBT_g.readStringUntil('\n');
        Serial.printf("code received: %s\n", LineL);
        CmdNumPart_g = "";
        for (int index = 0; index < LineL.length(); index++)
        {
            int CharacterL = LineL[index];
            if (isDigit(CharacterL))
            {
                CmdNumPart_g += (char)CharacterL;
            }
            else if (CharacterL != '\n')
            {
                RecvCmd_g = CharacterL;
            }
            else
            {
                break;
            }
        }
    }

    switch (RecvCmd_g)
    {
    case CMD_FORWARD:
        Direction_g = Directions_t::Forward;
        break;
    case CMD_LEFT_TURN:
        Direction_g = Directions_t::LeftTurn;
        break;
    case CMD_BACKWARD:
        Direction_g = Directions_t::Backward;
        break;
    case CMD_RIGHT_TURN:
        Direction_g = Directions_t::RightTurn;
        break;
    case CMD_STOP:
        Direction_g = Directions_t::Stop;
        break;
    case CMD_TOGGLE_M:
        LED1_g = true;
        break;
    case CMD_TOGGLE_m:
        LED1_g = false;
        break;
    case CMD_TOGGLE_N:
        LED2_g = true;
        break;
    case CMD_TOGGLE_n:
        LED2_g = false;
        break;
    case CMD_SERVO_J:
        if (CmdNumPart_g != "")
        {
            SonarServoPos_g = CmdNumPart_g.toInt();
        }
        break;
    case CMD_SERVO_K:
        if (CmdNumPart_g != "")
        {
            ServoKPos_g = CmdNumPart_g.toInt();
        }
        break;
    }
}
#endif // ENABLE_BT

#if defined(ENABLE_MOTORS)
/**
 * @brief Update direction control.
 *
 */
void update_direction_control()
{
    if (Direction_g == Directions_t::Forward)
    {
        PWMLeft_g += PWM_STEP;
        PWMRight_g += PWM_STEP;
    }
    else if (Direction_g == Directions_t::Backward)
    {
        PWMLeft_g -= PWM_STEP;
        PWMRight_g -= PWM_STEP;
    }
    else if (Direction_g == Directions_t::LeftTurn)
    {
        PWMLeft_g -= PWM_STEP;
        PWMRight_g += PWM_STEP;
    }
    else if (Direction_g == Directions_t::RightTurn)
    {
        PWMLeft_g += PWM_STEP;
        PWMRight_g -= PWM_STEP;
    }
    else if (Direction_g == Directions_t::Stop)
    {
        PWMLeft_g = 0;
        PWMRight_g = 0;
    }

    // Limit Left PWM.
    if (PWMLeft_g > PWM_MAX)
    {
        PWMLeft_g = PWM_MAX;
    }
    // Limit Left PWM by absolute maximum.
    if (PWMLeft_g > PWM_ABSOLUTE_MAX)
    {
        PWMLeft_g = PWM_ABSOLUTE_MAX;
    }
    // Limit Left PWM.
    if (PWMLeft_g < -PWM_MAX)
    {
        PWMLeft_g = -PWM_MAX;
    }
    // Limit Left PWM by absolute maximum.
    if (PWMLeft_g < -PWM_ABSOLUTE_MAX)
    {
        PWMLeft_g = -PWM_ABSOLUTE_MAX;
    }

    // Limit Right PWM.
    if (PWMRight_g > PWM_MAX)
    {
        PWMRight_g = PWM_MAX;
    }
    // Limit Right PWM by absolute maximum.
    if (PWMRight_g > PWM_ABSOLUTE_MAX)
    {
        PWMRight_g = PWM_ABSOLUTE_MAX;
    }
    // Limit Right PWM.
    if (PWMRight_g < -PWM_MAX)
    {
        PWMRight_g = -PWM_MAX;
    }
    // Limit Right PWM by absolute maximum.
    if (PWMRight_g < -PWM_ABSOLUTE_MAX)
    {
        PWMRight_g = -PWM_ABSOLUTE_MAX;
    }
}

/** @brief Interrupt Service Routine for handling left encoder.
 *  @return Void.
 */
void ISR_Left_Encoder()
{
    MotorController.UpdateLeftEncoder();
}

/** @brief Interrupt Service Routine for handling right encoder.
 *  @return Void.
 */
void ISR_Right_Encoder()
{
    MotorController.UpdateRightEncoder();
}
#endif // ENABLE_MOTORS

#pragma endregion
