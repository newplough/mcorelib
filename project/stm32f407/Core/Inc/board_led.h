#ifndef __BOARD_LED_H__
#define __BOARD_LED_H__
#ifdef __cplusplus
extern "C" {
#endif

void BoardLed_LightOn(unsigned index);
void BoardLed_LightOff(unsigned index);
void BoardLed_Toggle(unsigned index);

#ifdef __cplusplus
}
#endif
#endif
