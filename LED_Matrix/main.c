#include "stm32f0xx.h"
#include <stdint.h>



// Uncomment only one of the following to test each step
//#define STEP41
//#define STEP42
//#define STEP43
#define STEP44

void init_usart2();
void setup_button();
void switch_message();
void init_usart1();

static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void init_usart1() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // enables port A
    GPIOB->MODER |= 0x2000; // Enables PA2
    // PA2 + USART2_TX = AF1
    // PA3 + USART2_RX = AF1
    GPIOB->AFR[0] |= 0<<24; // 0001 is AF1, and shift 8 bits left to reference pin 2

    // enable clock for USART
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // disable USART2 by turning off UE bit
    USART1->CR1 &= ~(USART_CR1_UE);

    // set word size of 8 bits
    USART1->CR1 &= ~(USART_CR1_M);

    // set for one stop bit
    USART1->CR2 &= ~(USART_CR2_STOP);

    // set it for no parity
    USART1->CR1 &= ~(USART_CR1_PCE);

    // use 16x oversampling
    USART1->CR1 &= ~(USART_CR1_OVER8);

    // baud rate of 19200 (19.2k baud)
    USART1->BRR = 0x9C4; // look at table 96 of the family reference manual

    // enable transmitter and the receiver by setting the TE and RE bits
    USART1->CR1 |= USART_CR1_TE;
    //USART2->CR1 |= USART_CR1_RE;

    // enable the USART
    USART1->CR1 |= USART_CR1_UE;

    // wait for the TE and RE bits to be acknowledged by checking the TEACK and REACK bits are both set in the ISR
    // indicates that the USART is ready to transmit and receive
    USART1->ISR |= USART_ISR_TEACK;
    //USART2->ISR |= USART_ISR_REACK;

}

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

void setup_button(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // enables port B
	GPIOB->MODER &= ~0x000F0000;
	GPIOB->MODER |= 0x00010000;
	GPIOB->PUPDR |= 0xaaaaaaa;

	GPIOB->ODR = 0x100;
}

#ifdef STEP44

#include <stdio.h>

#define MESSAGESIZE1 42
#define MESSAGESIZE2 30

int message1[MESSAGESIZE1] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x54,
				 	 	 	 	 0x00, 0x49, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
								 0x20, 0x00, 0x53, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x52, 0x00, 0x45, 0x26};

int message2[MESSAGESIZE1] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x39,
				 	 	 	 	 0x00, 0x39, 0x00, 0x39, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
								 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x30};

uint8_t message3[MESSAGESIZE2] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x14, 0x41, 0x41, 0x33, 0x41, 0x00, 0x79,
				 0x00, 0x75, 0x00, 0x6b, 0x00, 0x69, 0x00, 0x20, 0x00, 0x6F, 0x00, 0x74, 0x00, 0x61, 0x26};

uint8_t message4[MESSAGESIZE2] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x14, 0x41, 0x41, 0x33, 0x41, 0x00, 0x30,
				 0x00, 0x32, 0x00, 0x32, 0x00, 0x32, 0x00, 0x20, 0x00, 0x33, 0x00, 0x33, 0x00, 0x33, 0x26};


int count = 0;

//uint8_t message[MESSAGESIZE] = {0xFE, 0x42, 0x8A, 0xA0, 0xCA, 0x32, 0xAC, 0xA0,
//					   0x08, 0x02, 0x00, 0x98, 0x28, 0x2A, 0x0A, 0xCC, 0xA0,
//					   0xA0, 0x08, 0x32, 0x42};

//uint8_t message2[MESSAGESIZE] = {0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
//					   0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
//					   0x12, 0x12, 0x12, 0x12};


