// Pin settings
#define RECEIVER_MD0 7
#define RECEIVER_MD1 6
#define RECEIVER_AUX 3
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

// Global variables
int senderAux = HIGH, senderLastAux;
int receiverAux = HIGH, receiverLastAux;
int sending = 0;

/* Helpers */

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
  pinMode(RECEIVER_MD0, OUTPUT);
  pinMode(RECEIVER_MD1, OUTPUT);
  pinMode(RECEIVER_AUX, INPUT);

  LoRaReceiver.begin(9600);

  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, SLEEP);
  LoRaSendSetCmd(LoRaReceiver, SET_SETTINGS, RECEIVER_ADDH, RECEIVER_ADDL, RECEIVER_SPEED, RECEIVER_CHAN, RECEIVER_OPTION);
  LoRaSetMode(RECEIVER_MD0, RECEIVER_MD1, NORMAL);
}

void loop() {
  // Check working mode
  receiverLastAux = receiverAux;
  receiverAux = digitalRead(RECEIVER_AUX);

//  if (receiverAux != receiverLastAux) {
//    if (receiverAux == HIGH) {
//      Serial.println("[RECEIVER] AUX Rising...");
//    } else {
//      Serial.println("[RECEIVER] AUX Falling...");
//    }
//  }

  // Receive data
  if (LoRaReceiver.available()) {
    //Serial.print("[RECEIVE] ");
    byte b = LoRaReceiver.read();
    //Serial.print(b);

    LoRaReceiver.write(b);
    LoRaReceiver.flush();
  }
}
