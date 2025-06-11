const int data = 4;
const int latch = 5;
const int clock = 6;

const int latchTick = 200;
const int clkHalfTick = 150;
const int TICK = 10;

const int BT_UP = 1 << 3;
const int BT_DOWN = 1 << 2;
const int BT_LEFT = 1 << 1;
const int BT_RIGHT = 1 << 0;

const int BT_HAZARD = 1 << 7;


void init_joystick()
{
    pinMode(data, INPUT_PULLUP);
    pinMode(clock, OUTPUT);
    pinMode(latch, OUTPUT);

    digitalWrite(clock, HIGH);
}

int get_keys_state_joystick()
{
    digitalWrite(latch, HIGH);
    delayMicroseconds(latchTick);
    digitalWrite(latch, LOW);

    int keys_state = 0;

    for (int i = 0; i < 8; ++i) {
        delayMicroseconds(clkHalfTick);
        digitalWrite(clock, LOW);
        delayMicroseconds(12);  // Ждём стабилизации DATA

        keys_state <<= 1;
        keys_state += digitalRead(data);

        delayMicroseconds(clkHalfTick);
        digitalWrite(clock, HIGH);
    }

    return keys_state;
}

void initFamicloneJoystick()
{
    init_joystick();

    Serial.begin(115200);
}

// Без подключенного геймпада - все кнопки будут нажаты
// Отталкиваясь от этого можно определить что у нас нажата вся крестовина (что невозможно на обычном геймпаде)
// и предположить что геймпад не подключен
bool hasImpossibleKeyStrokes(int result)
{
    int mask = (BT_LEFT | BT_RIGHT | BT_UP | BT_DOWN);
    return (result & mask) == mask;
}

int updateFamicloneJoystick()
{
    int keys = get_keys_state_joystick();
    int result = 0;

    for (int i = 0 ; i < 8; i++)  
    {
        int state = keys & (1 << i);        
        result |= !state << i;
    }    

    if (hasImpossibleKeyStrokes(result))
      result = 0;

    return result;
}