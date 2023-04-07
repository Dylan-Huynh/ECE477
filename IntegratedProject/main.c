//============================================================================
//
//            THIS PROGRAM IS FOR GAME MODE TESTING
//
//============================================================================

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define GAMETIME 19
#define ONE (38)
#define ZERO (19)
#define RS (0)
#define NUMLED (185)
#define NUMTARGET (5)
#define LEDSIZE (24*NUMLED + 600)
#define MESSAGESIZE 42
#define BLUETOOTHMESSAGESIZE 24
#define BLUETOOTHMESSAGESIZE 28

uint8_t gametimeMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x39,
				 	 	 	 	 0x00, 0x39, 0x00, 0x39, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
								 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x30};
uint8_t titleMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x59,
				 	 	 	 	 0x00, 0x75, 0x00, 0x6b, 0x00, 0x69, 0x00, 0x20, 0x00, 0x4F, 0x00, 0x74, 0x00, 0x61, 0x00,
								 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t gamefinishedMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x47,
				 	 	 	 	 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4F, 0x00, 0x76, 0x00, 0x65, 0x00,
								 0x00, 0x72, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t timescoreMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x54,
				 	 	 	 	 0x00, 0x49, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
								 0x20, 0x00, 0x53, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x52, 0x00, 0x45, 0x00};

uint8_t bluetoothGame2[BLUETOOTHMESSAGESIZE] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};

uint8_t bluetoothGame2[BLUETOOTHMESSAGESIZE] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};

uint8_t bluetoothGame3[BLUETOOTHMESSAGESIZE] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00};

uint8_t ledstrip[LEDSIZE] = {0};
uint8_t redChain[LEDSIZE] = {0};
uint8_t blueChain[LEDSIZE] = {0};
uint8_t offChain[LEDSIZE] = {0};

uint8_t off[24] = {19,19,19,19,19,19,19,19,
				  19,19,19,19,19,19,19,19,
				  19,19,19,19,19,19,19,19};
uint8_t red[24] = {19,19,19,19,19,19,19,19,
					38,38,38,38,38,38,38,38,
					19,19,19,19,19,19,19,19};
uint8_t green[24] = {38,38,38,38,38,38,38,38,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
uint8_t blue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  38,38,38,38,38,38,38,38};
uint8_t white[24] = {38,38,38,38,38,38,38,38,
						38,38,38,38,38,38,38,38,
						38,38,38,38,38,38,38,38};
uint8_t purple[24] = {19,19,19,19,19,19,19,19,
					38,38,38,38,38,38,38,38,
					38,38,38,38,38,38,38,38};

uint8_t halfred[24] = {19,19,19,19,19,19,19,19,
					38,19,19,19,19,19,19,19,
					19,19,19,19,19,19,19,19};
uint8_t halfgreen[24] = {38,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
uint8_t halfblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  38,19,19,19,19,19,19,19};
uint8_t halfwhite[24] = {38,19,19,19,19,19,19,19,
						38,19,19,19,19,19,19,19,
						38,19,19,19,19,19,19,19};

uint8_t quarterred[24] = {19,19,19,19,19,19,19,19,
					19,19,38,19,38,19,19,19,
					19,19,19,19,19,19,19,19};
uint8_t quartergreen[24] = {19,19,38,19,38,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
uint8_t quarterblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,38,19,38,19,19,19};
uint8_t quarterwhite[24] = {19,19,38,19,38,19,19,19,
						19,19,38,19,38,19,19,19,
						19,19,38,19,38,19,19,19};

uint8_t lowestred[24] = {19,19,19,19,19,19,19,19,
					19,19,19,19,19,19,19,38,
					19,19,19,19,19,19,19,19};
uint8_t lowestgreen[24] = {19,19,19,19,19,19,19,38,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
uint8_t lowestblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,38};
uint8_t lowestwhite[24] = {19,19,19,19,19,19,19,38,
						19,19,19,19,19,19,19,38,
						19,19,19,19,19,19,19,38};

// Status variables
static int target = -1; // neutral state
static int gamemode = -1; // menu
static int time_left = GAMETIME; // time for gamemode_2 - 1 second
static int targets_hit = 0; // amount of targets hit in gamemode_2

