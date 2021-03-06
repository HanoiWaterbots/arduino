#include <SimpleTimer.h>

// Pin settings
#define SENDER_MD0 7
#define SENDER_MD1 6
#define SENDER_AUX 3
#define RECEIVER_MD0 5
#define RECEIVER_MD1 4
#define RECEIVER_AUX 2
#define LoRaSender Serial1
#define LoRaReceiver Serial2

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

// Utilities
#define END_OF_BUFFER '\0'
#define DATA_SIZE_LIMIT 512 // Strictly speaking, 512 - 1, one for END_OF_BUFFER symbol
#define PACKET_DATA_SIZE 256
#define PACKET_HEADER_SIZE (sizeof(SENDER_ADD) + sizeof(RECEIVER_ADD))

SimpleTimer sendDataTimer;
int sendDataTimerId;

// Global variables
int senderAux = HIGH, senderLastAux;
byte senderDataBuffer[DATA_SIZE_LIMIT];
int receiverAux = HIGH, receiverLastAux;
int sending = 0;

/* Helpers */

void printBuffer(byte buf[], size_t bufSize) {
  for (int i = 0; buf[i] != END_OF_BUFFER && i < bufSize; i++)  {
    Serial.print(buf[i], HEX); 
  }
  Serial.println();
}

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

/* Main */

void setup() {
  pinMode(SENDER_MD0, OUTPUT);
  pinMode(SENDER_MD1, OUTPUT);
  pinMode(SENDER_AUX, INPUT);
  pinMode(RECEIVER_MD0, OUTPUT);
  pinMode(RECEIVER_MD1, OUTPUT);
  pinMode(RECEIVER_AUX, INPUT);

  Serial.begin(115200);
  LoRaSender.begin(9600);
  LoRaReceiver.begin(9600);

  LoRaSetMode(SENDER_MD0, SENDER_MD1, SLEEP);
  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, SLEEP);
  LoRaSendSetCmd(LoRaSender, SET_SETTINGS, SENDER_ADDH, SENDER_ADDL, SENDER_SPEED, SENDER_CHAN, SENDER_OPTION);
  LoRaSendSetCmd(LoRaReceiver, SET_SETTINGS, RECEIVER_ADDH, RECEIVER_ADDL, RECEIVER_SPEED, RECEIVER_CHAN, RECEIVER_OPTION);
  //LoRaSetMode(SENDER_MD0, SENDER_MD1, SAVE);
  //LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, SAVE);
  LoRaSetMode(SENDER_MD0, SENDER_MD1, NORMAL);
  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, NORMAL);
}

void loop() {
  // Check working mode
  senderLastAux = senderAux;
  senderAux = digitalRead(SENDER_AUX);

  if (senderAux != senderLastAux) {
    if (senderAux == HIGH) {
      Serial.println("[SENDER] AUX Rising...");
    } else {
      Serial.println("[SENDER] AUX Falling...");
    }
  }

  receiverLastAux = receiverAux;
  receiverAux = digitalRead(RECEIVER_AUX);

  if (receiverAux != receiverLastAux) {
    if (receiverAux == HIGH) {
      Serial.println("[RECEIVER] AUX Rising...");
    } else {
      Serial.println("[RECEIVER] AUX Falling...");
    }
  }

  // Send data
  if (Serial.available()) {
    if (!sending) {
      Serial.println("[SENDER] Start sending...");
      sending = 1;
      //LoRaSetMode(SENDER_MD0, SENDER_MD1, WAKEUP);
    }
    byte b = Serial.read();
    LoRaSender.write(b);
  } else {
    if (sending) {
      Serial.println("[SENDER] Stop sending.");
      LoRaSender.flush();
      sending = 0;
      //LoRaSetMode(SENDER_MD0, SENDER_MD1, SAVE);
    }
  }

  // Receive data
  if (LoRaReceiver.available()) {
    Serial.print("[RECEIVER] ");
    byte b = LoRaReceiver.read();
    Serial.print(b);
  }
}
