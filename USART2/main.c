#include "stm32f0xx.h"
#include <stdint.h>



// Uncomment only one of the following to test each step
//#define STEP41
//#define STEP42
//#define STEP43
#define STEP44

void init_usart2();

void init_usart2() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // enables port A
    GPIOA->MODER |= 0x20; // Enables PA2
    // PA2 + USART2_TX = AF1
    // PA3 + USART2_RX = AF1
    GPIOA->AFR[0] |= 1<<8; // 0001 is AF1, and shift 8 bits left to reference pin 2

    // enable clock for USART
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // disable USART2 by turning off UE bit
    USART2->CR1 &= ~(USART_CR1_UE);

    // set word size of 8 bits
    USART2->CR1 &= ~(USART_CR1_M);

    // set for one stop bit
    USART2->CR2 &= ~(USART_CR2_STOP);

    // set it for no parity
    USART2->CR1 &= ~(USART_CR1_PCE);

    // use 16x oversampling
    USART2->CR1 &= ~(USART_CR1_OVER8);

    // baud rate of 19200 (19.2k baud)
    USART2->BRR = 0x9C4; // look at table 96 of the family reference manual

    // enable transmitter and the receiver by setting the TE and RE bits
    USART2->CR1 |= USART_CR1_TE;
    //USART2->CR1 |= USART_CR1_RE;

    // enable the USART
    USART2->CR1 |= USART_CR1_UE;

    // wait for the TE and RE bits to be acknowledged by checking the TEACK and REACK bits are both set in the ISR
    // indicates that the USART is ready to transmit and receive
    USART2->ISR |= USART_ISR_TEACK;
    //USART2->ISR |= USART_ISR_REACK;

}


#ifdef STEP44

#include <stdio.h>

#define MESSAGESIZE 20

unsigned char message[MESSAGESIZE] = {'B', '\x08A', '\x0A0', '\x0CA', '2', '\x0AC', '\x0A0',
							'\b', '\1', '\0', '\x098', '(', '*', '\n', '0x0CC', '0x0A0', '0x0A0',
							'\b', '2', 'B'};

int count = 0;

//uint8_t message[MESSAGESIZE] = {0xFE, 0x42, 0x8A, 0xA0, 0xCA, 0x32, 0xAC, 0xA0,
//					   0x08, 0x02, 0x00, 0x98, 0x28, 0x2A, 0x0A, 0xCC, 0xA0,
//					   0xA0, 0x08, 0x32, 0x42};

uint8_t message2[MESSAGESIZE] = {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
					   0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
					   0x12, 0x12, 0x12, 0x12};

void enable_tty_interrupt(void) {
    // Raise an interrupt every time the receive data register becomes not empty
    // Remember to set the proper bit in the NVIC ISER as well
    // Note that the name of the bit to set is difficult to determine. It is USART3_8_IRQn
    USART2->CR1 |= USART_CR1_TXEIE;

    // Trigger a DMA operation every time the receive data register becomes not empty
    USART2->CR3 |= USART_CR3_DMAT;
    NVIC->ISER[0] = 1<<USART2_IRQn;

    // The subroutine should also enable the RCC clock for DMA Controller 2
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
    DMA2->RMPCR |= DMA2_CH1_USART2_TX;
    DMA2_Channel1->CCR &= ~DMA_CCR_EN;  // First make sure DMA is turned off

    // CMAR should be set to the address of serfifo
    DMA2_Channel1->CMAR = (uint32_t)(message2); // configure memory address

    // CPAR should be set to the address of the USART5->RDR
    DMA2_Channel1->CPAR = (uint32_t)(&(USART2->TDR)); // Set CPAR to the address of the USART5->RDR register.

    // CNDTR should be set to FIFOSIZE
    DMA2_Channel1->CNDTR = MESSAGESIZE;

    // The DIRection of copying should be from peripheral to memory
    DMA2_Channel1->CCR |= DMA_CCR_DIR;

    // Neither the total-completion nor the half-transfer interrupt should be enabled
    DMA2_Channel1->CCR &= ~DMA_CCR_HTIE;
    DMA2_Channel1->CCR &= ~DMA_CCR_TCIE;

    // Both the MSIZE and the PSIZE should be set for 8 bits
    //set memory datum size to 8bit
    DMA2_Channel1->CCR &= ~DMA_CCR_MSIZE;
    //set peripheral datum size to 8bit
    DMA2_Channel1->CCR &= ~DMA_CCR_PSIZE;

    // MINC should be set to increment the CMAR
    DMA2_Channel1->CCR |= DMA_CCR_MINC; // increments the CMAR for every transfer

    // PINC should not be set so that CPAR always points at the USART5->RDR.
    DMA2_Channel1->CCR &= ~DMA_CCR_PINC;

    // Enable CIRCular transfers.
    DMA2_Channel1->CCR &= ~DMA_CCR_CIRC; //Set the channel for CIRCular operation.

    // Do not enable MEM2MEM transfers.
    DMA2_Channel1->CCR &= ~DMA_CCR_MEM2MEM; // memory to memory mode disabled

    // Set the Priority Level to highest.
    DMA2_Channel1->CCR |= DMA_CCR_PL;

    //Finally, make sure that the channel is enabled for operation.
    DMA2_Channel1->CCR |= DMA_CCR_EN; // enable DMA

}

/*
// Works like line_buffer_getchar(), but does not check or clear ORE nor wait on new characters in USART
char interrupt_getchar() {
    // TODO

    // USART_TypeDef *u = USART5;

    // If we missed reading some characters, clear the overrun flag.
    if (u->ISR & USART_ISR_ORE)
        u->ICR |= USART_ICR_ORECF;

    // Wait for a newline to complete the buffer.
    while(fifo_newline(&input_fifo) == 0) {
        asm volatile ("wfi"); // wait for an interrupt
        while (!(u->ISR & USART_ISR_RXNE))
            ;
        insert_echo_char(u->RDR);
    }
    // Return a character from the line buffer.
    char ch = fifo_remove(&input_fifo);
    return ch;
}
*/

int __io_putchar(char c) {
    // TODO Copy from step 42
    while(!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = c;
    return c;
}

/*
int __io_getchar(void) {
    // TODO Use interrupt_getchar() instead of line_buffer_getchar()
    int c = interrupt_getchar();
    return c;
}
*/

// name from startup_stm32.s
//void USART2_IRQHandler(void) {
//
//}

int main() {
    init_usart2();
    //enable_tty_interrupt();
    for(;;) {
    	if (count == MESSAGESIZE) break;
    	int c = message[count];
    	count++;
    	__io_putchar(c);
    }
}
#endif
