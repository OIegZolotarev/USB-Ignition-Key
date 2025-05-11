// Константы
const int lockFirstPin = 2;
const int lockNumButtons = 2;

const int PIN_IGNITION = 2;
const int PIN_STARTER = 3;


int lastIgnitionKeyState = 0;

void setupIgnitionKey() {
  int start = lockFirstPin;
  int end = start + lockNumButtons;

  for (int i = start; i < end; i++)
    pinMode(i, INPUT_PULLUP);
}

int readIgnitionKey() {
  int r = 0;

  int start = lockFirstPin;
  int end = lockFirstPin + lockNumButtons;

  for (int i = start; i < end; i++) {
    int currentButtonState = !digitalRead(i);
    r |= currentButtonState << (i);
  }

  const bool hasStarter = r & 1 << PIN_STARTER;
  const bool hasIgnition = r & 1 << PIN_IGNITION;

  // Выключаем кнопку зажигания если нажат стартер
  // Чтобы для компьютера выглядело как разные нажатия кнопок (фикс для Beam.NG)
  if (hasStarter) {
    r &= ~(1 << PIN_IGNITION);
  }

  // Сдвиг битов вправо чтобы нумерация кнопок пошла с 0
  return r >> lockFirstPin;
}

int updateIgnitionKey(int buttonOffset) {

  int result = 0;

  int currentButtonState = readIgnitionKey();

  for (int i = 0; i < lockNumButtons; i++) {

    int state = (currentButtonState & (1 << i)) > 0;
    result |= state << (buttonOffset + i);
  }

  lastIgnitionKeyState = currentButtonState;

  return result;
}