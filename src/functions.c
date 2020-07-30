/**
 * @file functions.c
 * @author Neel Gondalia (ngondali@mail.uoguelph.ca)
 * @date June 2017
 * @brief File containing all the functions for password vault
 */

#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "functions.h"

void interface2 (hash *ht, int choice, int i, char *keys[], char* fileName)
{
    int j = 0, k = 0, choice2 = 0;
    char *password = NULL, *name = NULL, *temp = NULL, temp2[100] = "Retrieved Password is: ", temp3[100] = "Deleted System: ";
    systemInfo *retrieve = NULL;
    systemInfo *update = NULL;
    
    switch (choice)
    {
        case 1:
            if (i==0)
            {
                print ("***No Systems Found***");
                break;
            }
            else
            {
                choice2 = getChoice (keys, i);
            }

            retrieve = (systemInfo *) lookupData (ht, (unsigned char *) keys[choice2-1]);

            temp = retrieve->sys;
            removeData (ht, (unsigned char *) keys[choice2-1]);
            
            password = inputRet ("Enter new password: ");

            while (strlen(password) < 1)
            {
                print ("Password field can't be empty.");
                password = inputRet ("Enter new password: ");
            }

            update = malloc (sizeof (struct systemInfo));

            update->sys = malloc (sizeof (char) * ((strlen(temp)) + 1));
            strcpy (update->sys, temp);
            
            update->password =  malloc (sizeof (char) * ((strlen(password)) + 1));
            strcpy (update->password, password);

            insertData (ht, (unsigned char *) keys[choice2-1], (void *) update);
            
            break;

        case 2:
            name = inputRet ("Enter system name: ");

            while (strlen(name) < 1)
            {
                print ("System name field can't be empty.");
                name = inputRet ("Enter system name: ");
            }

            while (strcmp (name, "Master Password") == 0)
            {
                print ("System name can't be 'Master Password'.");
                name = inputRet ("Enter system name: ");
            }
            
            password = inputRet ("Enter system password: ");

            while (strlen(password) < 1)
            {
                print ("Password field can't be empty.");
                password = inputRet ("Enter system password: ");
            }

            keys[i] = name;

            update = malloc (sizeof (struct systemInfo));

            update->sys = malloc (sizeof (char) * ((strlen(name)) + 1));
            strcpy (update->sys, name);

            
            update->password = malloc (sizeof (char) * ((strlen(password)) + 1));
            strcpy (update->password, password);
            
            insertData (ht, (unsigned char *) keys[i], (void *) update);
            i++;
            
            break;

        case 3:
            if (i==0)
            {
                print ("***No Systems Found***");
                break;
            }
            else
            {
                choice2 = getChoice (keys, i);
            }

            retrieve = (systemInfo *) lookupData (ht, (unsigned char *) keys[choice2-1]);
            temp = retrieve->password;
            strcat (temp2, temp);
            print (temp2);
            break;

        case 4:
            if (i==0)
            {
                print ("***No Systems Found***");
                break;
            }
            else
            {
                choice2 = getChoice (keys, i);

                while (choice2 == 1)
                {
                    print ("***'Master Password' can not be deleted, only changed.***");
                    choice2 = getChoice (keys, i);
                }
            }

            removeData (ht, (unsigned char *) keys[choice2-1]);

            temp = keys[choice2-1];
            strcat (temp3, temp);
            print (temp3);
            
            j= choice2-1;

            while (j<(i-1))
            {
                strcpy (keys[j], keys[j+1]);
                j++;
            }
            i--;
            break;
            

        default:
            break;
    }

    FILE * fptr = fopen(fileName, "w+");
    fseek (fptr, 0, SEEK_SET);
    
    for (k=0; k<i; k++)
    {
        retrieve = (systemInfo *) lookupData (ht, (unsigned char *) keys[k]);
        fseek (fptr, 0, SEEK_CUR);
        fprintf (fptr, "%s{}%s\n", retrieve->sys, retrieve->password);
    }
    fclose (fptr);

    return;
}

int getChoice (char *choices[], int numChoices)
{
    int scroll = 0, shadow = 0, choice = 0, i = 0, x = 0;

    clear();
    initscr();
    noecho();
    cbreak();
    getmaxyx (stdscr, i, x);
    WINDOW *menu = newwin(numChoices+4, x/2 , 10, 0);
    i=0;

    mvprintw (2, 1, "*****************************");
    mvprintw (4, 1, "  WELCOME TO PASSWORD VAULT  ");
    mvprintw (6, 1, "*****************************");
    mvprintw (8, 1, "***Use arrow keys to scroll up and down. Press 'enter' to select a menu choice.");
    
    box(menu, 0, 0);
    refresh();
    keypad(menu, true);

    while (scroll != 10)
    {
        for (i=0; i<numChoices; i++)
        {
            if (i == shadow)
            {
                wattron (menu, A_STANDOUT);
            }

            mvwprintw (menu, i+2, 1, choices[i]);
            wattroff (menu, A_STANDOUT);
        }

        scroll = wgetch (menu);

        switch (scroll)
        {
            case KEY_UP:
                shadow--;
                if (shadow == -1)
                {
                    shadow = 0;
                }
                break;

            case KEY_DOWN:
                shadow++;
                if (shadow == numChoices)
                {
                    shadow = 0;
                }
                break;

            case 10:
                choice = shadow + 1;
                break;

            default:
                break;
        }

    }

    erase();
    endwin();
    return choice;
}

