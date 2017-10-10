/*	Name & E-mail: Steve Guardado sguar001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Custom Lab
 *	Description: Functions to build hash table and put words into the hash table. Also, here I calculate the emotion of the inputted string. 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <stdlib.h>
#include <math.h>
#include "Hash.h"
#include "correct_usart.h"

//variable that declares the size of the table
const int SIZE_OF_TABLE = 1000;


//Sets HASH table size to SIZE_OF_TABLE
void newHash(Hash *hashTable, int size)
{
	hashTable->tableSize = SIZE_OF_TABLE;
}

//Compute hash of each word based on first 2 chars to find a spot in the table.
int computeHash( char* s, Hash* table)
{
	int hash = s[0]*pow(3, 2) + s[1]*pow(5,2);
	if(hash < 0)
	{
		USART_SendString("Overflowed\n");
		hash = abs(hash); 
	}

	int result =  hash % SIZE_OF_TABLE; 

	//If calculated spot is empty, return result
	if(table->list[result] == 0)
	{
		return result;
	}
	else
	{	
		//Else, continue to iterate through table to find an open spot, then return that spot 
		while((table->list[result] != NULL))
		{
			if(strcmp(s, table->list[result]->word) == 0)
			{	
				return result;
			}
			
			result++;
		}	
		return result % SIZE_OF_TABLE;
	}
}

//Function to enter textWord and its corresponding score into the HASH table
void put(char* textWord, int score, Hash *table)
{
	//if reach newline character, return
	if(strcmp(textWord,  "\n") == 0)
	{
		return; 
	}
	
	//compute hashvalue for parameter textWord
	int hashValue = computeHash(textWord, table);
	//check to see if word is in table already, if not, create a new WordUp object and store in table. 

	if(!(contains(textWord, table, hashValue))) //function to search the constructed hashtable for the inputted string. 
	{
		WordUp *wordNew = (WordUp*)malloc(sizeof(WordUp));
		newWord(wordNew, textWord, score);

		table->list[hashValue] = wordNew;
		return;
	}
	else //Else, update the number of times the word has appeared and update it's score also.
	{
		table->list[hashValue]->numOfAppearances++;
		table->list[hashValue]->wordScore += score;
	}
}

//Function to see if the parameter string is in the HASH table
bool contains( char* string, Hash *table, int hashValue)
{
	unsigned int i;
	int size = SIZE_OF_TABLE;	
	
	//first, check the spot at hashValue (the computed hash for the string)
	if(table->list[hashValue] == NULL)
	{
		return false;
	}

	// if the spot is not empty, check to see if the word at that spot is the string, else, check nearby locations until found
	for(i = hashValue; i < size; i++)
	{
		if(!(table->list[i] == NULL))
		{	
			if(strcmp(table->list[i]->word, string) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

//function to get the average of the parameter string, used to calculate the total "emotion"
double getAverage( char* string, Hash *table)
{
	int hashValue = computeHash(string, table);

	//if the string is in the table, return it's average (score/numOfApppearances)
	if(contains(string, table, hashValue))
	{
		int totalScore = table->list[hashValue]->wordScore;
		int totalNum = table->list[hashValue]->numOfAppearances;
		return (double)totalScore/totalNum;// Return averagge of WordUp object
	}
	
	//If the word is not contained in the Hash table, return a average value of 2.5
	return 2.5;
}

//function to get the word at the current index value in the table
char* getString(Hash* table, int index)
{
	return table->list[index]->word;
}