#include "RootEncoder.h"

const int8_t RootEncoder::EncoderTable[16] = {
     0,  1, -1,  0,
    -1,  0,  0,  1,
     1,  0,  0, -1,
     0, -1,  1,  0
};

RootEncoder::RootEncoder(uint8_t InPinA, uint8_t InPinB, uint8_t InPinButton)
    : PinA(InPinA), PinB(InPinB), PinButton(InPinButton)
{}

void RootEncoder::Setup() 
{
    pinMode(PinA, INPUT_PULLUP);
    pinMode(PinB, INPUT_PULLUP);
    if(PinButton != 255) pinMode(PinButton, INPUT_PULLUP);
	
    LastStateFlags = ReadStateFlags();
}

void RootEncoder::Update() 
{
	// Update Encoder
    uint8_t state = ReadStateFlags();
    uint8_t key = (LastStateFlags << 2) | state;
    int8_t delta = EncoderTable[key & 0x0F];
    LastStateFlags = state;
    if(delta != 0 && OnRotate) OnRotate(delta);
    
	// Update Button
	if(PinButton == 255) return;

	bool bPressed = digitalRead(PinButton) == LOW;
	uint32_t now = millis();

	if(bPressed && !bButtonLastState) 
	{
		bButtonLastState = true;
		ButtonDownTime = now;
		bHoldTriggered = false;
	} 
	else if(!bPressed && bButtonLastState) 
	{
		bButtonLastState = false;
		if(!bHoldTriggered && (now - ButtonDownTime < RootEncoder_HoldTime)) 
		{
			// короткий клик
			if(now - LastClickTime > RootEncoder_ClickTimeout) ClickCounter = 0; // сброс старых
			ClickCounter++;
			LastClickTime = now;
		}
	}

	// проверка удержания
	if(bPressed && !bHoldTriggered && (now - ButtonDownTime >= RootEncoder_HoldTime)) 
	{
		bHoldTriggered = true;
		if(OnPressEvent) OnPressEvent(0);
		ClickCounter = 0;
		return;
	}

	// проверка завершения кликов
	if(!bPressed && ClickCounter > 0 && (now - LastClickTime > RootEncoder_ClickTimeout)) 
	{
		if(OnPressEvent) OnPressEvent(ClickCounter);
		ClickCounter = 0;
		return;
	}
}
