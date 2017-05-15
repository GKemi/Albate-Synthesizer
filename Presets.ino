//2D array that is used for storing presets of the synthesizer
//By storing the individual frequencies of each waveform, 
//for 7 of the available presets
int presetFreqs[7][8];


//Used for setting up the presets values when the program initially begins
void setupPresets() {
  //Preset 1
  presetFreqs[0][0] = 100;
  presetFreqs[0][1] = 100;
  presetFreqs[0][2] = 100;
  presetFreqs[0][3] = 100;
  presetFreqs[0][4] = 100;
  presetFreqs[0][5] = 120;
  presetFreqs[0][6] = 110;
  presetFreqs[0][7] = 120;

  
}

//Function that is used for applying a preset 
void applyPreset(int presetNo) {
  for (int currentFrequency = 0; currentFrequency < 8; currentFrequency++) {
    waveFrequency[currentFrequency] = presetFreqs[presetNo-1][currentFrequency];
  }

  //Applies additional changes pertinent to the inputted preset number
  if (presetNumber == 1){ 
    currentFilter = 1;
    filter2.frequency(400);

    delayOn = false;

    distortOn = 0;

    filterFreq = false;
  }
}



