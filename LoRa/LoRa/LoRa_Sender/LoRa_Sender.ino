#include <SimpleTimer.h>

// Pin settings
#define MD0 7
#define MD1 6
#define AUX 3
#define LoRa Serial1

// LoRa modes
#define NORMAL 0
#define WAKEUP 1
#define SAVE 2
#define SLEEP 3

// LoRa settings
byte ADDH = 0x12;
byte ADDL = 0x30;
byte SPEED = 0x1A;
byte CHAN = 0x17;
byte OPTION = 0x44;

// LoRa commands
byte SET_SETTINGS_PERM[] = { 0xC0, ADDH, ADDL, SPEED, CHAN, OPTION }; // write to flash
byte SET_SETTINGS[] = { 0xC2, ADDH, ADDL, SPEED, CHAN, OPTION }; // write temporarily

byte READ_SETTINGS[] = { 0xC1, 0xC1, 0xC1 };
byte READ_VERSION_NO[] = { 0xC3, 0xC3, 0xC3 };
byte RESET[] = { 0xC4, 0xC4, 0xC4 };
byte READ_VOLTAGE[] = { 0xC5, 0xC5, 0xC5 };

// LoRa receiver information
byte RECEIVER_ADD[2] = { 0xff, 0xff };

// Utilities
#define END_OF_BUFFER '\0'

SimpleTimer timer;
int sendDataTimerId;

// Global variables
char c;
int flag = 0;
int aux = HIGH, lastAux;
byte dataBuffer[512];

/* Helpers */

void parseData() {
  dataBuffer[0] = 0x11; dataBuffer[1] = 0x22; dataBuffer[2] = 0x33; 
  dataBuffer[3] = END_OF_BUFFER;
}

void LoRaSetMode(int mode) {
  digitalWrite(MD0, mode % 2);
  digitalWrite(MD1, mode / 2);
}

void LoRaSendReadCmd(byte cmd[]) {
  LoRa.write(cmd, sizeof(cmd));
}

void LoRaSendSetCmd(byte cmd[], byte addh, byte addl, byte sp, byte chan, byte option) {
  cmd[1] = addh; cmd[2] = addl; cmd[3] = sp; cmd[4] = chan; cmd[5] = option;
  LoRa.write(cmd, sizeof(cmd));
}

void LoRaSendData() {
  Serial.println("Parsing data...");
  parseData();
  
  LoRaSetMode(WAKEUP);
  Serial.print("Sending data to "); Serial.print(RECEIVER_ADD[0], HEX); Serial.print(RECEIVER_ADD[1], HEX); Serial.println("...");
  
  LoRa.write(RECEIVER_ADD, sizeof(RECEIVER_ADD));
  for (int i = 0; dataBuffer[i] != END_OF_BUFFER; i++) {
    LoRa.write(dataBuffer[i]);
    Serial.print(dataBuffer[i], HEX);  
  }
  Serial.println();
  
  LoRaSetMode(SAVE);
}

/* Main */

void setup() {
  pinMode(MD0, OUTPUT);
  pinMode(MD1, OUTPUT);
  pinMode(AUX, INPUT);

  Serial.begin(115200);
  LoRa.begin(9600);

  LoRaSetMode(SLEEP);
  LoRaSendSetCmd(SET_SETTINGS, ADDH, ADDL, SPEED, CHAN, OPTION);
  LoRaSetMode(SAVE);

  sendDataTimerId = timer.setInterval(10000, LoRaSendData);
}

void loop() {
  timer.run();
  
  // Check working mode
  lastAux = aux;
  aux = digitalRead(AUX);

  if (aux != lastAux) {
    if (aux == HIGH) {
      Serial.println("\nAUX Rising...");
    } else {
      Serial.println("AUX Falling...");
    }
  }

  // Receive data
  if (LoRa.available()) {
    c = LoRa.read();
    Serial.print(c, HEX);
  }
}