systemInfo *intoStruct (char *input)
{
    systemInfo *load = malloc(sizeof (systemInfo));
    int i = 0;
    int j =0;
    char s[50] = "";
    char p[50] = "";

    i=0;
    while (input[i] != '{')
    {
        s[i] = input[i];
        i++;
    }

    load->sys = malloc (sizeof (char) * ((strlen(s)) + 1));
    strcpy (load->sys, s);

    i=i+2;
    j=0;
    while (i<strlen(input))
    {
        p[j] = input[i];
        i++;
        j++;
    }

    load->password = malloc (sizeof (char) * ((strlen(p)) + 1));
    strcpy (load->password, p);
    return load;
}

char *inputRet(char *input)
{
    char *ret = malloc (25);
    char *in = NULL;
    int x, y = 0;

    clear();
    initscr();
    noecho();
    cbreak();
    getmaxyx (stdscr, y, x);

    WINDOW *screen = newwin (y, x, 0, 0);
    wrefresh(screen);
    refresh();

    mvprintw (2, 1, "******************************");
    mvprintw (4, 1, " PASSWORD VAULT: INPUT SCREEN ");
    mvprintw (6, 1, "******************************");

    mvprintw (8, 0, "%s\n", input);
    refresh();
    
    scanw ("%s", ret);
    refresh();

    if (ret[strlen(ret)] == '\n')
    {
        ret[strlen(ret)] = '\0';
    }

    in = ret;
    
    erase();
    endwin();

    return in;
}

void print (char *input)
{
    int x, y = 0;
    
    clear();
    initscr();
    noecho();
    cbreak();
    getmaxyx (stdscr, y, x);

    WINDOW *screen = newwin (y, x, 0, 0);
    wrefresh(screen);
    refresh();

    mvprintw (2, 1, "*******************************");
    mvprintw (4, 1, " PASSWORD VAULT: OUTPUT SCREEN ");
    mvprintw (6, 1, "*******************************");
    
    mvprintw (8, 0, "%s\n", input);
    refresh();

    mvprintw (10, 0, "Press any key to go back.");
    refresh();

    getch();
    erase();
    endwin();

    return;
}

//This hash function is based off of djb2 by dan bernstein
unsigned long hFunction(int size, unsigned char *key)
{
    unsigned long h = 8311;
    int i = 0;

    while ((i = *key++))
    {
        h = h * 33 + i;
    }
    return (h+2) % size;
}

hash *createTable(int size)
{
    hash *hTable = malloc(sizeof(hash));

    hTable->size = size;
    hTable->table = malloc(sizeof(Node) * size);
    
    return hTable;
}

Node *createNode(unsigned char *key, void *data)
{
    Node *new = malloc(sizeof(Node));

    new->key = key;
    new->data = data;
    new->next = NULL;

    return new;
}

void insertData(hash *hashTable, unsigned char *key, void *data)
{
    Node *new = NULL;
    Node *temp = NULL;

    if (hashTable == NULL)
    {
        return;
    }

    else
    {
        int index = hFunction (hashTable->size, key);

        if (hashTable->table[index] == NULL)
        {
            new = createNode(key, data);
            hashTable->table[index] = new;
        }
        else
        {
            temp = hashTable->table[index];

            while (temp->next != NULL && strcmp((char *) temp->key, (char *) key) != 0)
            {
                temp = temp->next;
            }

            if (strcmp((char *) temp->key, (char *) key) == 0)
            {
                temp->data = data;
            }
            else
            {
                new = createNode(key, data);
                temp->next = new;
            }
        }
    }
    return;
}

void removeData(hash *hashTable, unsigned char *key)
{
    Node *new = NULL;

    if (hashTable == NULL)
    {
        return;
    }

    else
    {
        int index = hFunction(hashTable->size, key);

        if (hashTable->table[index] == NULL)
        {
            return;
        }

        
        new = hashTable->table[index];
        Node *prev = NULL;
        
        if (new == NULL)
        {
            return;
        }

        if (strcmp((char *) new->key, (char *) key) == 0)
        {
            prev = new;
            new = new->next;
            hashTable->table[index] = new;
            free (prev->data);
            free (prev);
            return;
        }

        while (strcmp((char *) new->key, (char *) key) != 0 && new != NULL)
        {
            prev=new;
            new = new->next;
        }
        
        if (new == NULL)
        {
            return;
        }
        else
        {
            prev->next = new->next;
            free (new->data);
            free (new);
            return;
        }
    }
}

void *lookupData(hash *hashTable, unsigned char *key)
{
    Node *new = NULL;

    if (hashTable == NULL)
    {
        return NULL;
    }

    else
    {
        int index = hFunction (hashTable->size, key);


        if (hashTable->table[index] == NULL)
        {
            return NULL;
        }

        new = hashTable->table[index];
        while (new != NULL)
        {
            if (strcmp((char *) new->key, (char *) key) == 0)
            {
                return new->data;
            }

            new = new->next;
        }
    }
    return NULL;
}
