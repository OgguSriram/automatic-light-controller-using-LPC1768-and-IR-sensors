## 📄 main.c

```c
#include <LPC17xx.h>
#include "LCD.h"
#include "stdio.h"

#define IR1 (1 << 4)    // P0.4
#define IR2 (1 << 5)    // P0.5
#define RELAY (1 << 28) // P1.28

volatile uint32_t count = 0;

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 6000; i++);
}

void GPIO_init(void) {
    // IR1 and IR2 as input
    LPC_GPIO0->FIODIR &= ~(IR1 | IR2);  // Set as input

    // Relay as output
    LPC_GPIO1->FIODIR |= RELAY;        // Output for relay
}

void light_control(void) {
    if (count > 0)
        LPC_GPIO1->FIOSET = RELAY;  // Turn ON light
    else
        LPC_GPIO1->FIOCLR = RELAY;  // Turn OFF light
}

int main(void) {
    GPIO_init();
    initLCD();
    sendString("HII");
    char buffer[20];

    while (1) {
        // ENTRY: IR1 triggers first, then IR2
        if (!(LPC_GPIO0->FIOPIN & IR1)) {  // IR1 active LOW
            while (!(LPC_GPIO0->FIOPIN & IR1));  // Wait for release

            while (1) {
                if (!(LPC_GPIO0->FIOPIN & IR2)) {  // IR2 active LOW
                    while (!(LPC_GPIO0->FIOPIN & IR2));  // Wait for release
                    count++;
                    commandOrData(0x80, 0); // Set cursor
                    sprintf(buffer, "Count: %d ", count);
                    sendString(buffer);
                    light_control();
                    break;
                }
            }
        }

        // EXIT: IR2 triggers first, then IR1
        if (!(LPC_GPIO0->FIOPIN & IR2)) {  // IR2 active LOW
            while (!(LPC_GPIO0->FIOPIN & IR2));  // Wait for release

            while (1) {
                if (!(LPC_GPIO0->FIOPIN & IR1)) {  // IR1 active LOW
                    while (!(LPC_GPIO0->FIOPIN & IR1));  // Wait for release
                    if (count > 0) count--;
                    commandOrData(0x80, 0); // Set cursor
                    sprintf(buffer, "Count: %d ", count);
                    sendString(buffer);
                    light_control();
                    break;
                }
            }
        }
    }
}
