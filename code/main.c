##..main.c
#include <LPC17xx.h>
#include <stdio.h>

#define IR1       (LPC_GPIO0->FIOPIN & (1 << 4))
#define IR2       (LPC_GPIO0->FIOPIN & (1 << 6))

#define RELAY_ON()     LPC_GPIO1->FIOSET = (1 << 28)
#define RELAY_OFF()    LPC_GPIO1->FIOCLR = (1 << 28)

#define BUZZER_ON()    LPC_GPIO1->FIOSET = (1 << 27)
#define BUZZER_OFF()   LPC_GPIO1->FIOCLR = (1 << 27)

volatile int count = 0;

void delayMs(unsigned int ms) {
    for (uint32_t i = 0; i < ms * 12000; i++);
}

void UART0_Init(void) {
    LPC_SC->PCONP |= (1 << 3);
    LPC_PINCON->PINSEL0 &= ~((3 << 4) | (3 << 6));
    LPC_PINCON->PINSEL0 |= (1 << 4) | (1 << 6);
    LPC_UART0->LCR = 0x83;
    LPC_UART0->DLM = 0;
    LPC_UART0->DLL = 0xA2;
    LPC_UART0->LCR = 0x03;
}

void UART0_TxChar(char c) {
    while (!(LPC_UART0->LSR & (1 << 5)));
    LPC_UART0->THR = c;
}

void UART0_TxString(const char *str) {
    while (*str) {
        UART0_TxChar(*str++);
    }
}

void UART0_TxNumber(int num) {
    char buf[10];
    sprintf(buf, "%d", num);
    UART0_TxString(buf);
}

void beep() {
    BUZZER_ON();
    delayMs(100);
    BUZZER_OFF();
}

void GPIO_Config() {
    LPC_GPIO0->FIODIR &= ~((1 << 4) | (1 << 6)); 
    LPC_GPIO1->FIODIR |= (1 << 28);          
    LPC_GPIO1->FIODIR |= (1 << 27);           
}

void EntryDetected() {
    count++;
    beep();

    UART0_TxString("Entry Count: ");
    UART0_TxNumber(count);
    UART0_TxString("\r\n");

    UART0_TxString("Total People in Room: ");
    UART0_TxNumber(count);
    UART0_TxString("\r\n");

    if (count > 0)
        RELAY_ON();  // Turn ON bulb
}

void ExitDetected() {
    if (count > 0) {
        count--;
        beep();

        UART0_TxString("Exit Count: ");
        UART0_TxNumber(count);
        UART0_TxString("\r\n");

        UART0_TxString("Total People in Room: ");
        UART0_TxNumber(count);
        UART0_TxString("\r\n");
    }

    if (count == 0)
        RELAY_OFF();  // Turn OFF bulb
}

void detectMovement() {
    if (!IR1) {
        while (!IR1);
        delayMs(50);
        if (!IR2) {
            while (!IR2);
            delayMs(50);
            EntryDetected();
        }
    } else if (!IR2) {
        while (!IR2);
        delayMs(50);
        if (!IR1) {
            while (!IR1);
            delayMs(50);
            ExitDetected();
        }
    }
}

int main() {
    GPIO_Config();
    UART0_Init();

    RELAY_OFF();
    BUZZER_OFF();

    UART0_TxString("System Initialized...\r\n");

    while (1) {
        detectMovement();
		}
}
