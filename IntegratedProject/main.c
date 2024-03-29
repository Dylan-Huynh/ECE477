//============================================================================
//
//            THIS PROGRAM IS FOR GAME MODE TESTING
//
//
//				Notes: Gamemode 0 is menu, 1 is endless, 2 is timed
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
#define MESSAGESIZE 41
#define BLUETOOTHMESSAGESIZE 28

// Arrays for all of the LED Matrix Messages
uint8_t gametimeMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x39,
				 	 	 	 	 	 	0x00, 0x39, 0x00, 0x39, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
										0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30};
uint8_t titleMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
								 	 0x20, 0x00, 0x59, 0x00, 0x75, 0x00, 0x6b, 0x00, 0x69, 0x00, 0x20, 0x00, 0x4F, 0x00, 0x74, 0x00, 0x61, 0x00,
									 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t reactionTimeMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x20, 0x00,
										 0x6d, 0x00, 0x73, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t gamefinishedMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00,
								 	 	 	0x20, 0x00, 0x20, 0x00, 0x47, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4F,
											0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t tooSlowMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00,
								 	 	 	0x20, 0x00, 0x20, 0x20, 0x00, 0x54, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x53,
											0x00, 0x6c, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t timescoreMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x54,
				 	 	 	 	 	 	 0x00, 0x49, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
										 0x20, 0x00, 0x53, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x52, 0x00, 0x45};
uint8_t roundMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x52, 0x00, 0x6f, 0x00, 0x75, 0x00, 0x6e, 0x00, 0x64, 0x00,
										 0x20, 0x00, 0x30, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t waitMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x57, 0x00, 0x61, 0x00,
										 0x69, 0x00, 0x74, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t goMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x47, 0x00,
										 0x6f, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t progressMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x50,
				 	 	 	 	 	 	 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x20, 0x00, 0x74, 0x00, 0x6f, 0x00,
										 0x20, 0x00, 0x53, 0x00, 0x74, 0x00, 0x61, 0x00, 0x72, 0x00, 0x74};
uint8_t mode1Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x50, 0x00, 0x72, 0x00,
		 	 	 	 	 	 	 	 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x00, 0x20, 0x4d, 0x00, 0x6f, 0x00,
									 0x64, 0x00, 0x65, 0x00, 0x20};
uint8_t mode2Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	 0x54, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00,
									 0x65, 0x00, 0x20, 0x00, 0x20};
uint8_t mode3Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x52, 0x00, 0x65, 0x00,
		 	 	 	 	 	 	 	 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x6d, 0x00, 0x6f, 0x00,
									 0x64, 0x00, 0x65, 0x00, 0x20};
uint8_t mode4Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x4d, 0x00,
		 	 	 	 	 	 	 	 0x6f, 0x00, 0x76, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00,
									 0x65, 0x00, 0x20, 0x00, 0x20};
uint8_t count1Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	  0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x31, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
									  0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t count2Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	  0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x32, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
									  0x20, 0x00, 0x20, 0x00, 0x20};
uint8_t count3Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	  0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x33, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
									  0x20, 0x00, 0x20, 0x00, 0x20};


// Arrays for the BLE Messages
uint8_t bluetoothGame1[BLUETOOTHMESSAGESIZE] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};

uint8_t bluetoothGame2[BLUETOOTHMESSAGESIZE] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00};

uint8_t bluetoothGame3[BLUETOOTHMESSAGESIZE] = {0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03,
												0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00};

// Arrays for the chains of the Arrays, tbh not sure what is needed from here, only ledstrip I think
uint8_t ledstrip[LEDSIZE] = {0};
uint8_t redChain[LEDSIZE] = {0};
uint8_t blueChain[LEDSIZE] = {0};
uint8_t offChain[LEDSIZE] = {0};

// Signals to copy into ledstrip which generates certain color for a single LED
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
static int time_out = 0;

// gamemode 3 specific variables
static int wait_mode = 0; //wait mode for the gamemode_3, 0 if not lighting target, 1 when it lets player hit, 2 is pause mode
static int counter = 0; // Acts as both counter into time for lighting, and then the time it takes for player to go
static int counter2 = 0;
static int wait_time = 0; // Variable to hold to say how long the timer should wait to go into play mode
static int game2round = 0; // variable that says what round it is, exits when round 4 is finished

