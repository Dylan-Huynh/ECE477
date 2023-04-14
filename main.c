//============================================================================
//
//            THIS PROGRAM IS FOR GAME MODE TESTING
//
//
//				Notes: Gamemode 0 is menu, 1 is endless, 2 is timed,
//				3 is reaction, 4 is moving, 5 is multi
//
//============================================================================

#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define GAMETIME 20
#define ONE (38)
#define ZERO (19)
#define RS (0)
#define NUMLED (185)
#define NUMTARGET (5)
#define LEDSIZE (24*NUMLED + 600)
#define MESSAGESIZE 41
#define BLUETOOTHMESSAGESIZE 28

// Arrays for all of the LED Matrix Messages message commented above
// XXX      XXXX
uint8_t gametimeMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x39,
				 	 	 	 	 	 	0x00, 0x39, 0x00, 0x39, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
										0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30};
// Yuki Ota
uint8_t titleMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
								 	 0x20, 0x00, 0x59, 0x00, 0x75, 0x00, 0x6b, 0x00, 0x69, 0x00, 0x20, 0x00, 0x4F, 0x00, 0x74, 0x00, 0x61, 0x00,
									 0x20, 0x00, 0x20, 0x00, 0x20};
// XXX ms 
uint8_t reactionTimeMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x20, 0x00,
										 0x6d, 0x00, 0x73, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Game Over
uint8_t gamefinishedMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00,
								 	 	 	0x20, 0x00, 0x20, 0x00, 0x47, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x20, 0x00, 0x4F,
											0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Too Slow
uint8_t tooSlowMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00,
								 	 	 	0x20, 0x00, 0x20, 0x20, 0x00, 0x54, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x53,
											0x00, 0x6c, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Time      Score not used
uint8_t timescoreMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x54,
				 	 	 	 	 	 	 0x00, 0x49, 0x00, 0x4d, 0x00, 0x45, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
										 0x20, 0x00, 0x53, 0x00, 0x43, 0x00, 0x4f, 0x00, 0x52, 0x00, 0x45};
// Round X
uint8_t roundMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x52, 0x00, 0x6f, 0x00, 0x75, 0x00, 0x6e, 0x00, 0x64, 0x00,
										 0x20, 0x00, 0x30, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Wait
uint8_t waitMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x57, 0x00, 0x61, 0x00,
										 0x69, 0x00, 0x74, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Go
uint8_t goMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20,
				 	 	 	 	 	 	 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x47, 0x00,
										 0x6f, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
// Press to Start
uint8_t progressMessage[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x50,
				 	 	 	 	 	 	 0x00, 0x72, 0x00, 0x65, 0x00, 0x73, 0x00, 0x73, 0x00, 0x20, 0x00, 0x74, 0x00, 0x6f, 0x00,
										 0x20, 0x00, 0x53, 0x00, 0x74, 0x00, 0x61, 0x00, 0x72, 0x00, 0x74};
// Mode 1
uint8_t mode1Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x50, 0x00, 0x72, 0x00,
		 	 	 	 	 	 	 	 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 0x00, 0x20, 0x4d, 0x00, 0x6f, 0x00,
									 0x64, 0x00, 0x65, 0x00, 0x20};
// Mode 2
uint8_t mode2Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	 0x54, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00,
									 0x65, 0x00, 0x20, 0x00, 0x20};
// Mode 3
uint8_t mode3Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x52, 0x00, 0x65, 0x00,
		 	 	 	 	 	 	 	 0x61, 0x00, 0x63, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00, 0x20, 0x00, 0x6d, 0x00, 0x6f, 0x00,
									 0x64, 0x00, 0x65, 0x00, 0x20};
// Mode 4
uint8_t mode4Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
									 0x4d, 0x00, 0x75, 0x00, 0x6c, 0x00, 0x74, 0x00, 0x69, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00,
									 0x65, 0x00, 0x20, 0x00, 0x20};
// Mode 5
uint8_t mode5Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x4d, 0x00,
		 	 	 	 	 	 	 	 0x6f, 0x00, 0x76, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x6f, 0x00, 0x64, 0x00,
									 0x65, 0x00, 0x20, 0x00, 0x20};
// 1
uint8_t count1Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	  0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x31, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
									  0x20, 0x00, 0x20, 0x00, 0x20};
