#ifndef SEVENSEGCONTROLLER_H
#define SEVENSEGCONTROLLER_H
#include <Arduino.h>

const uint8_t sevenSegChar[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

class SevenSegController
{
public:
    SevenSegController(int shiftDataPin, int shiftClkPin, int shiftLatchPin, int nbrDisplay, bool isCathodCommun);
    void begin(void);
    void drawCharSevenSeg(uint8_t character, uint8_t display);
    void drawCharAllSevenSeg(uint8_t character);
    void testSevenSegDisplay(void);
    void displayNbr(void);
    void saveDisplaynumber(uint32_t number);
    void displayBlink(uint8_t nbrOfBlink);
    void turnOffAllDisplay(void);
    void turnOnAllDisplay(void);

private:

    int _shiftDataPin;
    int _shiftClkPin;
    int _shiftLatchPin;
    int _nbrDisplay;
    int _nbrDisplayZeroIndexed;
    bool _isCathodCommun;
    uint8_t _maxNbrOfDisplay;

    uint8_t _sevenSegCharSize;
    uint32_t _maxDisplayNumber;

    uint8_t _numberToDisplay[8];

    uint8_t _currentDisplay;

    int _timer2Freq;

    void shiftLatchPulse(void);

    uint8_t splitNumberForDisplay(uint32_t number, uint8_t displayPosition);
};
#endif /*SEVENSEGCONTROLLER_H*/