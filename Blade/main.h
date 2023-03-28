#ifndef __MAIN_H
#define __MAIN_H_

typedef uint32_t u32;
void pick_target(void);
void CheckIfTargetHit(uint32_t hit);
void enable_TIM2(void);

void init_exti();
void EXTI0_1_IRQHandler();
void EXTI2_3_IRQHandler();
void EXTI4_15_IRQHandler();
#endif