// 2
uint8_t count2Message[MESSAGESIZE] = {0x42, 0x54, 0x30, 0x31, 0x35, 0x01, 0x00, 0x00, 0x21, 0x41, 0x41, 0x33, 0x41, 0x00, 0x20, 0x00, 0x20, 0x00,
		 	 	 	 	 	 	 	  0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x32, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00,
									  0x20, 0x00, 0x20, 0x00, 0x20};
// 3
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

// Arrays for the chains of the Arrays, tbh not sure what is needed from here, only ledstrip is used I think
uint8_t ledstrip[LEDSIZE] = {0};
uint8_t redChain[LEDSIZE] = {0};
uint8_t blueChain[LEDSIZE] = {0};
uint8_t offChain[LEDSIZE] = {0};

// Signals to copy into ledstrip which generates certain color for a single LED hex representations labeled above
// 0x000000
uint8_t off[24] = {19,19,19,19,19,19,19,19,
				  19,19,19,19,19,19,19,19,
				  19,19,19,19,19,19,19,19};
// 0x00ff00
uint8_t red[24] = {19,19,19,19,19,19,19,19,
					38,38,38,38,38,38,38,38,
					19,19,19,19,19,19,19,19};
// 0xff0000
uint8_t green[24] = {38,38,38,38,38,38,38,38,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
// 0x0000ff
uint8_t blue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  38,38,38,38,38,38,38,38};
// 0xffffff
uint8_t white[24] = {38,38,38,38,38,38,38,38,
						38,38,38,38,38,38,38,38,
						38,38,38,38,38,38,38,38};
// 0x00ffff
uint8_t purple[24] = {19,19,19,19,19,19,19,19,
					38,38,38,38,38,38,38,38,
					38,38,38,38,38,38,38,38};

// 0x008000
uint8_t halfred[24] = {19,19,19,19,19,19,19,19,
					38,19,19,19,19,19,19,19,
					19,19,19,19,19,19,19,19};
// 0x800000
uint8_t halfgreen[24] = {38,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
// 0x000080
uint8_t halfblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  38,19,19,19,19,19,19,19};
// 0x808080
uint8_t halfwhite[24] = {38,19,19,19,19,19,19,19,
						38,19,19,19,19,19,19,19,
						38,19,19,19,19,19,19,19};

// 0x002800
uint8_t quarterred[24] = {19,19,19,19,19,19,19,19,
					19,19,38,19,38,19,19,19,
					19,19,19,19,19,19,19,19};
// 0x280000
uint8_t quartergreen[24] = {19,19,38,19,38,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
// 0x000028
uint8_t quarterblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,38,19,38,19,19,19};
// 0x282828
uint8_t quarterwhite[24] = {19,19,38,19,38,19,19,19,
						19,19,38,19,38,19,19,19,
						19,19,38,19,38,19,19,19};

// 0x000100
uint8_t lowestred[24] = {19,19,19,19,19,19,19,19,
					19,19,19,19,19,19,19,38,
					19,19,19,19,19,19,19,19};
// 0x010000
uint8_t lowestgreen[24] = {19,19,19,19,19,19,19,38,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,19};
// 0x000001
uint8_t lowestblue[24] = { 19,19,19,19,19,19,19,19,
				  	  19,19,19,19,19,19,19,19,
					  19,19,19,19,19,19,19,38};
// 0x010101
uint8_t lowestwhite[24] = {19,19,19,19,19,19,19,38,
						19,19,19,19,19,19,19,38,
						19,19,19,19,19,19,19,38};

// Status variables
static int target = -1; // neutral state not used
static int gamemode = -1; // menu
static int time_left = GAMETIME; // time for gamemode_2 - 1 second
static int targets_hit = 0; // amount of targets hit in gamemode_2
static int time_out = 0; // counts up to a number in timer 2 and resets system of meets it

// gamemode 3 specific variables
static int wait_mode = 0; //wait mode for the gamemode_3, 0 if not lighting target, 1 when it lets player hit, 2 is pause mode
static int counter = 0; // Acts as both counter into time for lighting, and then the time it takes for player to go
static int counter2 = 0;
static int wait_time = 0; // Variable to hold to say how long the timer should wait to go into play mode
static int game2round = 0; // variable that says what round it is, exits when round 4 is finished

static uint8_t targets = 0xff; //-1 in hex, bit code of the 5 targets state from 5 most right bits
static int how_many = 2; // decides how many targets to light up in  multi target mode

// waits for a a time decided by n
static void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

// rand function to pick wait time for reaction test
int randomWait() {
	return ((rand() % 3000) + 1500);
}