void enable_tty_interrupt1(void) {
    // Raise an interrupt every time the receive data register becomes not empty
    // Remember to set the proper bit in the NVIC ISER as well
    // Note that the name of the bit to set is difficult to determine. It is USART3_8_IRQn
    USART1->CR1 |= USART_CR1_TXEIE;

    // Trigger a DMA operation every time the receive data register becomes not empty
    USART1->CR3 |= USART_CR3_DMAT;
    NVIC->ISER[0] = 1<<USART1_IRQn;

    // The subroutine should also enable the RCC clock for DMA Controller 2
    RCC->AHBENR |= RCC_AHBENR_DMA2EN;
    DMA2->RMPCR |= DMA2_CH1_USART2_TX;
    DMA2_Channel4->CCR &= ~DMA_CCR_EN;  // First make sure DMA is turned off

    // CMAR should be set to the address of serfifo
    DMA2_Channel4->CMAR = (uint32_t)(message1); // configure memory address

    // CPAR should be set to the address of the USART5->RDR
    DMA2_Channel4->CPAR = (uint32_t)(&(USART2->TDR)); // Set CPAR to the address of the USART5->RDR register.

    // CNDTR should be set to FIFOSIZE
    DMA2_Channel4->CNDTR = MESSAGESIZE1;

    // The DIRection of copying should be from peripheral to memory
    DMA2_Channel4->CCR |= DMA_CCR_DIR;

    // Neither the total-completion nor the half-transfer interrupt should be enabled
    DMA2_Channel4->CCR &= ~DMA_CCR_HTIE;
    DMA2_Channel4->CCR &= ~DMA_CCR_TCIE;

    // Both the MSIZE and the PSIZE should be set for 8 bits
    //set memory datum size to 8bit
    DMA2_Channel4->CCR &= ~DMA_CCR_MSIZE;
    //set peripheral datum size to 8bit
    DMA2_Channel4->CCR &= ~DMA_CCR_PSIZE;

    // MINC should be set to increment the CMAR
    DMA2_Channel4->CCR |= DMA_CCR_MINC; // increments the CMAR for every transfer

    // PINC should not be set so that CPAR always points at the USART5->RDR.
    DMA2_Channel4->CCR &= ~DMA_CCR_PINC;

    // Enable CIRCular transfers.
    DMA2_Channel4->CCR &= ~DMA_CCR_CIRC; //Set the channel for CIRCular operation.

    // Do not enable MEM2MEM transfers.
    DMA2_Channel4->CCR &= ~DMA_CCR_MEM2MEM; // memory to memory mode disabled

    // Set the Priority Level to highest.
    DMA2_Channel4->CCR |= DMA_CCR_PL;

    //Finally, make sure that the channel is enabled for operation.
    DMA2_Channel4->CCR |= DMA_CCR_EN; // enable DMA

}

void enable_tty_interrupt2(void) {
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
    DMA2_Channel2->CCR &= ~DMA_CCR_EN;  // First make sure DMA is turned off

    // CMAR should be set to the address of serfifo
    DMA2_Channel2->CMAR = (uint32_t)(message2); // configure memory address

    // CPAR should be set to the address of the USART5->RDR
    DMA2_Channel2->CPAR = (uint32_t)(&(USART2->TDR)); // Set CPAR to the address of the USART5->RDR register.

    // CNDTR should be set to FIFOSIZE
    DMA2_Channel2->CNDTR = MESSAGESIZE1;

    // The DIRection of copying should be from peripheral to memory
    DMA2_Channel2->CCR |= DMA_CCR_DIR;

    // Neither the total-completion nor the half-transfer interrupt should be enabled
    DMA2_Channel2->CCR &= ~DMA_CCR_HTIE;
    DMA2_Channel2->CCR &= ~DMA_CCR_TCIE;

    // Both the MSIZE and the PSIZE should be set for 8 bits
    //set memory datum size to 8bit
    DMA2_Channel2->CCR &= ~DMA_CCR_MSIZE;
    //set peripheral datum size to 8bit
    DMA2_Channel2->CCR &= ~DMA_CCR_PSIZE;

    // MINC should be set to increment the CMAR
    DMA2_Channel2->CCR |= DMA_CCR_MINC; // increments the CMAR for every transfer

    // PINC should not be set so that CPAR always points at the USART5->RDR.
    DMA2_Channel2->CCR &= ~DMA_CCR_PINC;

    // Enable CIRCular transfers.
    DMA2_Channel2->CCR &= ~DMA_CCR_CIRC; //Set the channel for CIRCular operation.

    // Do not enable MEM2MEM transfers.
    DMA2_Channel2->CCR &= ~DMA_CCR_MEM2MEM; // memory to memory mode disabled

    // Set the Priority Level to highest.
    DMA2_Channel2->CCR |= DMA_CCR_PL;

    //Finally, make sure that the channel is enabled for operation.
    DMA2_Channel2->CCR |= DMA_CCR_EN; // enable DMA

}

