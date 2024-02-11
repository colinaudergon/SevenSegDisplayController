#include "sevenSegController.h"

SevenSegController::SevenSegController(int shiftDataPin, int shiftClkPin, int shiftLatchPin, int nbrDisplay, bool isCathodCommun)
{
    _shiftDataPin = shiftDataPin;
    _shiftClkPin = shiftClkPin;
    _shiftLatchPin = shiftLatchPin;
    _nbrDisplay = nbrDisplay;
    _nbrDisplayZeroIndexed = nbrDisplay - 1;
    _isCathodCommun = isCathodCommun;
    _maxNbrOfDisplay = 8;
    _sevenSegCharSize = (sizeof(sevenSegChar) / sizeof(sevenSegChar[0]));
    _maxDisplayNumber = pow(2, (_nbrDisplay * 4)) - 1;

    _currentDisplay = 0;

    _timer2Freq = 500;
}

void SevenSegController::begin()
{
    pinMode(_shiftDataPin, OUTPUT);
    pinMode(_shiftClkPin, OUTPUT);
    pinMode(_shiftLatchPin, OUTPUT);

    // Set Timer 2 to CTC (Clear Timer on Compare Match) mode
    TCCR2A = (1 << WGM21);

    // Set the prescaler to 64 (CS22 = 1, CS21 = 1, CS20 = 0)
    TCCR2B = (1 << CS22) | (1 << CS21);

    // Set the compare match value for 500Hz frequency
    OCR2A = F_CPU / (64UL * _timer2Freq) - 1;

    // Enable the Timer 2 output compare A interrupt
    TIMSK2 |= (1 << OCIE2A);

    // Enable global interrupts
    sei();
}

void SevenSegController::drawCharSevenSeg(uint8_t character, uint8_t display)
{
    uint8_t onDisplay = 0x00;
    onDisplay = (1 << display);
    if (_isCathodCommun == false)
    {
        character = ~character;
        onDisplay = ~onDisplay;
    }
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, ~onDisplay);
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, character);
    shiftLatchPulse();
}

void SevenSegController::drawCharAllSevenSeg(uint8_t character)
{
    uint8_t onDisplay = 0xFF;
    if (_isCathodCommun == false)
    {
        character = ~character;
        onDisplay = 0x00;
    }
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, ~onDisplay);
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, character);
    shiftLatchPulse();
}

void SevenSegController::testSevenSegDisplay(void)
{
    for (uint8_t i = 0; i < _sevenSegCharSize; i++)
    {
        drawCharAllSevenSeg(sevenSegChar[i]);
        delay(500);
    }
    displayBlink(2);
}

void SevenSegController::displayNbr()
{
    if (_currentDisplay < _nbrDisplay)
    {
        _currentDisplay++;
    }
    else
    {
        _currentDisplay = 0;
    }
    drawCharSevenSeg(_numberToDisplay[_nbrDisplayZeroIndexed - _currentDisplay], _currentDisplay);
}

void SevenSegController::saveDisplaynumber(uint32_t number)
{
    for (uint8_t displayPos = 0; displayPos < _nbrDisplay; displayPos++)
    {
        _numberToDisplay[displayPos] = splitNumberForDisplay(number, displayPos);
    }
}

void SevenSegController::displayBlink(uint8_t nbrOfBlink)
{
    for (uint8_t blinkCntr = 0; blinkCntr < (nbrOfBlink + 1); blinkCntr++)
    {
        drawCharAllSevenSeg(0xFF);
        delay(100);
        drawCharAllSevenSeg(0x00);
        delay(100);
    }
}

void SevenSegController::turnOffAllDisplay(void)
{
    uint8_t onDisplay = 0x00;
    uint8_t character = 0x00;

    if (_isCathodCommun == false)
    {
        character = ~character;
        onDisplay = 0xFF;
    }

    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, ~onDisplay);
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, character);
    shiftLatchPulse();
}

void SevenSegController::turnOnAllDisplay(void)
{
    uint8_t onDisplay = 0xFF;
    uint8_t character = 0xFF;

    if (_isCathodCommun == false)
    {
        character = ~character;
        onDisplay = 0x00;
    }

    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, ~onDisplay);
    shiftOut(_shiftDataPin, _shiftClkPin, MSBFIRST, character);
    shiftLatchPulse();
}

void SevenSegController::shiftLatchPulse(void)
{
    digitalWrite(_shiftLatchPin, LOW);
    digitalWrite(_shiftLatchPin, HIGH);
    digitalWrite(_shiftLatchPin, LOW);
}

uint8_t SevenSegController::splitNumberForDisplay(uint32_t number, uint8_t displayPosition)
{
    uint32_t digit = 0;
    for (uint8_t i = 0; i <= displayPosition; ++i)
    {
        digit = number % 10;
        number /= 10;
    }

    // Return the corresponding 8-bit value from sevenSegChar
    if (digit <= _sevenSegCharSize)
    {
        return (sevenSegChar[digit]);
    }
    else
    {
        return 0x80;
    }
}