//rand function to pick 1-5 targets input is how many target to pick
uint8_t randomMulti(int n) {
	if (n == 5){
		return 0x1f;
	}
	if (n == 4) {
		int num = rand() % 5;
		return (0x1f & ~(1 << num)); // This picks a number between 0-3, then takes that target out will be 4 in final
	}
	if (n == 3) {
		int num1 = rand() % 5;
		int num2 = rand() % 5;
		while (num2 == num1) num2 = rand() % 5;
		return (0x1f & ~((0x1 << num1) | (0x1 << num2))); //chooses 3 of 5
	}
	if (n == 2) {
		int num1 = rand() % 5;
		int num2 = rand() % 5;
		while (num2 == num1) num2 = rand() % 5;
		return ((0x1 << num1) | (0x1 << num2));
	}
	else {
		int pick = rand() % (5);
		while (0x1 << pick == targets) pick = rand() % (5);
		return 0x1 << pick;
	}
}

// idk if this is ever called, delete this
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

// Goes through all LEDs and sets according to the 5 color inputs
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

//This picks which targets are off and colored
void Build_LED_Multi_Targets(uint8_t *color) {
	nano_wait(50000000);
	uint8_t bit1 = targets & (0x1 << 0);
	uint8_t bit2 = targets & (0x1 << 1);
	uint8_t bit3 = targets & (0x1 << 2);
	uint8_t bit4 = targets & (0x1 << 3);
	uint8_t bit5 = targets & (0x1 << 4);
	uint8_t *color_or_off1;
	uint8_t *color_or_off2;
	uint8_t *color_or_off3;
	uint8_t *color_or_off4;
	uint8_t *color_or_off5;
	color_or_off1 = bit1? color : off;
	color_or_off2 = bit2? color : off;
	color_or_off3 = bit3? color : off;
	color_or_off4 = bit4? color : off;
	color_or_off5 = bit5? color : off;
	Build_LED_Targets(ledstrip, color_or_off1, color_or_off2, color_or_off3, color_or_off4, color_or_off5);

	//Build_LED_Multi_Targets(halfgreen);
	nano_wait(10000);
}

// Uses Usart to send input array, which is a size 42 array though to draw on the LED Matrix USART 1 is GPIOB6
void send_LED_message(uint8_t* arr) {
	for(int i =0; i <= MESSAGESIZE; i++) {
		while(!(USART1->ISR & USART_ISR_TXE));
		USART1->TDR = arr[i];
	}
}

void countdown() {
	send_LED_message(count3Message);
	nano_wait(1000000000);
	send_LED_message(count2Message);
	nano_wait(1000000000);
	send_LED_message(count1Message);
	nano_wait(1000000000);
}

