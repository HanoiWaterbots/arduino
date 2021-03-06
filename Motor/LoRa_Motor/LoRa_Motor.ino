#include <Servo.h>

/* LoRa */

// Pin settings
#define RECEIVER_MD0 7
#define RECEIVER_MD1 6
//#define LoRaReceiver Serial1
#define LoRaReceiver Serial

// LoRa modes
#define NORMAL 0
#define WAKEUP 1
#define SAVE 2
#define SLEEP 3

// LoRa settings
byte ADDH = 0x12;
byte ADDL = 0x34;
byte SPEED = 0x1A;
byte CHAN = 0x17;
byte OPTION = 0x44;

byte SENDER_ADDH = 0x12;
byte SENDER_ADDL = 0x30;
byte SENDER_SPEED = 0x1A;
byte SENDER_CHAN = 0x17;
byte SENDER_OPTION = 0x44;

byte RECEIVER_ADDH = 0x12;
byte RECEIVER_ADDL = 0x31;
byte RECEIVER_SPEED = 0x1A;
byte RECEIVER_CHAN = 0x17;
byte RECEIVER_OPTION = 0x44;

byte SENDER_ADD[2] = { 0x12, 0x30 };
byte RECEIVER_ADD[2] = { 0x12, 0x31 };
byte BROADCAST_ADD[2] = { 0xff, 0xff };
String SENDER_ADD_STR = String((char *) SENDER_ADD);
String RECEIVER_ADD_STR = String((char *) RECEIVER_ADD);
String BROADCAST_ADD_STR = String((char *) BROADCAST_ADD);

// LoRa commands
byte SET_SETTINGS_PERM[] = { 0xC0, ADDH, ADDL, SPEED, CHAN, OPTION }; // write to flash
byte SET_SETTINGS[] = { 0xC2, ADDH, ADDL, SPEED, CHAN, OPTION }; // write temporarily

byte READ_SETTINGS[] = { 0xC1, 0xC1, 0xC1 };
byte READ_VERSION_NO[] = { 0xC3, 0xC3, 0xC3 };
byte RESET[] = { 0xC4, 0xC4, 0xC4 };
byte READ_VOLTAGE[] = { 0xC5, 0xC5, 0xC5 };


void LoRaSetMode(int md0, int md1, int mode) {
  digitalWrite(md0, mode % 2);
  digitalWrite(md1, mode / 2);
}

void LoRaSendReadCmd(HardwareSerial &LoRa, byte cmd[]) {
  LoRa.write(cmd, sizeof(cmd));
}

void LoRaSendSetCmd(HardwareSerial &LoRa, byte cmd[], byte addh, byte addl, byte sp, byte chan, byte option) {
  cmd[1] = addh; cmd[2] = addl; cmd[3] = sp; cmd[4] = chan; cmd[5] = option;
  LoRa.write(cmd, sizeof(cmd));
}

/* Motor */

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define BOTH_MOTOR 2
#define NO_MOTOR -1
Servo ESC1, ESC2;
int leftMotor = 1550;
int rightMotor = 1550;

/* Main */

int inCommand = 0;
int motor = NO_MOTOR;

void setup() {
  //Serial.begin(57600);

  // LoRa
  pinMode(RECEIVER_MD0, OUTPUT);
  pinMode(RECEIVER_MD1, OUTPUT);

  LoRaReceiver.begin(9600);

  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, SLEEP);
  LoRaSendSetCmd(LoRaReceiver, SET_SETTINGS, RECEIVER_ADDH, RECEIVER_ADDL, RECEIVER_SPEED, RECEIVER_CHAN, RECEIVER_OPTION);
  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, NORMAL);

  // Motor
  ESC1.attach(11);  
  ESC2.attach(10);

  // Arm the motors
  ESC1.writeMicroseconds(2000);
  ESC2.writeMicroseconds(2000);
  delay(500);

  ESC1.writeMicroseconds(leftMotor);
  ESC2.writeMicroseconds(rightMotor);
  delay(2000);

}

void loop() {
  if (LoRaReceiver.available()) {
    takeCommand();
  }
}

void takeCommand() {
  //Serial.print("[LoRa RECEIVE] ");
  if (inCommand) {
    int num = LoRaReceiver.parseInt();
    //Serial.print(num);
    //LoRaReceiver.print(num);

    switch(motor) {
      case NO_MOTOR:
        break;
      case LEFT_MOTOR:
        if (num >= 700 && num <= 2500) {
          ESC1.writeMicroseconds(num);
          leftMotor = num;
          delay(1000);
        }
        break;
      case RIGHT_MOTOR:
        if (num >= 700 && num <= 2500) {
          ESC2.writeMicroseconds(num);
          rightMotor = num;
          delay(1000);
        }
        break;
      case BOTH_MOTOR: {
        leftMotor = num;
        char dummy = LoRaReceiver.read(); // ';'
        rightMotor = LoRaReceiver.parseInt();
        //Serial.print(dummy);
        //Serial.print(rightMotor);
        ESC1.writeMicroseconds(leftMotor);
        ESC2.writeMicroseconds(rightMotor);
        delay(1000);
        break;
      }
      default:
        break;
    }
    motor = NO_MOTOR;
    inCommand = 0;
  } else {
    char c = LoRaReceiver.read();
    //Serial.print(c);
    //LoRaReceiver.write(c);

    inCommand = 1;
    switch(c) {
      case 'l':
        motor = LEFT_MOTOR;
        break;
      case 'r':
        motor = RIGHT_MOTOR;
        break;
      case 'b':
        motor = BOTH_MOTOR;
        break;
      default:
        motor = NO_MOTOR;
        inCommand = 0;
        break;
    }
  }
  //LoRaReceiver.flush();
  //Serial.println("");
}
