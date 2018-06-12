// Pin settings
#define MD0 7
#define MD1 6
#define AUX 3
#define LoRa Serial1

// LoRa modes
#define NORMAL 0
#define WAKE_UP 1
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

// Global variables
char c;
int flag = 0;
int aux = HIGH, lastAux;

/* Helpers */

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

/* Main */

void setup() {
  pinMode(MD0, OUTPUT);
  pinMode(MD1, OUTPUT);
  pinMode(AUX, INPUT);

  Serial.begin(115200);
  LoRa.begin(9600);

  LoRaSetMode(SLEEP);
  LoRaSendSetCmd(SET_SETTINGS, ADDH, ADDL, SPEED, CHAN, OPTION);
}

void loop() {
  lastAux = aux;
  aux = digitalRead(AUX);

  if (aux != lastAux) {
    if (aux == HIGH) {
      Serial.println("\nRising...");
    } else {
      Serial.println("\nFalling...");
    }
  }
  
  if (LoRa.available()) {
    c = LoRa.read();
    Serial.print(c, HEX);
  }
}
