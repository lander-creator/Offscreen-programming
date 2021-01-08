void vooruit(){
  digitalWrite(Red, LOW); // turn the red LED off
  digitalWrite(LED[0],HIGH);
  delay(500); //wait for a bit
  digitalWrite(LED[0],LOW);
  delay(500); //wait for a bit
  char* message = "voor";
  int length = strlen(message);
  boolean retained = true;
  client.publish(TOPIC1,(byte*)message,length,retained);
  Serial.println((String)TOPIC1 + " => V");
}
void achteruit(){
  digitalWrite(Red, LOW); // turn the red LED off
  digitalWrite(LED[1],HIGH);
  delay(500); //wait for a bit
  digitalWrite(LED[1],LOW);
  delay(500); //wait for a bit
  char* message = "achter";
  int length = strlen(message);
  boolean retained = true;
  client.publish(TOPIC1,(byte*)message,length,retained);
  Serial.println((String)TOPIC1 + " => A"); 
}
void links(){
  digitalWrite(Red, LOW); // turn the red LED off
  digitalWrite(LED[2],HIGH);
  delay(500); //wait for a bit
  digitalWrite(LED[2],LOW);
  delay(500); //wait for a bit
  char* message = "links";
  int length = strlen(message);
  boolean retained = true;
  client.publish(TOPIC1,(byte*)message,length,retained);
  Serial.println((String)TOPIC1 + " => L");
}
void rechts(){
  digitalWrite(Red, LOW); // turn the red LED off
  digitalWrite(LED[3],HIGH);
  delay(500); //wait for a bit
  digitalWrite(LED[3],LOW);
  delay(500); //wait for a bit
  char* message = "rechts";
  int length = strlen(message);
  boolean retained = true;
  client.publish(TOPIC1,(byte*)message,length,retained);
  Serial.println((String)TOPIC1 + " => R");
}
void buttonfunctie(){
 if (entered[0] != 0 && digitalRead(playbutton) == HIGH){
    Replay(); //call the Replay function
 }
 else if (entered[0] == 0 && digitalRead(playbutton) == HIGH){
  digitalWrite(memoryLed, HIGH);
  delay(50);
  digitalWrite(memoryLed, LOW);
  delay(50);
  digitalWrite(memoryLed, HIGH);
  delay(50);
  digitalWrite(memoryLed, LOW);
 }
 if (digitalRead(resetbutton) == HIGH){
    entered_reset(); //call the entered_reset function 
 }
}
void entered_reset(){
  digitalWrite(memoryLed,LOW);    
  digitalWrite(Red,HIGH); 
  digitalWrite(Red,LOW);
  delay(100);
  digitalWrite(Red,HIGH);
  delay(100);
  digitalWrite(Red,LOW);
  Serial.println("Red OFF");
  for (int i = 0; i < stepnumber; i++){ //this next loop is for debugging
    entered[i] = 0;
  }
  maximumSteps = 0;
  replay = 0;
  stepnumber = 0;
  char* message = "reset";
  int length = strlen(message);
  boolean retained = true;
  client.publish(TOPIC1,(byte*)message,length,retained);
  Serial.println((String)TOPIC1 + " => RST");    
}

void close_all(){
  digitalWrite(LED[0],LOW);
  digitalWrite(LED[1],LOW);
  digitalWrite(LED[2],LOW);
  digitalWrite(LED[3],LOW);
}
