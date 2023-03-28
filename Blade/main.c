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
#include "main.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static int target = -1;

static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

void enable_ports(void) {
    // Only enable port B for the Blade
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;
    GPIOB->MODER &= ~0xffffffff;
    GPIOB->MODER |= 0x00550000;
    GPIOA->MODER &= ~0x000fffff;
    GPIOA->MODER |= 0x00050000;
    GPIOB->PUPDR |= 0xaaaaaaa;
    GPIOA->PUPDR |= 0xaaaaaaa;
    GPIOC->MODER |= 0xffffffff;
    GPIOC->MODER &= ~0xffffffff;
    //GPIOC->PUPDR |= 0x55555555;
    //GPIOB->PUPDR |= 0x55555595;
    //GPIOB->PUPDR |= 0x00000040;
}


int main(void)
{
	enable_ports();
	//enable_TIM2();
	//TIM2->CR1 |= 0x1;

	// Old game logic
	/*while(1){

		CheckIfTargetHit();
		nano_wait(500);
	}*/

	// New game logic
	init_exti();
	pick_target();
	for(;;) {}
}

void enable_TIM2(void){


	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 48000 - 1;
	TIM2->ARR = 500 - 1;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 29);
	TIM2->CR1 = TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) {
	TIM2->SR = ~TIM_SR_UIF;
	//CheckIfTargetHit();
}


void CheckIfTargetHit(uint32_t hit){
	uint32_t light = GPIOB->ODR >> 8;
	if (target == -1) {
		pick_target();
	}
	else if (hit == 0x1){
		if (hit != 0xf){
			if (hit == light) {
				pick_target();
			}
		}
	}
	else if (hit == 0x2){
		if (hit != 0x000f){
			if (hit == light) {
				pick_target();
			}
		}

	}
	else if (hit == 0x4){
		if (hit != 0x000f){
			if (hit == light) {
				pick_target();
			}
		}
	}
	else if (hit == 0x8){
		if (hit != 0x000f){
			if (hit == light) {
				pick_target();
			}
		}
	}
}

int randomX(){
    return ((rand() % (4)) + 1) ;
}

void pick_target(void){
	GPIOB->BRR = 0xf00;
	nano_wait(50000000);
	target = randomX();
	if (target == 1){
		GPIOB->ODR = 0x0100;
	}
	if (target == 2){
		GPIOB->ODR = 0x0200;
	}
	if (target == 3){
		GPIOB->ODR = 0x0400;
	}
	if (target == 4){
		GPIOB->ODR = 0x0800;
	}
}













void init_exti() {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    SYSCFG->EXTICR[0] = SYSCFG->EXTICR[1] = 0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC | SYSCFG_EXTICR1_EXTI1_PC | SYSCFG_EXTICR1_EXTI2_PC | SYSCFG_EXTICR1_EXTI3_PC;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC;
    EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4;
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 | EXTI_IMR_MR3| EXTI_IMR_MR4;
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn | 1<<EXTI2_3_IRQn | 1<<EXTI4_15_IRQn;
}

void EXTI0_1_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR0) {
        EXTI -> PR = EXTI_PR_PR0;
        CheckIfTargetHit(0x1);
    }
    else if ((EXTI->PR) & EXTI_PR_PR1) {
        EXTI->PR = EXTI_PR_PR1;
        CheckIfTargetHit(0x2);

    }
}

void EXTI2_3_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR2) {
        EXTI->PR = EXTI_PR_PR2;
        CheckIfTargetHit(0x4);

    }
    else if ((EXTI->PR) & EXTI_PR_PR3) {
        EXTI->PR = EXTI_PR_PR3;
        CheckIfTargetHit(0x8);

    }
}

/*void EXTI4_15_IRQHandler() {
    EXTI->PR = EXTI_PR_PR4;
    GPIOC->BSRR = 0x10;
    nano_wait(1000000000);
    GPIOC->BRR = 0x10;

}*/
