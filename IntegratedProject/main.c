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
        display_handle(time_left);
    }
    else {
        gamemode = -1;
        time_left = GAMETIME;
        GPIOB->BRR = 0xFFF;
        Build_LED_Targets(ledstrip, red, red, red, red, red);
        TIM2->CR1 &= ~TIM_CR1_CEN;
        nano_wait(1000000000);
        display_handle(targets_hit);
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
            nano_wait(1000000000);
            display_handle(time_left);
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
