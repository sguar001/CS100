/*	Name & E-mail: Steve Guardado sguar001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Custom Lab
 *	Description: Constructor for WordUp objects.
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <stdlib.h>
#include "WordUp.h"

//Constructor to initialize a new WordUp object
void newWord(WordUp* words, char* enteredWord, int score)
{
	words->wordScore = score;
	words->word = enteredWord;
	words->numOfAppearances = 1;
}
