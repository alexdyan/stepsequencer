int ledPins[] = {4, 6, 8, 10};
int buttonPins[] = {36, 35, 34, 33};
boolean lastButtonStates[] = {LOW, LOW, LOW, LOW};
boolean buttonStates[] = {LOW, LOW, LOW, LOW};
boolean stepState[3][4] = {
  {HIGH, LOW, LOW, LOW},
  {LOW, HIGH, LOW, LOW},
  {LOW, LOW, HIGH, HIGH}
};

int tempo = 500;
int currentStep = 0;
unsigned long lastStepTime = 0;

int button1 = 37;       //these variables are for the forward and backward buttons
int button2 = 38;
boolean buttonState1 = LOW;
boolean buttonState2 = LOW;
boolean lastButtonState1 = LOW;
boolean lastButtonState2 = LOW;
int channel = 0;

void setup() {
  for (int i = 0; i < 4; i++) {       //set the 4 buttons as inputs,
    pinMode(buttonPins[i], INPUT);    //and the 4 leds as outputs
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(button1, INPUT);            //set the 2 toggle buttons as inputs
  pinMode(button2, INPUT);
  Serial.begin(9600);
}

void loop() {
  checkButtons();
  sequence();
  setLeds();
  stepForward();
  stepBackward();
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonStates[i] = buttonStates[i];
    buttonStates[i] = digitalRead(buttonPins[i]);

    if (buttonStates[i] == HIGH && lastButtonStates[i] == LOW) {
        if (stepState[channel][i] == false)
          stepState[channel][i] = true;
        else if (stepState[channel][i] == true)
          stepState[channel][i] = false;
    }
  }
}

void sequence() {
  if (millis() > lastStepTime + tempo) {  //if its time to go to the next step...
    currentStep = currentStep + 1;        //increment to the next step
    if (currentStep > 3)
      currentStep = 0;                    //at the end, go back to the first step
    lastStepTime = millis();              //set lastStepTime to the current time
    for (int j = 0; j < 3; j++) {
      if ((j == 0) && stepState[j][currentStep] == true) {
        Serial.println(j);
        usbMIDI.sendNoteOff(60, 0, 1);
        usbMIDI.sendNoteOn(60, 127, 1);
      }
      else if ((j == 1) && stepState[j][currentStep] == true) {
        Serial.println(j);
        usbMIDI.sendNoteOff(64, 0, 1);
        usbMIDI.sendNoteOn(64, 127, 1);
      }
      else if ((j == 2) && stepState[j][currentStep] == true) {
        Serial.println(j);
        usbMIDI.sendNoteOff(67, 0, 1);
        usbMIDI.sendNoteOn(67, 127, 1);
      }
    }
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {
    if (i == currentStep)
      analogWrite(ledPins[i], 255);     //full brightness
    else if (stepState[channel][i] == true)
      analogWrite(ledPins[i], 50);      //less brightness
    else
      analogWrite(ledPins[i], 0);
  }
}

void stepForward() {
  lastButtonState1 = buttonState1;
  buttonState1 = digitalRead(button1);
  if (buttonState1 == HIGH && lastButtonState1 == LOW) {
    channel++;                    //if you push the button, go to the next channel
    if (channel > 2)              //at the end, go back to the first channel
      channel = 0;
  }
}

void stepBackward() {
  lastButtonState2 = buttonState2;
  buttonState2 = digitalRead(button2);
  if (buttonState2 == HIGH && lastButtonState2 == LOW) {
    channel--;                    //if you push the button, go to the previous channel
    if (channel < 0)              //at the beginning, go back to the last channel
      channel = 2;
  }
}