// This function changes the score parts of the game array
void change_score(int n) {

	//Separates the number into it's components
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
	//Might need to fix this
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


// Sets random target based on number wanted then calls function to light leds based on targets
// Spaghetti code, check all the inputs for if it is set then rus it through the build led function
void pick_multi_target(int n, uint8_t *color) {
	nano_wait(50000000);
	targets = randomMulti(n);
	uint8_t bit1 = targets & (0x1 << 0);
	uint8_t bit2 = targets & (0x1 << 1);
	uint8_t bit3 = targets & (0x1 << 2);
	uint8_t bit4 = targets & (0x1 << 3);
	uint8_t bit5 = targets & (0x1 << 4);
	uint8_t *color_or_off1;
	uint8_t *color_or_off2;
	uint8_t *color_or_off3;
	uint8_t *color_or_off4;
	uint8_t *color_or_off5;
	color_or_off1 = bit1? color : off;
	color_or_off2 = bit2? color : off;
	color_or_off3 = bit3? color : off;
	color_or_off4 = bit4? color : off;
	color_or_off5 = bit5? color : off;
	Build_LED_Targets(ledstrip, color_or_off1, color_or_off2, color_or_off3, color_or_off4, color_or_off5);

	//Build_LED_Multi_Targets(halfgreen);
	nano_wait(10000);
}

// logic of setting up the picking up targets
void pick_target(void){
	nano_wait(50000000);
	targets = randomMulti(1);
	if (targets == 0x1){
		Build_LED_Targets(ledstrip, green, off, off, off, off);
		nano_wait(10000);
	}
	if (targets == 0x2){
		Build_LED_Targets(ledstrip, off, green, off, off, off);
		nano_wait(10000);
	}
	if (targets == 0x4){
		Build_LED_Targets(ledstrip, off, off, green, off, off);
		nano_wait(10000);
	}
	if (targets == 0x8){
		Build_LED_Targets(ledstrip, off, off, off, green, off);
		nano_wait(10000);
	}
	if (targets == 0x10){
		Build_LED_Targets(ledstrip, off, off, off, off, green);
		nano_wait(10000);
	}

}

//When called, moves target up 1
void change_target(void){
	nano_wait(50000000);
	if (targets == 0x1){ //1->2
		targets = 0x2;
		Build_LED_Targets(ledstrip, off, green, off, off, off);
		nano_wait(10000);
	}
	else if (targets == 0x2){ //2->3
		targets = 0x4;
		Build_LED_Targets(ledstrip, off, off, green, off, off);
		nano_wait(10000);
	}
	else if (targets == 0x4){ //3->4
		targets = 0x8;
		Build_LED_Targets(ledstrip, off, off, off, green, off);
		nano_wait(10000);
	}

	else if (target == 0x8){ //4->5
		target = 0x10;
		Build_LED_Targets(ledstrip, off, off, off, off, green);
		nano_wait(10000);
	}
	else if (targets == 0x10){ //5->1
		targets = 0x1;
		Build_LED_Targets(ledstrip, green, off, off, off, off);
		nano_wait(10000);
	}
}

// Checks if Target is hit by comparing lighting to see if it is the same as the hit mark
void CheckIfTargetHit(uint32_t hit) {
    uint32_t light = targets; // Makes a check variable of light output
    time_out = 0; // Shows that device is not idling
    if (gamemode == 5) {
    	if (light & hit) { //If right hit, should turn off target, set BRR, and if no targets remain pick new ones
    		targets = targets & ~hit;
    		targets_hit++;
    		if (targets == 0) {
    			// Adds more targets if score is higher
    			if (targets_hit < 14) {
    				how_many = 5;
    			}
    			else if (targets_hit < 9) {
    			    how_many = 4;
    			}
    			else if (targets_hit < 4) {
    				how_many = 3;
    			}
    			pick_multi_target(how_many, green);
    		}
    		else {
    			Build_LED_Multi_Targets(halfgreen);
    		}
    	}

    }
    else if (gamemode == 3) { //Reaction time mode
    	if (wait_mode == 0) { // do nothing if waiting
    		return;
    	}
    	else if (wait_mode == 2) { // tell the round number, then enter wait mode
    		roundMessage[34] = game2round + 49; // Increments round
    		// Does all the cutscene stuff like saying round then counting down then proceeds
    		send_LED_message(roundMessage);
    		nano_wait(1000000000);
    		countdown();
    		send_LED_message(waitMessage);
    		wait_time = randomWait();
    		wait_mode = 0;
    		game2round++;
    		TIM3->CR1 |= TIM_CR1_CEN;
    	}
    	else { //wait mode == 1
    		if (hit == light) { //hit happened
    		    EXTI->FTSR &= ~(EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4); // Stops pending interrupts
    			TIM3->CR1 &= ~TIM_CR1_CEN;
    			Build_LED_Targets(ledstrip, off, off, off, off, off);
    			wait_mode = 2; // Puts it in hold mode
    			change_reaction_time(counter2);
    			send_LED_message(reactionTimeMessage); // Reports reaction time
    			counter2 = 0; // Resets reaction time
    			nano_wait(1000000000);
    			if (game2round == 5) {
    				send_LED_message(gamefinishedMessage);
    				nano_wait(1000000000);
    				NVIC_SystemReset();
    			}
    			send_LED_message(progressMessage);
    		    EXTI->FTSR |= EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4;
    		}
    	}
    }
    else if (targets == 0xff) {
        pick_target(); //don't think this happens but if in no target is set, pick one
    }
    else if (hit == light) { // For gamemodes 1, 2, and 5
        pick_target(); //Choose new target
        targets_hit++;
        if (gamemode == 4) {
            if (wait_time - 50 <= 0) wait_time = 75;
            wait_time = wait_time - 50;
        }
    }
    // suspect logic for if none is found
}



// makes initializations for menu mode
void menu_setup() {

    //nano_wait(1000000000);
	send_LED_message(titleMessage);
    Build_LED_Targets(ledstrip, halfblue, halfblue, halfblue, halfblue, halfblue);
	nano_wait(1000000000);
	// Puts menu then accepts outputs
	gamemode = 0;
}

// Parameters for DMA Timer1
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

// DMA paramters being set
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

//Sets parameters for USART 1 and 2
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


// Changes the parameters centered around mode2, and then sends it out def need to change this
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
    if (gamemode == 3) { // Reaction time mode
        if (wait_mode == 0) {
        	if (counter == wait_time) { // if it's time for the user to strike
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
			    EXTI->FTSR &= ~(EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4);
        		send_LED_message(tooSlowMessage);
        		nano_wait(1000000000);
        		NVIC_SystemReset();
        	}
        }
    }
    else { //gamemode # 4 if the counter counts up to the designated wait time switch targets
    	if (counter == wait_time) {
    		change_target();
    		counter = 0;
    	}
    	else {
    		counter++;
    	}
    }

}

