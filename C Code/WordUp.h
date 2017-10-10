/*	Name & E-mail: Steve Guardado sguar001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Custom Lab
 *	Description: Struct to hold each read word's score, the actual word, and the number of times its appeared. Also a constructor to initalize each WordUp object.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef WORDUP_H
#define WORDUP_H


#include <stdlib.h>
typedef struct
{
	int wordScore; //score for word
	char* word; //read string 
	int numOfAppearances; //num of appearances in the table 
	
} WordUp;

void newWord(WordUp* words,  char* enteredWord, int score);


#endif