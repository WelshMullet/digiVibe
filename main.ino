
const int mosfetPin = 0;         // must be a pwm capable pin. Attach to mosfet.
const int buttonPin = 2;    // the number of the pushbutton pin

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

struct VALUE_AND_TIME {
  byte s;
  byte t;
};

VALUE_AND_TIME* currentPattern;
volatile int currentPatternNumber;
int currentStep;

int numberOfPatterns = 5;
unsigned long timer;

struct VALUE_AND_TIME pattern1[] = {
  {50,1000},
  {100,1000},
  {150,1000},
  {200,1000},
  {250,1000},
  {200,1000},
  {150,1000},
  {100,1000},
  {50,1000},
  {0,1000}
};

struct VALUE_AND_TIME pattern2[] = {
  {50,1000}
};

struct VALUE_AND_TIME pattern3[] = {
  {100,1000}
};

struct VALUE_AND_TIME pattern4[] = {
  {150,1000}
};

struct VALUE_AND_TIME pattern5[] = {
  {200,1000}
};

VALUE_AND_TIME* patterns[] = {pattern1,pattern2,pattern3,pattern4,pattern5};

void doVibe(VIBE vibe)
{
  timer = millis();

  while(millis() < timer + vibe.t)
  {
    // Take value from 255 as the mosfet 'inverts' it
      analogWrite(mosfetPin,255 - vibe.s);
  }

}

void setup() {
  // put your setup code here, to run once:
  pinMode(mosfetPin, OUTPUT);

  pinMode(buttonPin, INPUT); // set INT0 pin to input
  attachInterrupt(buttonPin, incrementPattern, FALLING ); // attach INT0 interrupt
  
  currentStep = 0;
  currentPattern = patterns[0];
}

void loop() {
  // put your main code here, to run repeatedly: 
  if(currentStep < sizeof(currentPattern)/sizeof(VIBE))
  {
    doVibe(currentPattern[currentStep]);
    currentStep++;
  }
  else
  {
   currentStep = 0;
   doVibe(currentPattern[currentStep]);
  }
}

void on_button_press()
{
   // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        incrementPattern();
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}

void incrementPattern()
{
  if(currentPattern == patterns[numberOfPatterns-1])
  {
    currentPatternNumber = 0;
    currentPattern = patterns[0];
  }
  else
  {
    currentPattern = patterns[++currentPatternNumber];
  }
}

