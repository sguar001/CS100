/*	Name & E-mail: Steve Guardado sguar001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Custom Lab
 *	Description: Prompt user for input in main function, then analyze received text to determine emotion. Finally, display the calculted emotion using the Tick_Display() function.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boom/Hash.c>
#include <boom/WordUp.c>
#include <boom/correct_usart.h>
#include <boom/timer.h>
#include <avr/eeprom.h>

#include "io.c"
#include "ledmatrix/ledmatrix7219d88.c"
#include "ledmatrix/max7219.c"

uint8_t EEMEM angryText[500] = "0 irritated 0 hated 0 enraged 0 hostile 0 insult 0 insulting 0 annoy 0 annoying 0 annoyed 0 upset 0 hate 0 hateful 0 unpleasant 0 offensive 0 aggressive 0 fuming 0 resent 0 agitated 0 angry 0 bitter 0 cruel 0 disgust 0 disgusted 0 furious 0 fury 0 grumpy 0 grouchy 0 jealous 0 mad 0 quarrel 0 stress 0 stressed 0 torment";
uint8_t EEMEM happyText[500] = "4 good 7 passed 6 thanks 6 thank 6 fortunately 10 beautiful 10 great 6 joy 6 luck 6 lucky 6 luckily 6 joy 6 joyous 6 delight 6 delighted 6 glee 6 gleeful 6 thankful 6 satisfied 6 cheerful 6 merry 6 sunny 6 love 6 admire 6 desire 6 eager 6 enjoy 6 excited 6 fascinated 6 generous 6 happy 6 happiness 6 jolly 6 love 8 thrilled 6 thrill 10 awesome 10 fantastic 6 wonderful 6 amused 6 amusement 10 amazed 10 amazing";
uint8_t EEMEM sadText[750] = "1 sad 1 grief 1 pain 1 bombed 1 sorrow 1 unhappy 1 anguish 1 desolate 1 lonely 1 mourn 1 mournful 1 pessimistic 1 dismay 1 tears 1 cry 1 crying 1 tormented 1 offended 1 wronged 1 lousy 1 ashamed 1 powerless 1 guilty 1 miserable 1 bad 1 despair 1 terrible 1 embarrassed 1 lost 1 unsure 1 empty 1 agony 1 anxiety 1 apprehensive 1 defeated 1 depressed 1 depression 1 dislike 1 disturbed 1 dread 1 discontent 1 gloomy 1 grim 1 hurt 1 humiliated 1 humiliate 1 homesick 1 mortified 1 uneasy 1 shame 1 suffer 1 tense 1 uncomfortable 1 worry 1 worried";
uint8_t EEMEM surprisedText[500] = "20 wow 50 holy 50 surprise 50 surprised 50 shocked 50 shock 50 off-guard 50 ecstatic 50 unbelievable 50 terrified 50 panic 50 alarmed 50 terrified 50 astonishment 50 awe 50 bewildered 50 flabbergasted 50 exuberant 50 euphoria 50 vivacious";

static double emotion;
enum SM1_States {SM1_Display} SM1_State;
	
static uint8_t happyFace[8] =
{
	//happy face
	0b11111111,
	0b10000001,
	0b10100101,
	0b10000001,
	0b11000011,
	0b10111101,
	0b10000001,
	0b11111111
};
			
static uint8_t sadFace[8] =
{
	//sad face
	0b11111111,
	0b10000001,
	0b10100101,
	0b10000001,
	0b10000001,
	0b10111101,
	0b11000011,
	0b11111111
};
			
static uint8_t madFace[8] =
{
	//mad face
	0b11111111,
	0b10000001,
	0b10100101,
	0b10000001,
	0b10111101,
	0b10111101,
	0b10000001,
	0b11111111
};
			
static uint8_t neutralFace[8] =
{
	//neutral face
	0b11111111,
	0b10000001,
	0b10100101,
	0b10000001,
	0b10000001,
	0b10111101,
	0b10000001,
	0b11111111
};
			
static uint8_t supriseFace[8] =
{
	//surprise face
	0b11111111,
	0b10000001,
	0b10100101,
	0b10000001,
	0b10111101,
	0b10100101,
	0b10111101,
	0b11111111
};


//Function to display correct mood depending on calculated emotion
void Tick_Display()
{
	//use ledmatrix 0 (single)
	uint8_t ledmatrix = 0;

	//set intensity of LEDs (0 - 16)
	ledmatrix7219d88_setintensity(0, 3);
	
	if(emotion >= 6.0)
	{
		ledmatrix7219d88_resetmatrix(ledmatrix);				
		USART_SendString("Surprised\n");
		ledmatrix7219d88_setmatrix(ledmatrix, supriseFace);
		delay_ms(1000);				
	}
	else if(emotion >= 3.5)
	{
		ledmatrix7219d88_resetmatrix(ledmatrix);				
		USART_SendString("Happy\n");
		ledmatrix7219d88_setmatrix(ledmatrix, happyFace);
		delay_ms(1000);
	}
	else if(emotion >= 2.5)
	{
		ledmatrix7219d88_resetmatrix(ledmatrix);		
		USART_SendString("Neutral\n");
		ledmatrix7219d88_setmatrix(ledmatrix, neutralFace);
		delay_ms(1000);
	}
	else if(emotion >= 2.0)
	{
		ledmatrix7219d88_resetmatrix(ledmatrix);				
		USART_SendString("Sad\n");
		delay_ms(50);
		ledmatrix7219d88_setmatrix(ledmatrix, sadFace);
		delay_ms(1000);
	}			
	else if(emotion >= 0.0)
	{
		ledmatrix7219d88_resetmatrix(ledmatrix);				
		USART_SendString("Mad\n");
		ledmatrix7219d88_setmatrix(ledmatrix, madFace);
		delay_ms(1000);
	}
	delay_ms(500);
	USART_Send('\n');
	USART_Send('\n');
	USART_Send('\n');
	
}

int main()
{
	DDRB = 0xFF; PORTB = 0x00;
	const unsigned char TEXTNUM = 4; //Number of text arrays
	unsigned char k = 0;

	unsigned char* textList[750];  //array of char pointers (strings) to hold each each type of keyword list
	int score; //var to hold the score of each keyword
	char getSpace; //temp var to read in the empty space char 
	char message[] = "temp"; // temp var to hold the message that is received 

	//Populate 2-D array	
	uint8_t SRAMangryText[500]; // temp uint8_t array to hold  angryText
	eeprom_read_block((void*)SRAMangryText, (const void*)angryText, 500); //read angryText[] stored in EEPROM, store in SRAM
	textList[k] = (unsigned char*) &SRAMangryText;
	k++;
	
	uint8_t SRAMhappyText[500]; // temp uint8_t array to hold  happyText
	eeprom_read_block((void*)SRAMhappyText, (const void*)happyText, 500); //read happyText[] stored in EEPROM, store in SRAM
	textList[k] =  (unsigned char*) &SRAMhappyText;
	k++;
	
	uint8_t SRAMsadText[750]; // temp uint8_t array to hold  sadText
	eeprom_read_block((void*)SRAMsadText, (const void*)sadText, 750); //read sadText[] stored in EEPROM, store in SRAM
	textList[k] =  (unsigned char*) &SRAMsadText;
	k++;
	
	uint8_t SRAMsurprisedText[500]; // temp uint8_t array to hold  sadText
	eeprom_read_block((void*)SRAMsurprisedText, (const void*)surprisedText, 500); //read surprisedText[] stored in EEPROM, store in SRAM
	textList[k] =  (unsigned char*) &SRAMsurprisedText;


	//Initialize and Flush USART
	initUSART();	
	USART_Flush();
	
	//initalize LED Matrix
	ledmatrix7219d88_init();

	//Create Hash table6 of size 1000 to be used as lookup table
	Hash table;
	int size = SIZE_OF_TABLE;
	newHash(&table, size); 

	for(unsigned char l = 0; l < TEXTNUM; l++)
	{
		//Parse the string for individual words
		char* subString = strtok(textList[l], " \n");

		while(subString != NULL)
		{
			score = atoi(subString); //convert read score into an int.

			subString = strtok(NULL, " \n");
			put(subString, score, &table); //add to the table each individual word and it's corresponding score
			subString = strtok(NULL, " \n");
		}
	}

	while(strlen(message) > 0)
	{
		unsigned char greeting[] = "Hey there, what's up? How's your day going?\n";

		//Prompt user for a reply to greeting
		while(!(USART_IsSendReady()));
			USART_SendString(greeting);

		//var to store received reply
		unsigned char input[150];
		unsigned char i = 0;

		while(1)
		{
			input[i] = USART_Receive(); //continue to receive individual characters of string.
			if(input[i] == '\n') //if reach the end of a reply, then break out
			{
				break;
			}
			i++; //increment input[i] position otherwise. 
		}
		input[i] = 0; //add null terminating character to the end of the received string.

		strcpy(message, input);

		//used for calculating the "mood" of the reply
		double sum = 0;
		int count = 0;
		emotion = 0;

		unsigned char* sub = strtok(message, " \n,!");

		//Identify all individual words from received reply
		while(sub != NULL)
		{
			double tempResult = getAverage(sub, &table); //Get the average of each word from reply
			sum += tempResult; 

			count++;

			sub = strtok(NULL, " \n,!");
		}

		if(strlen(message) > 0)
		{
			emotion = sum/count; //Calculate the emotion of the reply
		}
		
		//Period for task
		unsigned long int SMTick1_period = 1000;

		//Set timer and turn on
		TimerSet(SMTick1_period);
		TimerOn();

		SM1_State = SM1_Display;
			
		//call Tick function to display emotion on LED Matrix
		Tick_Display();
		while(!TimerFlag);
		TimerFlag = 0;	
	}
}