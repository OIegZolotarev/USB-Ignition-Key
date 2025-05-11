#include <Joystick.h>

const int numButtons = 10;

// Интерфейс джойстика и связанное
Joystick_ Joystick;

int lastButtonState = 0;

byte debouncer[numButtons] = {0};
byte pseudoPush[numButtons] = {0};

void setup() {
  setupIgnitionKey();
  initFamicloneJoystick();
  Joystick.begin();
}



void loop() {

  for (int i = 0 ; i < numButtons; i++)
  {
      if (pseudoPush[i] > 0)
      {
        Joystick.setButton(i, 1);
        pseudoPush[i]--;
      }
  }

  int famicomKeys = updateFamicloneJoystick();
  int ignitionKeys = updateIgnitionKey(0);

  int muxed = famicomKeys | ignitionKeys;

  for (int i = 0; i < numButtons; i++)
  { 
    int currentState = (muxed & 1 << i) > 0;
    int lastState = (lastButtonState & 1 << i);

    // Если поменялось состояние кнопки
    if (lastState != currentState)
    {
        // Если дебаунсер не держит кнопку
        if (debouncer[i] == 0)
        {
            // Устанавливаем новое состояние кнопки и включаем дебаусер
            debouncer[i] = 5;
            Joystick.setButton(i, currentState);

            if (currentState)
              lastButtonState |= 1 << i;
            else 
              lastButtonState &= ~(1 << i);
        }                
    }

    // отпускаем дебаунсер потихоньку
    if (debouncer[i] > 0) 
      debouncer[i]--;              
  }
 
  delay(50);
}