int __io_putchar(char c) {
    while(!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = c;
    return c;
}

void switch_message(){
	DMA2_Channel1->CCR &= ~DMA_CCR_EN;
	DMA2_Channel1->CMAR = (uint32_t)(message4);
	DMA2_Channel1->CCR |= DMA_CCR_EN;
}

void USART1_IRQHandler(void) {
	//if (GPIOB->IDR & GPIO_IDR_2) {
	//    		switch_message();
	//}
	for (int i = 0; i < MESSAGESIZE1; i++) {
		int c = message1[i];
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = c;
	}
	//count++;
	//int c = message1[count];
	//USART2->TDR = c;
}


// name from startup_stm32.s
void USART2_IRQHandler(void) {
	//if (GPIOB->IDR & GPIO_IDR_2) {
	//    		switch_message();
	//}
	if (DMA2->ISR & DMA_ISR_TEIF4) {
		USART1->CR1 |= (USART_CR1_UE);
		DMA2_Channel1->CCR &= ~DMA_CCR_EN;
	}
	for (int i = 0; i < MESSAGESIZE1; i++) {
		int c = message2[i];
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = c;
	}

	//count++;
	//int c = message2[count];
	//USART2->TDR = c;
}

void USART1_upload(void) {
	//USART1->CR1 |= (USART_CR1_UE);
	for (int i = 0; i < MESSAGESIZE1; i++) {
		int c = message1[i];
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = c;
	}
	//USART2->CR1 &= ~(USART_CR1_UE);

}

void USART2_upload(void) {

	//USART2->CR1 |= (USART_CR1_UE);
	for (int i = 0; i < MESSAGESIZE1; i++) {
		int c = message2[i];
		while(!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = c;
	}
	//USART2->CR1 &= ~(USART_CR1_UE);

}

void change_score(int n) {
	//USART2->CR1 &= ~(USART_CR1_UE);
	//uint8_t score_thousand = message1[40];
	//uint8_t score_hundred = message1[42];
	//uint8_t score_ten = message1[44];
	//uint8_t score_one = message1[46];

	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;
	int n_thousand = n / 1000;

	message2[40] = n_one + 48;
	message2[38] = n_ten + 48;
	message2[36] = n_hundred + 48;
	message2[34] = n_thousand+ 48;
}

void change_time(int n) {
	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;

	message2[18] = n_one + 48;
	message2[16] = n_ten + 48;
	message2[14] = n_hundred + 48;
}


int main() {
	init_usart1();
    init_usart2();
    //enable_tty_interrupt1();
    //enable_tty_interrupt2();
    USART1_upload();
    USART2_upload();
    int time = 999;
    int score = 0;
    for(;;) {
    	count++;
    	if (count % 1000 == 0) {
    		change_score(score);
    		change_time(time);
    		//USART1_upload();
    		//USART2_upload();
    		score += 8;
    		time--;
    	}
    	nano_wait(100000);

    }
    //for(;;) {
    //	if (count == MESSAGESIZE2) break;
    //	int c = message3[count];
    //	count++;
    //	__io_putchar(c);
    //}
}
#endif
