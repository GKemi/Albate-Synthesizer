#include <Audio.h>
#include <Wire.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
//Code generated through the use of the teensy audio library: https://www.pjrc.com/teensy/gui/index.html
AudioSynthWaveformSine   sine2;          //xy=439.3333435058594,118
AudioSynthWaveform       waveform1;      //xy=440,67
AudioSynthWaveform       waveform2;      //xy=443,193
AudioSynthWaveformSine   sine4;          //xy=445.6666564941406,257.6667022705078
AudioSynthWaveformSine   sine8;          //xy=456.6666679382324,489.9999990463257
AudioSynthWaveformSine   sine6;          //xy=458.3333320617676,388.3333225250244
AudioSynthWaveform       waveform4;      //xy=463,446
AudioSynthWaveform       waveform3;      //xy=466,339
AudioEffectMultiply      multiply1;      //xy=663.6666870117188,96.00001525878906
AudioEffectMultiply      multiply2;      //xy=670.0000610351562,238.33334350585938
AudioEffectMultiply      multiply3;      //xy=670.0000610351562,359.33331298828125
AudioEffectMultiply      multiply4;      //xy=695.6666870117188,483
AudioMixer4              mixer1;         //xy=844.4285278320312,195
AudioEffectBitcrusher    bitcrusher1;    //xy=969.9999923706055,288.5714063644409
AudioMixer4              mixer3;         //xy=1032.8571319580078,189.9999771118164
AudioFilterStateVariable filter2;        //xy=1180.0000839233398,242.8571491241455
AudioMixer4              filterMixer;         //xy=1326.1427841186523,233.85712814331055
AudioOutputI2S           i2s1;           //xy=1524.619068145752,306.04763412475586
AudioMixer4              mixer2; //xy=1564.285888671875,164.85702514648438
AudioEffectDelay         delay1;         //xy=1754.5714111328125,149
AudioConnection          patchCord1(sine2, 0, multiply1, 1);
AudioConnection          patchCord2(waveform1, 0, multiply1, 0);
AudioConnection          patchCord3(waveform2, 0, multiply2, 0);
AudioConnection          patchCord4(sine4, 0, multiply2, 1);
AudioConnection          patchCord5(sine8, 0, multiply4, 1);
AudioConnection          patchCord6(sine6, 0, multiply3, 1);
AudioConnection          patchCord7(waveform4, 0, multiply4, 0);
AudioConnection          patchCord8(waveform3, 0, multiply3, 0);
AudioConnection          patchCord9(multiply1, 0, mixer1, 0);
AudioConnection          patchCord10(multiply2, 0, mixer1, 1);
AudioConnection          patchCord11(multiply3, 0, mixer1, 2);
AudioConnection          patchCord12(multiply4, 0, mixer1, 3);
AudioConnection          patchCord13(mixer1, 0, mixer3, 0);
AudioConnection          patchCord14(mixer1, bitcrusher1);
AudioConnection          patchCord15(bitcrusher1, 0, mixer3, 1);
AudioConnection          patchCord16(mixer3, 0, filter2, 0);
AudioConnection          patchCord17(mixer3, 0, filterMixer, 0);
AudioConnection          patchCord18(filter2, 0, filterMixer, 1);
AudioConnection          patchCord19(filter2, 1, filterMixer, 2);
AudioConnection          patchCord20(filter2, 2, filterMixer, 3);
AudioConnection          patchCord21(filterMixer, 0, mixer2, 0);
AudioConnection          patchCord22(mixer2, delay1);
AudioConnection          patchCord23(mixer2, 0, i2s1, 0);
AudioConnection          patchCord24(mixer2, 0, i2s1, 1);
AudioConnection          patchCord25(delay1, 0, mixer2, 1);
AudioControlSGTL5000     audioShield;     //xy=996.5713844299316,424.1904306411743
// GUItool: end automatically generated code

//Bounce library Debounces values read by push buttons
#include <Bounce.h>
Bounce modeButton = Bounce(12, 15);
Bounce button7 = Bounce (14, 15);

//Library used for debouncing push buttons used through the analog pin
//http://www.eduardtiron.com/2015/08/analogpushbutton/
#include <AnalogButton.h>

//Debounced analog pushbuttons
AnalogButton button0(1000, 1023, 15);
AnalogButton button1(700, 800, 15);
AnalogButton button2(450, 500, 15);
AnalogButton button3(260, 290, 15);
AnalogButton button4(130, 160, 15);
AnalogButton button5(65, 95, 15);
AnalogButton button6(30, 55, 15);

