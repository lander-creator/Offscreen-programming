void StepRecording(int button){ //check the first element of the entered[] array
  if (replay == 1 || maximumSteps == 1){
    digitalWrite(LED[button-1],LOW);} 
  else{
    digitalWrite(LED[button-1],HIGH);
    delay(200);
    digitalWrite(LED[button-1],LOW);
    if (entered[stepnumber] != 0){ //if it is not a zero, i.e. it has already been inputted  
      }
    if(entered[stepnumber] == 0){ //if it is zero, i.e. if it hasn't been defined with a button yet
      entered[stepnumber] = button; //set the first element as the button that has been pressed
      Serial.print(stepnumber);Serial.print(": ");Serial.println(entered[stepnumber]); //for debugging
    } 
  }
  if (stepnumber == MAXStep){
    maximumSteps = 1;
    Serial.print("MAX steps ");
  }
}

void Replay(){
  digitalWrite(memoryLed,HIGH);
  replay = 1;
  for (int i = 0; i < stepnumber; i++){ //these lines play the recorded steps
    Serial.println(entered[i]);
    delay(10);
  
    if ((entered[i]== 1)){vooruit();}
    else if ((entered[i]== 2)){achteruit();}
    else if ((entered[i]== 3)){links();}
    else if ((entered[i]== 4)){rechts();}
    if (digitalRead(resetbutton) == HIGH){
      entered_reset();
      break;
    }
  }
   
  loop(); //return to loop() (not really necessary)
  close_all();
  } 