// Bluetooth message data
static uint8_t target1_hit = 0;
static uint8_t target1_miss = 0;
static uint8_t target2_hit = 0;
static uint8_t target2_miss = 0;
static uint8_t target3_hit = 0;
static uint8_t target3_miss = 0;
static uint8_t target4_hit = 0;
static uint8_t target4_miss = 0;
static uint8_t target5_hit = 0;
static uint8_t target5_miss = 0;
static uint8_t blade_hit = 0;
static uint8_t blade_miss = 0;

static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void display_handle(int num) {
	//            7
	//
	//    1               6
	//
	//            0
	//
	//    2               5
	//
	//            3

	// 4 = DOT
	num = num % 10;
	if (num == 0){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xEE;
	}
	else if (num == 1){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0x60;
	}
	else if (num == 2){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xCD;
	}
	else if (num == 3){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xE9;
	}
	else if (num == 4){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0x63;
	}
	else if (num == 5){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xAB;
	}
	else if (num == 6){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xAF;
	}
	else if (num == 7){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xE0;
	}
	else if (num == 8){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xEF;
	}
	else if (num == 9){
		GPIOB->BRR = 0xFF;
		GPIOB->BSRR = 0xE3;
	}

}

int randomX() {
    return ((rand() % (4)) + 1) ;
}

void pick_target(void){
	GPIOB->BRR = 0xf00;
	nano_wait(50000000);
	target = randomX();
	if (target == 1){
		GPIOB->BSRR = 0x0100;
		Build_LED_Targets(ledstrip, green, off, off, off, off);
		nano_wait(10000);
	}
	if (target == 2){
		GPIOB->BSRR = 0x0200;
		Build_LED_Targets(ledstrip, off, green, off, off, off);
		nano_wait(10000);
	}
	if (target == 3){
		GPIOB->BSRR = 0x0400;
		Build_LED_Targets(ledstrip, off, off, green, off, off);
		nano_wait(10000);
	}
	if (target == 4){
		GPIOB->BSRR = 0x0800;
		Build_LED_Targets(ledstrip, off, off, off, green, off);
		nano_wait(10000);
	}
	/*
	if (target == 5){
		GPIOB->BSRR = 0x1000;
		Build_LED_Targets(ledstrip, off, off, off, off, green);
		nano_wait(10000);
	}
	*/
}

void CheckIfTargetHit(uint32_t hit) {
    uint32_t light = GPIOB->ODR >> 8;
    if (target == -1) {
        pick_target();
    }
    else if (hit == light) {
        pick_target();
        if (gamemode == 2) {
            /*
            if (!((TIM2->CR1) & TIM_CR1_CEN)) {
                TIM2->CR1 |= TIM_CR1_CEN;
            }
            */
            targets_hit++;
        }
    }
}

void menu_setup() {
    gamemode = 0;
    GPIOB->ODR = 0;
    nano_wait(1000000000);
    GPIOB->BSRR = 0x300;
    Build_LED_Targets(ledstrip, purple, purple, off, off, off);
	send_LED_message[titleMessage];
}

void init_tim1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR2 |= TIM_CR2_CCDS;
    TIM1->DIER |= TIM_DIER_TDE;
    TIM1->DIER |= TIM_DIER_UDE;
    TIM1->DIER |= TIM_DIER_CC1DE;

    TIM1->PSC = 1 - 1;
    TIM1->ARR = 60 - 1;

    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC2M); // clear
    TIM1->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC4M); // clear
    TIM1->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1) | (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1); // this holds channel 1 and 2
    TIM1->CCMR2 = (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1) | (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // this holds channel 3 and 4
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void init_dma(void) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; // DMA clock disabled
    DMA1_Channel2->CCR &= ~DMA_CCR_EN; // makes sure DMA is turned off
    DMA1_Channel2->CPAR = (uint32_t)(&(TIM1->CCR1)); // Set CPAR to the address of the TIM1->CCR1 register.
    DMA1_Channel2->CMAR = (uint32_t)(ledstrip); // configure memory address
    DMA1_Channel2->CNDTR = (uint16_t) LEDSIZE; // sends 1 byte (1 integer)
    DMA1_Channel2->CCR |= DMA_CCR_DIR; // this sets it so it reads from memory
    DMA1_Channel2->CCR |= DMA_CCR_MINC; // increments the CMAR for every transfer
    DMA1_Channel2->CCR &= ~DMA_CCR_PSIZE; // clears out the bits for psize
    DMA1_Channel2->CCR |= DMA_CCR_PSIZE_0; // set to 1 = 16 bits
    DMA1_Channel2->CCR &= ~DMA_CCR_CIRC;
    DMA1_Channel2->CCR |= DMA_CCR_CIRC; //Set the channel for CIRCular operation.
}