// gamemode 5 status variable
static uint8_t targets = 0xff; //-1 in hex;
static int how_many = 2;

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

//For the 7-segment, not necessary
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

// rand function to pick wait time for reaction test
int randomWait() {
	return ((rand() % 3000) + 1500);
}

//rand function to pick target
int randomX() {
	int pick = target;
	while (pick == target) pick = ((rand() % (4)) + 1);
    return pick;
}

<<<<<<< Updated upstream
//rand function to pick two-4 targets input is how many target to pick returns a 4, will be 5 in full mode bit hex number to decide targets
uint8_t randomMulti(int n) {
	if (n == 4){ //will be 5 in final
		return 0xf;
	}
	if (n == 3) {
		int num = rand() % 4;
		return (0xf & ~ 1 < num); // This picks a number between 0-3, then takes that target out
	}
	if (n == 2) {
		int num1 = rand() % 4;
		int num2 = rand() % 4;
		while (num2 == num1) num2 = rand() % 4;
		return (0x1 < num1 | 0x1 < num2);
	}
	/* For final
	if (n == 3) {
		int num1 = rand() % 4;
		int num2 = rand() % 4;
		while (num2 == num1) num2 = rand() % 4;
		return (0x1f & ~(0x1 < num1 | 0x1 < num2)); //chooses 3 of 5
	}
	 */
=======
void pick_multi_target() {
	GPIOB->BRR = 0xf00;
	nano_wait(50000000);
	targets = randMulti();
	GPIOB->BSRR = targets;
	Build_LED_Multi_Targets();
	nano_wait(10000);
>>>>>>> Stashed changes
}

// logic of setting up the picking up targets
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

void change_target(void){
	GPIOB->BRR = 0xf00;
	nano_wait(50000000);
	if (target == 4){ //should be 5
		GPIOB->BSRR = 0x0100;
		target = 1;
		Build_LED_Targets(ledstrip, green, off, off, off, off);
		nano_wait(10000);
	}
	else if (target == 1){
		GPIOB->BSRR = 0x0200;
		target = 2;
		Build_LED_Targets(ledstrip, off, green, off, off, off);
		nano_wait(10000);
	}
	else if (target == 2){
		GPIOB->BSRR = 0x0400;
		target = 3;
		Build_LED_Targets(ledstrip, off, off, green, off, off);
		nano_wait(10000);
	}
	else if (target == 3){
		GPIOB->BSRR = 0x0800;
		target = 4;
		Build_LED_Targets(ledstrip, off, off, off, green, off);
		nano_wait(10000);
	}
	/*
	else if (target == 4){
		GPIOB->BSRR = 0x1000;
		target = 5;
		Build_LED_Targets(ledstrip, off, off, off, off, green);
		nano_wait(10000);
	}
	*/
}

// Checks if Target is hit by comparing lighting to see if it is the same as the hit mark
void CheckIfTargetHit(uint32_t hit) {
    uint32_t light = GPIOB->ODR >> 8; // Makes a check variable of light output
<<<<<<< Updated upstream
    time_out = 0;
    if (gamemode == 3) {
=======
	if (gamemode == 5) {
		if (light & hit) { //If right hit, should turn off target, set BRR, and if no targets remain pick new ones
			targets = targets & ~hit;
			GPIOB->BRR = hit << 8;
			if (light == 0) {
				pick_multi_target()
			}
			else {
				Build_LED_Multi_Targets(green);
			}
		}

	}
    else if (gamemode == 3) {
>>>>>>> Stashed changes
    	if (wait_mode == 0) { // do nothing if waiting
    		return;
    	}
    		else if (wait_mode == 2) { // tell the round number, then enter wait mode
    			roundMessage[34] = game2round + 49;
    			send_LED_message(roundMessage);

    			nano_wait(1000000000);
    			send_LED_message(waitMessage);
    			wait_time = randomWait();
    			wait_mode = 0;
    			game2round++;
    			TIM3->CR1 |= TIM_CR1_CEN;
    		}
    		else { //hit mode
    			if (hit == light) {
    				TIM3->CR1 &= ~TIM_CR1_CEN;
    				Build_LED_Targets(ledstrip, off, off, off, off, off);
    				wait_mode = 2;
    				change_reaction_time(counter2);
    				send_LED_message(reactionTimeMessage);
    				counter2 = 0;
    				nano_wait(1000000000);
    				if (game2round == 5) {
    					send_LED_message(gamefinishedMessage);
    					nano_wait(1000000000);
    					NVIC_SystemReset();
    				}
    				send_LED_message(progressMessage);

    			}
    		}
    }
    else if (target == -1) {
        pick_target(); // If in no target is set, pick one
    }
    else if (hit == light) { // Works if target picked is target hit
        pick_target(); //Choose new target
        if (gamemode == 2 || gamemode == 4) {
            /*
            if (!((TIM2->CR1) & TIM_CR1_CEN)) {
                TIM2->CR1 |= TIM_CR1_CEN;
            }
            */
            targets_hit++;
            if (gamemode == 4) {
            	if (wait_time - 50 <= 0) wait_time = 75;
            	wait_time = wait_time - 50;

            }
        }
    }
    // suspect logic for if none is found
}

// Uses Usart to send input array, which is a size 42 array though to draw on the LED Matrix
void send_LED_message(uint8_t* arr) {
	for(int i =0; i <= MESSAGESIZE; i++) {
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = arr[i];
	}
}

// makes initializations for menu mode
void menu_setup() {

    GPIOB->ODR = 0;
    nano_wait(1000000000);
    GPIOB->BSRR = 0x300;
    Build_LED_Targets(ledstrip, blue, blue, blue, blue, off);
	send_LED_message(titleMessage);
	nano_wait(1000000000);
	gamemode = 0;
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

//This picks which targets are off and colored
Build_LED_Multi_Targets(uint8_t *color) {
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;
	int targetLEDsize = NUMLED / 5;
	int n = 0;
	uint8_t bit;
	while (n < 5) {
		// Isolates bits with left shift and right shift
		bit = targets < (8 - n + 1) > 7
		if (bit) {
			for(int i = targetLEDsize * n; i < targetLEDsize * (n + 1); i++) {
				for(int j = 0; j < 24; j++){
					ledstrip[i*24 + j] = color[j];
				}
			}
		}
		else {
			for(int i = targetLEDsize + n; i < targetLEDsize * (n + 1); i++) {
				for(int j = 0; j < 24; j++){
					ledstrip[i*24 + j] = off[j];
				}
			}
		}
		n++;
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

void init_usart() {

    // enable clock for USART for 1 and 2
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // disable USART2 by turning off UE bit
    USART1->CR1 &= ~(USART_CR1_UE);
    USART2->CR1 &= ~(USART_CR1_UE);


    // set word size of 8 bits
    USART1->CR1 &= ~(USART_CR1_M);
    USART2->CR1 &= ~(USART_CR1_M);

    // set for one stop bit
    USART1->CR2 &= ~(USART_CR2_STOP);
    USART2->CR2 &= ~(USART_CR2_STOP);

    // set it for no parity
    USART1->CR1 &= ~(USART_CR1_PCE);
    USART2->CR1 &= ~(USART_CR1_PCE);

    // use 16x oversampling
    USART1->CR1 &= ~(USART_CR1_OVER8);
    USART2->CR1 &= ~(USART_CR1_OVER8);

    // baud rate of 19200 and 9600 (19.2k/9.6k baud)
    USART1->BRR = 0x9c4; // look at table 106 of the family reference manual
    USART2->BRR = 0x1388; // look at table 106 of the family reference manual

    // enable transmitter and the receiver by setting the TE bit
    USART1->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_TE;
    //USART2->CR1 |= USART_CR1_RE;

    // enable the USART
    USART1->CR1 |= USART_CR1_UE;
    USART2->CR1 |= USART_CR1_UE;

    // wait for the TE and RE bits to be acknowledged by checking the TEACK and bits are both set in the ISR
    // indicates that the USART is ready to transmit
    USART1->ISR |= USART_ISR_TEACK;
    USART2->ISR |= USART_ISR_TEACK;
    //USART2->ISR |= USART_ISR_REACK;

}

// This function changes the score parts of the game array
void change_score(int n) {

	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;
	int n_thousand = n / 1000;

	gametimeMessage[40] = n_one + 48;
	gametimeMessage[38] = n_ten + 48;
	gametimeMessage[36] = n_hundred + 48;
	gametimeMessage[34] = n_thousand + 48;
}

void change_reaction_time(int n){ //Loads reaction time into array, also loads it into bluetooth
	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;
	int n_thousand = n / 1000;

	reactionTimeMessage[28] = n_one + 48;
	reactionTimeMessage[26] = n_ten + 48;
	reactionTimeMessage[24] = n_hundred + 48;
	reactionTimeMessage[22] = n_thousand + 48;
	int shifter = 0;
	switch (game2round) {
		case 1:
			shifter = 2;
			break;
		case 2:
			shifter = 7;
			break;
		case 3:
			shifter = 12;
			break;
		case 4:
			shifter = 17;
			break;
		case 5:
			shifter = 22;
			break;
	}
	bluetoothGame3[0 + shifter] = n_thousand;
	bluetoothGame3[0 + shifter + 1] = n_hundred;
	bluetoothGame3[0 + shifter + 2] = n_ten;
	bluetoothGame3[0 + shifter + 3] = n_one;

}

// This function changes the time part of the array
void change_time(int n) {
	int n_one = n % 10;
	int n_ten = (n % 100) / 10;
	int n_hundred = (n % 1000) / 100;

	gametimeMessage[18] = n_one + 48;
	gametimeMessage[16] = n_ten + 48;
	gametimeMessage[14] = n_hundred + 48;
}

// Changes the parameters centered around mode2, and then sends it out
void load_and_send_bluetooth2() {
	bluetoothGame2[2] = target1_hit;
	bluetoothGame2[3] = target1_miss;
	bluetoothGame2[5] = target2_hit;
	bluetoothGame2[6] = target2_miss;
	bluetoothGame2[9] = target3_hit;
	bluetoothGame2[10] = target3_miss;
	bluetoothGame2[13] = target4_hit;
	bluetoothGame2[14] = target4_miss;
	bluetoothGame2[17] = target5_hit;
	bluetoothGame2[18] = target5_miss;
	bluetoothGame2[21] = blade_hit;
	bluetoothGame2[22] = blade_miss;
	for(int i =0; i <= BLUETOOTHMESSAGESIZE; i++) {
		while(!(USART2->ISR & USART_ISR_TXE));
		USART2->TDR = bluetoothGame2[i];
	}
}

void init_tim3() { // game 3 and 4 timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CR1 &= ~TIM_CR1_CEN;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->PSC = 48 - 1; // Want to make it go 100 times a second/ so PSC * ARR = 48000
	TIM3->ARR = 1000 - 1;
	NVIC->ISER[0] = 1 << TIM3_IRQn;
	NVIC_SetPriority(TIM3_IRQn, 28);
}

void TIM3_IRQHandler() {
    TIM3->SR = ~TIM_SR_UIF;
    if (gamemode == 3) {
        if (wait_mode == 0) {
        	if (counter == wait_time) {
        		wait_mode = 1;
        		counter = 0;
        		pick_target();
        		send_LED_message(goMessage);
        	}
        	else {
        		counter++;
        	}
        }
        else {
        	// The hit logic will be conducted by EXTI, so we don't need to check that here, but simply increment the timer with a timeout
        	counter2++;
        	if (counter2 == 9999) {
        		TIM3->CR1 &= ~TIM_CR1_CEN;
        		send_LED_message(tooSlowMessage);
        		nano_wait(1000000000);
        		NVIC_SystemReset();
        	}
        }
    }
    else { //gamemode # 4
    	if (counter == wait_time) {
    		change_target();
    		counter = 0;
    	}
    	else {
    		counter++;
    	}
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

/* Timer logic for downcounting timer
 * If time remains, it will run through and decrement and put that on the screen, plus update the screen*/
void TIM2_IRQHandler() {
    TIM2->SR = ~TIM_SR_UIF;
    if (gamemode == 1) {
    	if (time_out == 10) {
    		send_LED_message(tooSlowMessage);
    		nano_wait(1000000000);
    		NVIC_SystemReset();
    	}
    	time_left++;
    	time_out++;
    	change_time(time_left);
    	change_score(targets_hit);
    	send_LED_message(gametimeMessage);

    }
    else {
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
    	    TIM2->CR1 &= ~TIM_CR1_CEN;
    	    TIM3->CR1 &= ~TIM_CR1_CEN;
    	    Build_LED_Targets(ledstrip, red, red, red, red, red);
    	    send_LED_message(gamefinishedMessage);
    	    nano_wait(1000000000);
    		send_LED_message(gametimeMessage);
    		load_and_send_bluetooth2();
    	    //display_handle(targets_hit);
    	    nano_wait(1000000000);
    	    NVIC_SystemReset();
    	    menu_setup();

    	}
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


// Logic for hitting Targets 1 and 2
void EXTI0_1_IRQHandler() {
    if ((EXTI->PR) & EXTI_PR_PR0) { //T
        EXTI -> PR = EXTI_PR_PR0;
        if (gamemode == -1) {}
        else if (gamemode == 0) { // menu
        	// Starts endless mode
            gamemode = 1;
            GPIOB->BRR = 0xFFF;
            Build_LED_Targets(ledstrip, white, white, white, white, white);
            send_LED_message(mode1Message);
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
        	//Starts timed mode
            targets_hit = 0;
            gamemode = 2;
            GPIOB->BRR = 0xFFF;
            Build_LED_Targets(ledstrip, white, white, white, white, white);
			send_LED_message(mode2Message);
            nano_wait(1000000000);

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
        else if (gamemode == 0) { // menu
        	//Starts timed mode
            gamemode = 3;
            GPIOB->BRR = 0xFFF; //not sure if this stuff is necessary
            Build_LED_Targets(ledstrip, white, white, white, white, white);
            send_LED_message(mode3Message);
            nano_wait(1000000000);
        	roundMessage[34] = game2round + 49;
            //display_handle(time_left);
        	wait_mode = 2;
            //TIM3->CR1 |= TIM_CR1_CEN;
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            send_LED_message(progressMessage);
            //pick_target();
        }
        else if (gamemode) {
            CheckIfTargetHit(0x4);
        }
    }
    else if ((EXTI->PR) & EXTI_PR_PR3) {
        EXTI->PR = EXTI_PR_PR3;
        if (gamemode == -1) {}
        else if (gamemode == 0) { // menu
            //Starts moving mode
        	targets_hit = 0;
        	gamemode = 4;
            GPIOB->BRR = 0xFFF; //not sure if this stuff is necessary
            Build_LED_Targets(ledstrip, white, white, white, white, white);
            send_LED_message(mode4Message);
            nano_wait(1000000000);
            //display_handle(time_left);
            wait_time = 500;
            TIM3->CR1 |= TIM_CR1_CEN;
            TIM2->CR1 |= TIM_CR1_CEN;
            pick_target();

        }
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
	// Only enable port B for the USART, switch later
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	//GPIOB->MODER &= ~0xffffffff;
	//GPIOB->MODER |= 0x00555555; // PB0-11 outputs
    GPIOB->MODER |= 0x2000; // Enables PB6 for USART1_TX
    GPIOB->AFR[1] &= ~0xF << 24; //AF0 for PB6
	GPIOA->MODER &= ~0x000fffff;
	GPIOA->MODER |= 0x00020000; // PA8 alternate function
    GPIOA->MODER |= 0x20; // Enables PA2
    GPIOA->AFR[0] |= 1<<8; //Alternate Function 1 for PA2, subject to change
	GPIOA->AFR[1] &= ~0x0000FFFF;
	GPIOA->AFR[1] |= 0x00002222;
	GPIOB->PUPDR |= 0xaaaa;
	GPIOC->MODER &= ~0xffffffff; // PC0-3 inputs
}

int main() {
	init_tim1();
	init_dma();
    init_tim2();
    init_tim3();
    init_pins();
    init_usart();
    GPIOB->ODR = 0;
    init_exti();
    menu_setup();
    for(;;) {
        asm volatile ("wfi");
    }
    return 0;
}
