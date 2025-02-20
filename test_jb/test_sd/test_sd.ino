#include <SD.h>
#include <SPI.h>
//--------------------------------------------------------------------------------------- 
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7   // Teensy 4 ignores this, uses pin 11
#define SDCARD_SCK_PIN   14  // Teensy 4 ignores this, uses pin 13
//--------------------------------------------------------------------------------------- 
void setup() {
  Sd2Card card;
  File melodyFile;
  char buffer[512];
  boolean status;

  while (!Serial) {
    delay(10); // Attendre que le port série soit prêt
  }

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  Serial.begin(9600);
  Serial.println("------------");
  Serial.println("SD Card Test");
  Serial.println("------------");

  status = card.init(SPI_FULL_SPEED, SDCARD_CS_PIN);
  if (status) {
    Serial.println("SD card is connected :-)");
  } else {
    Serial.println("SD card is not connected or unusable :-(");
    return;
  }

  Serial.println("------------");
  Serial.print("Initialisation de la carte SD...");
  Serial.println("------------");

  status = SD.begin(SDCARD_CS_PIN);
  if (status) {
    Serial.println("SD library is able to access the filesystem");
  } else {
    Serial.println("SD library can not access the filesystem!");
    Serial.println("Please report this problem, with the make & model of your SD card.");
    Serial.println("  http://forum.pjrc.com/forums/4-Suggestions-amp-Bug-Reports");
  }

  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("Échec de l'initialisation !");
    return;
  }
  Serial.println("Initialisation réussie.");
}

void loop() {
}