//Timer 2 specifications
void init_tim2() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->PSC = 47999;
    TIM2->ARR = 999;
    NVIC->ISER[0] = 1 << TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn, 29);
}

/* Timer logic for downcounting timer, upcounts if gamemode 1
 * If time remains, it will run through and decrement and put that on the screen, plus update the screen*/
void TIM2_IRQHandler() {
    TIM2->SR = ~TIM_SR_UIF;
    if (gamemode == 1) {
    	if (time_out == 5) {
    		send_LED_message(tooSlowMessage);
		    EXTI->FTSR &= ~(EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4);
    		nano_wait(1000000000);
    		NVIC_SystemReset();
    	}
    	time_left++;
    	time_out++;
    	change_time(time_left);
    	change_score(targets_hit);
    	send_LED_message(gametimeMessage);

    }
    else { //gamemode 2 4 and 5
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
		    EXTI->FTSR &= ~(EXTI_FTSR_TR0 | EXTI_FTSR_TR1 | EXTI_FTSR_TR2 | EXTI_FTSR_TR3 | EXTI_FTSR_TR4);
    	    TIM2->CR1 &= ~TIM_CR1_CEN;
    	    TIM3->CR1 &= ~TIM_CR1_CEN;
    	    Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
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
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC;
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
        	time_left = 0;
            Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
            send_LED_message(mode1Message);
            nano_wait(1000000000);
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            countdown();
            gamemode = 1;
            TIM2->CR1 |= TIM_CR1_CEN;
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
            Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
			send_LED_message(mode2Message);
            nano_wait(1000000000);
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            countdown();
            //display_handle(time_left);
            gamemode = 2;
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
        	//Starts reaction time mode
            Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
            send_LED_message(mode3Message);
            nano_wait(1000000000);
        	roundMessage[34] = game2round + 49;
            //display_handle(time_left);
        	wait_mode = 2;
            //TIM3->CR1 |= TIM_CR1_CEN;
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            gamemode = 3;
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
            Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
            send_LED_message(mode4Message);
            nano_wait(1000000000);
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            countdown();
            //display_handle(time_left);
            wait_time = 500;
            gamemode = 4;
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
        else if (gamemode == 0) { // menu
            //Starts multi mode
        	targets_hit = 0;
            Build_LED_Targets(ledstrip, halfred, halfred, halfred, halfred, halfred);
            send_LED_message(mode4Message);
            nano_wait(1000000000);
            Build_LED_Targets(ledstrip, off, off, off, off, off);
            countdown();
            //display_handle(time_left);
            gamemode = 5;
            pick_multi_target(2, halfgreen);
            TIM2->CR1 |= TIM_CR1_CEN;

        }
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
    GPIOB->MODER |= 0x2000; // Enables PB6 for USART1_TX
    GPIOB->AFR[1] &= ~0xF << 24; //AF0 for PB6
	GPIOA->MODER &= ~0x000fffff;
	GPIOA->MODER |= 0x00020000; // PA8 alternate function
    GPIOA->MODER |= 0x20; // Enables PA2
    GPIOA->AFR[0] |= 1<<8; //Alternate Function 1 for PA2, subject to change
	GPIOA->AFR[1] &= ~0x0000FFFF;
	GPIOA->AFR[1] |= 0x00002222;
	GPIOB->PUPDR |= 0xaaaa;
	GPIOC->MODER &= ~0xffffffff; // PC0-4 inputs
}

int main() {
	init_tim1();
	init_dma();
    init_tim2();
    init_tim3();
    init_pins();
    init_usart();
    GPIOB->ODR = 0;
    menu_setup();
    nano_wait(100000);
    init_exti();
    for(;;) {
        asm volatile ("wfi");
    }
    return 0;
}
