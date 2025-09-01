#ifndef RootEncoder_h
#define RootEncoder_h

#include <Arduino.h>

#define RootEncoder_HoldTime 800     // мс для зажати
#define RootEncoder_ClickTimeout 300 // мс между кликами

class RootEncoder 
{
	
public:
    RootEncoder(uint8_t InPinA, uint8_t InPinB, uint8_t InPinButton = 255);

    void Setup();
    void Update();

    // Коллбэки
    void (*OnRotate)(int8_t) = nullptr;      // направление вращения
    void (*OnPressEvent)(uint8_t) = nullptr; // 0 = удержание, остальное - клики
	
private:
    uint8_t PinA, PinB, PinButton;
    
	uint8_t LastStateFlags;
	
    bool bButtonLastState;
    uint32_t ButtonDownTime;
    uint32_t LastClickTime;
    uint8_t ClickCounter;
    bool bHoldTriggered;
    
    uint8_t ReadStateFlags() { return (digitalRead(PinA) << 1) | digitalRead(PinB); }
	
    static const int8_t EncoderTable[16];
};

#endif
