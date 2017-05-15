void regularMode() {
  
  if (blinkTime > 150) {
    blinkTime = 0;

    if (waveSelect) {
      blinkOn = !blinkOn;

      if (blinkOn) {
        digitalWrite(currentWave, HIGH);
      } else {
        digitalWrite(currentWave, LOW);
      }
    } else if (presetSelect){
      blinkOn = !blinkOn;

      if (blinkOn) {
        for (int i = 0; i < 8; i++) digitalWrite(i, HIGH);
      } else {
        for (int i = 0; i < 8; i++) digitalWrite(i, LOW);
      }
    
    } else {
      
      for (int i = 0; i < 8; i++) {
        if (i < currentWave || i > currentWave) {
          digitalWrite(i, LOW);
        } else {
          digitalWrite(currentWave, HIGH);
        }
      }
      
    }
  }

  //If statements that consistently check the state of each button 

  if (button0.wasPressed()) {
    if (waveSelect) {  //Selec
      currentWave = 0;
      waveSelect = false;
    } else if (presetSelect){ //Applies preset 1 if preset selection is active
      presetNumber = 1;
      applyPreset(presetNumber);
      presetSelect = false;
    } else {
      waveSelect = true; //if none of the previous conditions are met, wave selection is turned on
    }

  }

  if (button1.wasPressed()) {

    if (waveSelect) {
      currentWave = 1;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 2;
      presetSelect = false;
    } else {
      delayOn = !delayOn;
    }

  }

  if (button2.wasPressed()) {

    if (waveSelect) {
      currentWave = 2;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 3;
      presetSelect = false;
    } else if (currentFilter != 1){
      currentFilter = 1;
    } else if (currentFilter == 1){
      currentFilter = 0;
    }
    
   
  }

  if (button3.wasPressed()) {
    if (waveSelect) {
      currentWave = 3;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 4;
      presetSelect = false;
    } else if (currentFilter != 2){
      currentFilter = 2;
    } else if (currentFilter == 2){
      currentFilter = 0;
    }

  }

  if (button4.wasPressed()) {
    if (waveSelect) {
      currentWave = 4;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 5;
      presetSelect = false;
    } else if (currentFilter != 3){
      currentFilter = 3;
    } else if (currentFilter == 3){
      currentFilter = 0;
    }

  }

  if (button5.wasPressed()) {
    if (waveSelect) {
      currentWave = 5;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 6;
      presetSelect = false;
    } else {
      filterFreq = !filterFreq;
    }
  }

  if (button6.wasPressed()) {
    if (waveSelect) {
      currentWave = 6;
      waveSelect = false;
    } else if (presetSelect){
      presetNumber = 7;
      presetSelect = false;
    } else {
      if (distortionCounter < 2) {
        distortionCounter++;
      } else {
        distortionCounter = 0;
      }
    }
  }

  if (button7.fallingEdge()) {
    if (waveSelect) {//Selects waveform 7 if in waveform selection mode
      currentWave = 7;
      waveSelect = false;
    } else if (!presetSelect){ //Activates preset selection
      presetSelect = true; 
    } else if (presetSelect){ //Deactivates preset selection if pressed again
      presetSelect = false;
    }
  }

  if (filterFreq) {
    filtKnob = map(analogRead(A3), 0, 1023, 0, 7000);
  } else {
    filtKnobFloat = mapFloat(analogRead(A3), 0, 1023, 0.7, 5.0);
  }

  if (distortionCounter == 0){
    distortOn = 16;
  } else if (distortionCounter == 1){
    distortOn = 4;
  } else {
    distortOn = 0;
  }

  int freqKnob = map(analogRead(A7), 0, 1023, 0, 3000);
  int distKnob = map(analogRead(A6), 0, 1023, 0, 25000);
  filter2.frequency(filtKnob);
  filter2.resonance(filtKnobFloat);
  bitcrusher1.sampleRate(distKnob);
  bitcrusher1.bits(distortOn);

  waveFrequency[currentWave] = freqKnob;
  
  produceSound(); //Outputs the sound
  
}

void produceSound(){
  waveform1.frequency(waveFrequency[0]);
  waveform2.frequency(waveFrequency[2]);
  waveform3.frequency(waveFrequency[4]);
  waveform4.frequency(waveFrequency[6]);

  sine2.frequency(waveFrequency[1]);
  sine4.frequency(waveFrequency[3]);
  sine6.frequency(waveFrequency[5]);
  sine8.frequency(waveFrequency[7]);
}

