/*	Name & E-mail: Steve Guardado sguar001@ucr.edu
 *	Lab Section: 23
 *	Assignment: Custom Lab
 *	Description: functions for hash.h. Includes struct used for building table. 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef HASH_H
#define HASH_H

#include "WordUp.h"
#include <stdbool.h>

typedef struct 
{
	int tableSize; //var for size of table
	WordUp* list[1000]; // HASH table to store all the read words 
} Hash;

void newHash(Hash *hashTable, int size);

bool contains( char* string, Hash *table, int hashValue);

double getAverage( char* string, Hash *table);

void put(char* textWord, int score, Hash *table);

int computeHash( char* s, Hash* table);

char* getString(Hash* table, int index);


#endif