void Build_LED(uint8_t *ledstrip, uint8_t *color) {
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;

	for(int i = 0; i < NUMLED; i++)
	{
		for(int j = 0; j < 24; j++){
			ledstrip[i*24 + j] = color[j];
		}
	}

	int k;
	int size = sizeof(ledstrip)/sizeof(ledstrip[0]); // Get the size of the array
	for (k = 0; k < size && ledstrip[k] != 0; k++);
	for (; k < size; k++) {
		ledstrip[k] = 0;
	}

	DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void Build_LED_Targets(uint8_t *ledstrip, uint8_t *color1, uint8_t *color2, uint8_t *color3, uint8_t *color4, uint8_t *color5) {
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	int targetLEDsize = NUMLED / 5;

	for(int i = 0; i < targetLEDsize; i++)
	{
		for(int j = 0; j < 24; j++){
			ledstrip[i*24 + j] = color1[j];
		}
	}

	for(int i = targetLEDsize; i < targetLEDsize*2; i++)
		{
			for(int j = 0; j < 24; j++){
				ledstrip[i*24 + j] = color2[j];
			}
		}

	for(int i = targetLEDsize*2; i < targetLEDsize*3; i++)
		{
			for(int j = 0; j < 24; j++){
				ledstrip[i*24 + j] = color3[j];
			}
		}

	for(int i = targetLEDsize*3; i < targetLEDsize*4; i++)
		{
			for(int j = 0; j < 24; j++){
				ledstrip[i*24 + j] = color4[j];
			}
		}

	for(int i = targetLEDsize*4; i < targetLEDsize*5; i++)
		{
			for(int j = 0; j < 24; j++){
				ledstrip[i*24 + j] = color5[j];
			}
		}

	DMA1_Channel2->CCR |= DMA_CCR_EN;
}

void init_usart1() {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // enables port A
    GPIOB->MODER |= 0x2000; // Enables PB6
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

void send_LED_message(uint8_t arr) {
    // TODO Copy from step 42
	for(int i =0; i <= MESSAGESIZE; i++) {
		while(!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = arr[i];
	}
}

void change_score(int n) {


	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;
	int n_thousand = n / 1000;

	gametimeMessage[40] = n_one + 48;
	gametimeMessage[38] = n_ten + 48;
	gametimeMessage[36] = n_hundred + 48;
	gametimeMessage[34] = n_thousand+ 48;
}

void change_time(int n) {
	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;

	gametimeMessage[18] = n_one + 48;
	gametimeMessage[16] = n_ten + 48;
	gametimeMessage[14] = n_hundred + 48;
}

void load_and_send_bluetooth2() {
	bluetoothGame2[1] = target1_hit;
	bluetoothGame2[2] = target1_miss;
	bluetoothGame2[4] = target1_hit;
	bluetoothGame2[5] = target1_miss;
	bluetoothGame2[8] = target1_hit;
	bluetoothGame2[9] = target1_miss;
	bluetoothGame2[12] = target1_hit;
	bluetoothGame2[13] = target1_miss;
	bluetoothGame2[16] = target1_hit;
	bluetoothGame2[17] = target1_miss;
	bluetoothGame2[20] = blade1_hit;
	bluetoothGame2[21] = blade1_miss;
	for(int i =0; i <= BLUETOOTHMESSAGESIZE; i++) {
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = arr[i];
	}
}

void init_tim2() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->PSC = 47999;
    TIM2->ARR = 999;
    NVIC->ISER[0] = 1 << TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn, 29);
}

void TIM2_IRQHandler() {
    TIM2->SR = ~TIM_SR_UIF;
    if (time_left) {
        time_left--;
		change_time(time_left);
		change_score(targets_hit);
		send_LED_message(gametimeMessage);
        //display_handle(time_left);
    }
    else {
        gamemode = -1;
        time_left = GAMETIME;
        GPIOB->BRR = 0xFFF;
        Build_LED_Targets(ledstrip, red, red, red, red, red);
		send_LED_message(gamefinishedMessage);
        TIM2->CR1 &= ~TIM_CR1_CEN;
        nano_wait(1000000000);
		send_LED_message(gametimeMessage);
        //display_handle(targets_hit);
        nano_wait(1000000000);
        menu_setup();
    }
}

void init_exti() {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    SYSCFG->EXTICR[0] = SYSCFG->EXTICR[1] = SYSCFG->EXTICR[3] = 0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC | SYSCFG_EXTICR1_EXTI1_PC | SYSCFG_EXTICR1_EXTI2_PC | SYSCFG_EXTICR1_EXTI3_PC;
    //SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC;
    //SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB; // Old menu button used in testing
    EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4;
    EXTI->RTSR |= EXTI_RTSR_TR15;
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 | EXTI_IMR_MR3| EXTI_IMR_MR4 | EXTI_IMR_MR15;
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn | 1<<EXTI2_3_IRQn | 1<<EXTI4_15_IRQn;
}

void EXTI0_1_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR0) {
        EXTI -> PR = EXTI_PR_PR0;
        if (gamemode == -1) {}
        else if (gamemode == 0) { // menu
            gamemode = 1;
            GPIOB->BRR = 0xFFF;
            Build_LED_Targets(ledstrip, white, white, white, white, white);
            nano_wait(1000000000);
            pick_target();
        }
        else if (gamemode) { // endless
            CheckIfTargetHit(0x1);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR1) {
        EXTI->PR = EXTI_PR_PR1;
        if (gamemode == -1) {}
        else if (gamemode == 0) { // menu
            targets_hit = 0;
            gamemode = 2;
            GPIOB->BRR = 0xFFF;
            Build_LED_Targets(ledstrip, white, white, white, white, white);
			send_LED_message(gamefinishedMessage);
            nano_wait(1000000000);
			load_bluetooth_message();
			change_time(time_left);
			change_score(targets_hit);
			send_LED_message(gametimeMessage);
            //display_handle(time_left);
            TIM2->CR1 |= TIM_CR1_CEN;
            pick_target();
        }
        else if (gamemode) {
            CheckIfTargetHit(0x2);
        }
    }
}

