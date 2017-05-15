void sequenceMode() {
  tempoKnob = mapFloat(analogRead(A7), 0, 1023.0, 1.0, 60.0); //Gets basic tempo value ranging from 1 to 240bpm
  sequencerSemitone = map(analogRead(A6), 0, 1023, 0, 12); //Checks to see what the current semitone value is selected
  tempoInMillis = (tempoKnob / 60.0) * 1000; //Converts tempo value to miliseconds


  //The eight buttons are used for switching steps on the sequencer on/off
  if (button0.wasPressed()) {

    shiftTone(0);
    stepOnOff[0] = !stepOnOff[0];

  }

  if (button1.wasPressed()) {

    shiftTone(1);
    stepOnOff[1] = !stepOnOff[1];

  }

  if (button2.wasPressed()) {

    shiftTone(2);
    stepOnOff[2] = !stepOnOff[2];
    
  }

  if (button3.wasPressed()) {

    shiftTone(3);
    stepOnOff[3] = !stepOnOff[3];
    
  }

  if (button4.wasPressed()) {

    shiftTone(4);
    stepOnOff[4] = !stepOnOff[4];
    
  }

  if (button5.wasPressed()) {
    
    shiftTone(5);
    stepOnOff[5] = !stepOnOff[5];
    
  }

  if (button6.wasPressed()) {

    shiftTone(6);
    stepOnOff[6] = !stepOnOff[6];
    
  }

  if (button7.fallingEdge()) { //Uses digital push button instead

    shiftTone(7);
    stepOnOff[7] = !stepOnOff[7];
    
  }

//Checks to see if the step is on off, and turns the LEDs on/off 
//depending on the value that is true
  for (int i = 0; i < 8; i++) {
    
    if (stepOnOff[i]) { 
      digitalWrite(i, HIGH);
    } else if (i != sequenceStep && !stepOnOff[i]) {
      digitalWrite(i, LOW);
    }
    
  }

  if (sequenceTime > tempoInMillis) {

    if (sequenceStep < 7) {
      sequenceStep++;
    } else {
      if (!stepOnOff[sequenceStep]) digitalWrite(sequenceStep, LOW);
      sequenceStep = 0;
    }

    digitalWrite(sequenceStep, HIGH);
    if (!stepOnOff[sequenceStep - 1]) digitalWrite(sequenceStep - 1, LOW);

    for (int i = 0; i < 8; i++){
    Serial.print("Wave ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(stepSound[0][i]);
  }

    sequenceTime = 0;
  }

  if (stepOnOff[sequenceStep]) { //Plays sound for the specific step at the present moment in time
    waveform1.frequency(stepSound[sequenceStep][0]);
    sine2.frequency(stepSound[sequenceStep][1]);
    waveform2.frequency(stepSound[sequenceStep][2]);
    sine4.frequency(stepSound[sequenceStep][3]);
    waveform3.frequency(stepSound[sequenceStep][4]);
    sine6.frequency(stepSound[sequenceStep][5]);
    waveform4.frequency(stepSound[sequenceStep][6]);
    sine8.frequency(stepSound[sequenceStep][7]);
    //digitalWrite(sequenceStep, HIGH);
  } else { //Makes teensy silent if the select has not been selected
    waveform1.frequency(0);
    waveform2.frequency(0);
    waveform3.frequency(0);
    waveform4.frequency(0);

    sine2.frequency(0);
    sine4.frequency(0);
    sine6.frequency(0);
    sine8.frequency(0);
  }

}

//Applies the semitonal change for a selected step in the sequencer
void changeFreq(int difference, int stepValue) {

  for (int i = 0; i < 8; i++) {
    if (difference > 0) {
      for (int j = 0; j < toneDifference; j++) {
        stepSound[stepValue][i] *= 1.059;
      }
    } else if (difference < 0) {
      for (int j = 0; j > toneDifference; j--) {
        stepSound[stepValue][i] /= 1.059;
      }
    }
  }

}

//Takes the value of the step number, and shifts the frequencies of that step
//varying upon what the difference between the potentiometer input and the 
//current set value of the step's semitone is
void shiftTone(int stepNumber) {
  if (currentTone[stepNumber] != sequencerSemitone) {
    toneDifference = sequencerSemitone - currentTone[stepNumber];
    changeFreq(toneDifference, stepNumber);
    currentTone[stepNumber] += toneDifference;
    modeToneDiff[stepNumber] = sequencerSemitone;
  }
}