int currentWave = 0; //Keeps track of the currently selected waveform
int waveFrequency[8]; //Maintains frequencies for the the individual waveforms in regular mode
int currentFilter = 0; //is used to keep track of what form of filter is selected
boolean delayOn = false; //Boolean used for switching delay on and off
boolean filterFreq = true; //Boolean used for switching knob between cutoff and resonance
boolean sequence = false; //Boolean used for determining which mode the synthesizer operates in
boolean waveSelect = false; //Used for determining if the waveform selection button was pressed
boolean stepOnOff[8]; //Keeps track of the 8 steps in the sequencer
int distortOn = 16, distortionCounter = 0; //Used for the bitcrusher
int filtKnob; //Used for storing cutoff frequency value
float filtKnobFloat; //Used for storing the resonance value
float tempoKnob; //Stores tempo value, retrieved from the potentiometer
elapsedMillis sequenceTime; //Used to make the sequencer operational
elapsedMillis blinkTime; //Used for blinking LEDs
boolean blinkOn = false; //Used for blinking
int sequenceStep; //Keeps track of the current step in the sequencer
float tempoInMillis; //Holds the converted value of the tempo as a millisecond value
float stepSound[8][8]; //Holds individual frequencies of all the waveforms, for each step in the sequencer
int sequencerSemitone, currentTone[8], toneDifference = 0, modeToneDiff[8]; //Used for calculating and applying the equal temperament values onto the sound in the sequencer
boolean presetSelect = false; //Used for checking of 'Preset selection' is on
int presetNumber; //Keeps track of the number for the preset that is applied
int buttonValue; //Used for reading the analog button value(s)


void setup() {
  Serial.begin(9600);
  AudioMemory(140);
  audioShield.enable();

  setupPresets();

  //Sets the output pins for the LEDs
  for (int i = 0; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  //Input for pushbuttons that couldn't be connected through
  //the analog resistance ladder
  pinMode(12, INPUT);
  pinMode(14, INPUT);

  //Code for setting up the waveforms
  waveform1.begin(0.5, 100, WAVEFORM_SAWTOOTH);
  waveform2.begin(0.5, 100, WAVEFORM_SQUARE);
  waveform3.begin(0.5, 100, WAVEFORM_TRIANGLE);
  waveform4.begin(0.5, 100, WAVEFORM_SAWTOOTH);

  sine2.amplitude(0.5);
  sine4.amplitude(0.5);
  sine6.amplitude(0.5);
  sine8.amplitude(0.5);

  sine2.frequency(100);
  sine4.frequency(100);
  sine6.frequency(100);
  sine8.frequency(100);

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer1.gain(2, 0.5);
  mixer1.gain(3, 0.5);

  mixer2.gain(1, 0.5);
  mixer2.gain(2, 0.5);

  mixer3.gain(0, 0);
  mixer3.gain(1, 0.5);

  filterMixer.gain(0, 0.5);
  filterMixer.gain(1, 0);
  filterMixer.gain(2, 0);
  filterMixer.gain(3, 0);

  //Keeps track of the current step in the sequencer
  sequenceStep = 0;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      stepSound[i][j] = 0;
      stepOnOff[i] = false;
      currentTone[i] = 0;
    }
  }

  applyPreset(1);

}

void loop() {
  buttonValue = analogRead(A2);
  buttonUpdate(buttonValue);

  //Controls the volume for the device
  float volumeKnob = mapFloat(analogRead(A1), 0, 1023, 0, 1.0);
  audioShield.volume(volumeKnob);
  if (modeButton.fallingEdge()) { //Button switches between sequencer and regular mode
    sequence = !sequence;
    sequenceTime = 0;

    
    //Applies new settings when the mode is switched to sequencer mode
    if (sequence) {
      sequenceStep = 0; //Sets the position of the sequencer to 0
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          //updates the frequencies of each waveform for each step
          stepSound[i][j] = waveFrequency[j];
          for (int k = 0; k < modeToneDiff[i]; k++){
            //Ensures semitonic differences are maintained, even if the main frequencies
            //have changed after swiching modes
            stepSound[i][j] *= 1.059; 
                                      
          }
        }
      } 

    }

  }

  if (currentFilter == 0) {
    filterMixer.gain(1, 0);
    filterMixer.gain(2, 0);
    filterMixer.gain(3, 0);
    filterMixer.gain(0, 0.5);
  } else if (currentFilter == 1) {
    filterMixer.gain(0, 0);
    filterMixer.gain(2, 0);
    filterMixer.gain(3, 0);
    filterMixer.gain(1, 0.5);
  } else if (currentFilter == 2) {
    filterMixer.gain(0, 0);
    filterMixer.gain(1, 0);
    filterMixer.gain(3, 0);
    filterMixer.gain(2, 0.5);
  } else if (currentFilter == 3) {
    filterMixer.gain(0, 0);
    filterMixer.gain(1, 0);
    filterMixer.gain(2, 0);
    filterMixer.gain(3, 0.5);
  }

  if (!sequence) regularMode(); else sequenceMode(); //Switches between regular and sequencer mode

  if (delayOn) { //Turns delay on/off
    delay1.delay(0, 425);
  } else {
    delay1.disable(0);
  }

  
}

//Alternative mapping function that returns a floating point value
//Taken from: https://forum.arduino.cc/index.php?topic=3922.0
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Function used for updating the button values
void buttonUpdate(int val) {
  modeButton.update();
  button7.update(); 

  button0.update(val);
  button1.update(val);
  button2.update(val);
  button3.update(val);
  button4.update(val);
  button5.update(val);
  button6.update(val);
  
}