void EXTI2_3_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR2) {
        EXTI->PR = EXTI_PR_PR2;
        if (gamemode == -1) {}
        else if (gamemode) {
            CheckIfTargetHit(0x4);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR3) {
        EXTI->PR = EXTI_PR_PR3;
        if (gamemode == -1) {}
        else if (gamemode) {
            CheckIfTargetHit(0x8);
        }
    }
}

void EXTI4_15_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR4) {
        EXTI->PR = EXTI_PR_PR4;
        if (gamemode == -1) {}
        else if (gamemode) {
            CheckIfTargetHit(0x10);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR15) {
        EXTI->PR = EXTI_PR_PR15;
        if (gamemode == -1) {}
        else if (gamemode) {
            menu_setup();
        }
    }
}

void init_pins() {
	// Only enable port B for the Blade
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	GPIOB->MODER &= ~0xffffffff;
	GPIOB->MODER |= 0x00555555; // PB0-11 outputs
	GPIOA->MODER &= ~0x000fffff;
	GPIOA->MODER |= 0x00020000; // PA8 alternate function
	GPIOA->AFR[1] &= ~0x0000FFFF;
	GPIOA->AFR[1] |= 0x00002222;
	GPIOB->PUPDR |= 0xaaaaaaa;
	GPIOC->MODER &= ~0xffffffff; // PC0-3 inputs
}

void main() {
	init_tim1();
	init_dma();
    init_tim2();
    init_pins();
    GPIOB->ODR = 0;
    init_exti();
    menu_setup();
    for(;;) {
        asm volatile ("wfi");
    }
}
