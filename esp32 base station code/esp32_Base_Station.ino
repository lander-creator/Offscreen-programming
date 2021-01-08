/*
 Record and Replay of the recorded sequence with 4 rfid Tags.

 INSTRUCTIONS
 1, show the tags in the sequence you want, It will be automatically saved.
 2, The default maximum steps are 10. 
 3, if you want to play the recorded sequence press the play button. (the memory led turns on)
 4, the recording will be saved until you press the reset button.(red will flash and memory led turn off)
 5, Now you can play a new sequence.
*/
// library
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>
//-------------------------------------------------------------------------------
//                              RFID
#define SS_PIN 21
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
//-------------------------------------------------------------------------------
//                              WiFi
const char* ssid = "Robot-WiFi";   // name of your WiFi network
const char *password = "123456789"; // password of the WiFi network
//-------------------------------------------------------------------------------
//                         MQTT ID & TOPIC
const char *ID = "Robot";  // Name of our device, must be unique
const char *TOPIC1 = "controlBoard1";  // Topic to subcribe to
//-------------------------------------------------------------------------------
//                            broker IP
IPAddress broker(192,168,0,1); // IP address of your MQTT broker eg. 192.168.1.50
WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT client
//-------------------------------------------------------------------------------
                            //GPIO-PINS
//input 
const int playbutton = 26; //play the array
const int resetbutton = 25; //reset entered sequence
//output
const int LED[] = {33,14,13,32}; //Control led input(33=voor, 14=achter, 13=links, 32=rechts)
const int memoryLed = 27; //green LED is pin 27
const int Red = 12; //red LED is on pin 12
//-------------------------------------------------------------------------------
//                     Maximum steps for recording
int MAXStep = 100; //MAXStep & entered must be the same number.
int entered[100];              
//-------------------------------------------------------------------------------
//                            Variables
void checkEntered1(int button);                
int buttonPushCounter = 0;   // counter for the number of button presses
int maximumSteps = 0;
int stepnumber = 0;
int replay = 0;
int one = 1;
//-------------------------------------------------------------------------------
                
void setup(){ //run once at sketch startup
  Serial.begin(9600); //begin Serial
  
  // RFID setup
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read

  pinMode(playbutton, INPUT); //playbutton is an input
  digitalWrite(playbutton,HIGH);
  pinMode(resetbutton, INPUT); //resetbutton is an input
  digitalWrite(resetbutton,HIGH);
  pinMode(Red, OUTPUT); //the red LED is an output
  pinMode(memoryLed, OUTPUT); // the memoryLed is an output
  digitalWrite(Red, LOW); //turn the red LED on
  for (int i = 0; i < 9;i++){   //work through the first 10 steps 
    Serial.println(entered[i]); //print 10 elements of the entered[]
                                //Allow the control leds to work properly 
    pinMode(LED[i],OUTPUT);
  }
  //run setup wifi
  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
}

void loop(){
  // mqtt (re)connect
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
  buttonfunctie();
  rfid();
}

void rfid(){
// begin RFID
// Look for new cards
 // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
   
  byte block;
  byte len;
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  } 
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  } 
  Serial.println(F("**Card Detected:**")); 
  byte buffer1[18]; 
  block = 4;
  len = 18;
 
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  } 
  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
   
  String value = "";
  for (uint8_t i = 0; i < 1; i++) //Looks only to the first caracter in the message.
  {                               //If the message on the card has more caracters replace the 1.
      value += (char)buffer1[i];    
  }
  value.trim();
  Serial.println(value); 
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  if (value == "1"){ //if button1 is pressed
    StepRecording(1); //call checkEntered and pass it a 1
    stepnumber ++;  
    delay(250);//wait, needed for correct functioning, otherwise
  }
  else if (value == "2"){ //if button2 is pressed
    StepRecording(2); //call checkEntered1 and pass it a 2
    stepnumber ++;   
    delay(250); //wait  
  }
  else if (value == "3"){ //if button3 is pressed
    StepRecording(3); //call checkEntered1 and pass it a 3
    stepnumber ++;   
    delay(250); //wait  
  }
  else if (value == "4"){ //if button4 is pressed
    StepRecording(4); //call checkEntered1 and pass it a 4
    stepnumber ++;  
    delay(250); //wait  
  }
   Serial.println(F("\n**End Reading**\n"));
}
