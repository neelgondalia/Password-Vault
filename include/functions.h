/**
 * @file functions.h
 * @author Neel Gondalia (ngondali@mail.uoguelph.ca)
 * @date June 2017
 * @brief File containing all the function definitions
 */

#ifndef _FUNCTIONS_
#define _FUNCTIONS_

#include <stdio.h>
#include <stdlib.h>

/**
*structure for data to be entered into hash table 
**/
typedef struct systemInfo
{
    char *sys; ///< string that contains the system descriptor
    char *password; ///< string that contains the password for a system descriptor 
}systemInfo;

/**
*Node of the hash table. 
**/
typedef struct Node
{
    unsigned char *key; ///< string that represents a piece of data in the table
    void *data; ///< void pointer to generic data that is to be stored in the hash table
    struct Node *next; ///< pointer to the next Node in case of a collision
} Node;

/**
*Hash table structure
**/
typedef struct hash
{
    int size; ///< integer that represents the size of the hash table
    Node **table; ///< array that contains all of the table nodes
}hash;

/**Function that uses ncurses to create a menu
*@return integer that is the choice of the user from the menu displayed
*@param choices array of char pointers that are used as menu options
*@param numChoices integer that is the number of menu options to be displayed
**/
int getChoice (char *choices[], int numChoices);

/**Function that loads data to a struct
*@return pointer to struct where the data is stored
*@param input char pointer to the string that is to be loaded to struct
**/
systemInfo *intoStruct (char *input);

/**Function that uses ncurses to get input from user
*@return char pointer to user input
*@param input char pointer to the string that is printed to ask user for input
**/
char *inputRet(char *input);

/**Function that uses ncurses to print a statement
*@param input char pointer to the string that is to be printed
**/
void print (char *input);

/**Makes changes to hash table and uploads all the data to file
*@pre hash table type must exist and have memory allocated to it 
*@param choice integer that is the choice of what function the user wants to perform
*@param i integer that represents the amount of data structs stored in the hash table
*@param keys array of char pointer that points to all the system descriptors
*@param file char pointer to the name of the file that is to be updated
**/
void interface2 (hash *ht, int choice, int i, char *keys[], char* file);

/**Function that returns an index for the hash table to store at
*@return unsigned long that is the index the data will be stored at in the hash table
*@param size size of the hash table
*@param key char pointer that points to the string to be hashed
**/
unsigned long hFunction(int size, unsigned char *key);

/**Function to create a hash table. Allocates memory to the struct and table based on the size given.
*@return pointer to the hash table
*@param size size of the hash table
**/
hash *createTable(int size);

/**Function for creating a node for the hash table.
*@pre Node must be cast to void pointer before being added
*@post Node is valid and able to be added to the hash table
*@param key unsigned char pointer that points to a string that represents the data 
*@param data is a generic pointer to any data type.
*@return returns a node for the hash table
**/
Node *createNode(unsigned char *key, void *data);

/**Inserts a Node in the hash table.
*@pre hashTable type must exist and have data allocated to it
*@param hashTable pointer to the hash table
*@param key unsigned char pointer that points to a string that represents the data 
*@param data pointer to generic data that is to be inserted into the list
**/
void insertData(hash *hashTable, unsigned char *key, void *data);

/**Function to remove a node from the hash table 
 *@pre Hash table must exist and have memory allocated to it
 *@post Node at key will be removed from the hash table if it exists.
 *@param hashTable pointer to the hash table struct
 *@param key unsigned char pointer that points to a string that represents the data 
 **/
void removeData(hash *hashTable, unsigned char *key);

/**Function to return the data located at a certain key
 *@pre The hash table exists and has memory allocated to it
 *@param hashTable pointer to the hash table containing all the data nodes
 *@param key unsigned char pointer that points to a string that represents the data 
 *@return returns a pointer to the data in the hash table. Returns NULL if the data is not found.
 **/
void *lookupData(hash *hashTable, unsigned char *key);

#endif
