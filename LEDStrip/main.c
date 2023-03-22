/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
			
void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

#define ONE (38)
#define ZERO (19)
#define RS (0)
#define NUMLED (38)
#define LEDSIZE (24*NUMLED + 200)
int count = 0;

//BUFFER AT END FOR OFF CYCLE
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

uint8_t halfred[24] = {19,19,19,19,19,19,19,19,
					38,19,38,19,38,19,38,19,
					19,19,19,19,19,19,19,19};
uint8_t halfgreen[24] = {38,19,38,19,38,19,38,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
uint8_t halfblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  38,19,38,19,38,19,38,19};
uint8_t halfwhite[24] = {38,19,38,19,38,19,38,19,
						38,19,38,19,38,19,38,19,
						38,19,38,19,38,19,38,19};

void setup_tim1(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // enables port A

    GPIOA->MODER &= ~0xFF0000; // clear out 8-11
    GPIOA->MODER |= 0xAA0000;

    GPIOA->AFR[1] &= ~0x0000FFFF;
    GPIOA->AFR[1] |= 0x00002222;

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

void setup_dma(void) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN; // DMA clock disabled
    DMA1_Channel2->CCR &= ~DMA_CCR_EN; // makes sure DMA is turned off
    DMA1_Channel2->CPAR = (uint32_t)(&(TIM1->CCR1)); // Set CPAR to the address of the TIM1->CCR1 register.
    DMA1_Channel2->CMAR = (uint32_t)(ledstrip); // configure memory address
    DMA1_Channel2->CNDTR = (uint16_t) LEDSIZE; // sends 1 byte (1 integer)
    DMA1_Channel2->CCR |= DMA_CCR_DIR; // this sets it so it reads from memory
    DMA1_Channel2->CCR |= DMA_CCR_MINC; // increments the CMAR for every transfer
    DMA1_Channel2->CCR &= ~DMA_CCR_PSIZE; // clears out the bits for psize
    DMA1_Channel2->CCR |= DMA_CCR_PSIZE_0; // set to 1 = 16 bits
    DMA1_Channel5->CCR &= ~DMA_CCR_CIRC;
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

void setup_button(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // enables port B
	GPIOB->MODER &= ~0x000F0000;
	GPIOB->MODER |= 0x00010000;
	GPIOB->PUPDR |= 0xaaaaaaa;

	GPIOB->ODR = 0x100;
}

void Add_Reset(uint8_t *ledstrip){
	int i;
	int size = sizeof(ledstrip)/sizeof(ledstrip[0]); // Get the size of the array
	for (i = 0; i < size && ledstrip[i] != 0; i++);
	for (; i < size; i++) {
		ledstrip[i] = 0;
	}
}

int main(void)
{
	setup_tim1();
	setup_dma();
	int count = 0;
    while(1){
    	if(count == 0){
    		Build_LED(ledstrip, red);
    		//Build_LED(ledstrip, halfred);
    	}
    	else if(count == 100000){
    		Build_LED(ledstrip, blue);
    		Build_LED(ledstrip, halfblue);
    	}
    	else if(count == 200000) {
    		Build_LED(ledstrip, green);
    		//Build_LED(ledstrip, halfgreen);
    	}
    	else if(count == 300000) {
    		Build_LED(ledstrip, white);
    		//Build_LED(ledstrip, halfwhite);
        }
    	else if(count == 400000) {
    	    Build_LED(ledstrip, off);
    	}
    	else if(count > 500000) {
    	    count = -1;
    	}

    	nano_wait(10000);
    	count++;

 	}
}
