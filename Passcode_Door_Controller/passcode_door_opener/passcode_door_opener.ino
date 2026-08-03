#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servo;

LiquidCrystal_I2C lcd(0x27, 16, 2);


#define SERVO_PIN 3

#define JOY_X A0
#define JOY_Y A1
#define BUTTON_PIN 2

#define BUZZER_PIN 4
#define LED_PIN 5

#define TRIG_PIN 10
#define ECHO_PIN 9


String password = "1234";
String enteredPassword = "";

bool unlocked = false;

int servoPosition = 120;



void setup() {

  Serial.begin(9600);


  servo.attach(SERVO_PIN);


  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);


  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  digitalWrite(LED_PIN, LOW);


  // Start locked position
  servoPosition = 120;
  servo.write(servoPosition);



  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.print("Enter Password");


  Serial.println("Enter Password:");

}







void loop() {



  // PASSWORD MODE

  if (!unlocked) {


    if (Serial.available()) {


      enteredPassword = Serial.readStringUntil('\n');

      enteredPassword.trim();



      if (enteredPassword == password) {


        unlocked = true;


        digitalWrite(LED_PIN, HIGH);


        tone(BUZZER_PIN, 1000, 150);



        lcd.clear();

        lcd.print("Access Granted");


        delay(1000);


        lcd.clear();

        lcd.print("Door Unlocked");


        Serial.println("Joystick Enabled");


      }


      else {


        lcd.clear();

        lcd.print("Wrong Password");


        delay(1000);


        lcd.clear();

        lcd.print("Enter Password");


      }



      enteredPassword = "";

    }


    return;

  }







  // ULTRASONIC SENSOR

  long distance = getDistance();



  if (distance > 0 && distance < 5) {


    lockDoor();
      


    delay(1000);


    return;

  }







  // BUTTON LOCK

  if (digitalRead(BUTTON_PIN) == LOW) {


    lockDoor();


    delay(1000);


    return;

  }







  // JOYSTICK CONTROL

  int joystickX = analogRead(JOY_X);

  int joystickY = analogRead(JOY_Y);





  // LEFT = 0 degrees

  if (joystickX < 400) {


    servoPosition = 0;

    servo.write(servoPosition);



    delay(300);

  }




  // RIGHT = 180 degrees

  if (joystickX > 600) {


    servoPosition = 180;

    servo.write(servoPosition);


    delay(300);

  }




  // UP/DOWN = RESET TO 120 degrees

  if (joystickY < 400 || joystickY > 600) {


    servoPosition = 120;

    servo.write(servoPosition);


    delay(300);

  }



  delay(20);

}









void lockDoor() {


  // Set servo to locked position
  lcd.clear();

  Serial.println("Door is Locking");

  lcd.print("Door is Locking");

  delay(2000);

  servoPosition = 120;

  servo.write(servoPosition);



  unlocked = false;


  digitalWrite(LED_PIN, LOW);

  lcd.clear();

  lcd.print("Enter Password:");
  Serial.println("Enter Password:");

}









long getDistance() {


  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);



  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);



  digitalWrite(TRIG_PIN, LOW);



  long duration = pulseIn(ECHO_PIN, HIGH, 20000);



  long distance = duration * 0.034 / 2;



  return distance;

}