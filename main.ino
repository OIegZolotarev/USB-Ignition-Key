#include <Joystick.h>

// Create the Joystick
Joystick_ Joystick;


const int buttonStartingPin = 2;
const int numButtons = 7;

void setup() {
	
  int start = buttonStartingPin;
  int end = buttonStartingPin + numButtons;


  for (int i = start; i < end; i++)
	  pinMode(i, INPUT_PULLUP);
  

	// Initialize Joystick Library
	Joystick.begin();
}

// Last state of the button
int lastButtonState = -1;

#define PIN_STARTER 2
#define PIN_IGNITION 3

int readButtons()
{
  int r = 0;

  int start = buttonStartingPin;
  int end = buttonStartingPin + numButtons;

  for (int i = start; i < end; i++)
	{
    int currentButtonState = !digitalRead(i);
    r |= currentButtonState << i;
  }

  const bool hasStarter = r & 1<< PIN_STARTER;
  const bool hasIgnition = r & 1<< PIN_IGNITION;

  if (hasStarter)
  {
    r &= ~ (1<<PIN_IGNITION);
  }

  // Нажать виртуальную кнопку выключения
  // if (!(hasStarter || hasIgnition))
  //  r |= 1;

  return r;
}




void loop() {

	// Read pin values
	int currentButtonState = readButtons();
	if (currentButtonState != lastButtonState)
	{

    for (int i = 0; i < numButtons; i++)
	  {
      const int state = (currentButtonState & (1<<i)) > 0;
      Joystick.setButton(i, state);
    }
	  lastButtonState = currentButtonState;
	}

  // pseudo-debounce
	delay(50);
}