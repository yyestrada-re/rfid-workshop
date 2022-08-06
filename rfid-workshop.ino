/*
 * Initial Author: ryand1011 (https://github.com/ryand1011)
 *
 * Reads data written by a program such as "rfid_write_personal_data.ino"
*/

#include <SPI.h> // SPI device communication library
#include <MFRC522.h> // RFID module library

#define RST_PIN 9           
#define SS_PIN  10   

#define RED_PIN 2
#define GREEN_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);   // creates MFRC522 instance

String myUID = "REPLACE-ME";
unsigned long strUID[4];

void setup() {
  pinMode(RED_PIN, OUTPUT); // declare the LED as an output
  pinMode(GREEN_PIN, OUTPUT); // declare the LED as an output

  Serial.begin(9600); // starts the serial connection
  SPI.begin(); // initiates SPI connection between RFID module and Arduino
  mfrc522.PCD_Init(); // initiates MFRC522 RFID module                                           
  Serial.println("Approximate your card to the reader!");  // PICC = proximity integrated circuit card (contactless card/tag). denotes that it's ready to be read
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  // RFID tag identifiers are always WRITTEN as hexadecimal values to the actual tag
  char str[32] = ""; // UID is 4-bytes or 32-bits
  array_to_string(mfrc522.uid.uidByte, 4, str); // byte array, length, char array for output
  Serial.println(str); // print the output UID string

  if(String(str) == myUID) { // compares both strings for equality
    Serial.println("it's a match!");
    authorized();
  } else {
    Serial.println("incorrect UID!");
    unauthorized();
  }

  Serial.println(F("\n**End Reading**\n"));
  delay(1000); //change value if you want to read cards faster
  mfrc522.PICC_HaltA(); // tells the reader to stop reading
}

// converts from HEX to UTF-8
void array_to_string(byte array[], unsigned int len, char buffer[]) {
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F; 
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

void authorized() {
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
}

void unauthorized() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
}
