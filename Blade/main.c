//============================================================================
//
//            THIS PROGRAM IS FOR GAME MODE TESTING
//
//============================================================================

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define GAMETIME 4

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

int randomX() {
    return ((rand() % (5)) + 1) ;
}

void pick_target(void) {
    GPIOC->BRR = 0x1f;
    nano_wait(50000000);
    target = randomX();
    if (target == 1){
        GPIOC->ODR = 0x1;
    }
    if (target == 2){
        GPIOC->ODR = 0x2;
    }
    if (target == 3){
        GPIOC->ODR = 0x4;
    }
    if (target == 4){
        GPIOC->ODR = 0x8;
    }
    if (target == 5){
        GPIOC->ODR = 0x10;
    }
}

void CheckIfTargetHit(uint32_t hit) {
    uint32_t light = GPIOC->ODR;
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
    GPIOA->ODR = 0;
    GPIOC->ODR = 0;
    nano_wait(1000000000);
    GPIOC->ODR = 0x3;
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
    }
    else {
        time_left = GAMETIME;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        GPIOC->ODR = 0x1f;
        nano_wait(500000000);
        menu_setup();
    }
}

void init_exti() {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
    SYSCFG->EXTICR[0] = SYSCFG->EXTICR[1] = SYSCFG->EXTICR[3] = 0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB | SYSCFG_EXTICR1_EXTI1_PB | SYSCFG_EXTICR1_EXTI2_PB | SYSCFG_EXTICR1_EXTI3_PB;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI15_PB;
    EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4;
    EXTI->RTSR |= EXTI_RTSR_TR15;
    EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 | EXTI_IMR_MR3| EXTI_IMR_MR4 | EXTI_IMR_MR15;
    NVIC->ISER[0] = 1<<EXTI0_1_IRQn | 1<<EXTI2_3_IRQn | 1<<EXTI4_15_IRQn;
}

void EXTI0_1_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR0) {
        EXTI -> PR = EXTI_PR_PR0;
        if (gamemode == 0) { // menu
            gamemode = 1;
            GPIOA->ODR = 0x100;
            GPIOC->ODR = 0;
            nano_wait(200000000);
            GPIOC->ODR = 0x1f;
            nano_wait(100000000);
            GPIOC->ODR = 0;
            nano_wait(100000000);
            GPIOC->ODR = 0x1f;
            nano_wait(100000000);
            GPIOC->ODR = 0;
            nano_wait(500000000);
            pick_target();
        }
        else if (gamemode) { // endless
            CheckIfTargetHit(0x1);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR1) {
        EXTI->PR = EXTI_PR_PR1;
        if (gamemode == 0) { // menu
            targets_hit = 0;
            gamemode = 2;
            GPIOA->ODR = 0x200;
            GPIOC->ODR = 0;
            nano_wait(200000000);
            GPIOC->ODR = 0x1f;
            nano_wait(100000000);
            GPIOC->ODR = 0;
            nano_wait(100000000);
            GPIOC->ODR = 0x1f;
            nano_wait(100000000);
            GPIOC->ODR = 0;
            nano_wait(500000000);
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
        if (gamemode) {
            CheckIfTargetHit(0x4);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR3) {
        EXTI->PR = EXTI_PR_PR3;
        if (gamemode) {
            CheckIfTargetHit(0x8);
        }
    }
}

void EXTI4_15_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR4) {
        EXTI->PR = EXTI_PR_PR4;
        if (gamemode) {
            CheckIfTargetHit(0x10);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR15) {
        EXTI->PR = EXTI_PR_PR15;
        if (gamemode != 0) {
            menu_setup();
        }
    }
}

void init_pins() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
    GPIOA->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
    GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 |
                    GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
                    GPIO_MODER_MODER4 | GPIO_MODER_MODER15);
    GPIOC->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |
                    GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
                    GPIO_MODER_MODER4_0;
}

void main() {
    init_pins();
    init_tim2();
    init_exti();
    menu_setup();
    for(;;) {
        asm volatile ("wfi");
    }
}
