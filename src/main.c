/**
 * @file main.c
 * @author Neel Gondalia (ngondali@mail.uoguelph.ca)
 * @date June 2017
 * @brief main file that reads the file and stores it into a hash table and calls on functions according to user input
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main (void)
{
    char *choices [3] = {"1. Open Existing File", "2. Open New File", "3. Exit Program"};
    char *choices1 [3] = {"1. Set and Change Program Password", "2. Manage Passwords", "3. Exit Program"};
    char *choices2 [5] = {"1. Change password for an existing system", "2. Enter new system and password", "3. Retrieve password", "4. Remove a system", "5. Exit Program"};
    char *keys[51], admin[1][50], p[50] = "", *pass = NULL;
    int i = 0, j = 0;

    FILE *file = fopen ("docs/data.txt", "a+");

    //checking if file is empty
    fseek (file, 0, SEEK_END);
    if (ftell (file) == 0)
    {
        fseek (file, 0, SEEK_SET);
        fprintf (file, "Master Password{}0000\n");
    }

    //getting master password
    fseek (file, 0, SEEK_SET);
    while (fgets (admin[0], 50, file))
    {
        if (admin[0][strlen(admin[0])-1] == '\n')
        { 
            admin[0][strlen(admin[0])-1] = '\0';
        }
        break;
    }
    fclose (file);

    while (admin[0][i] != '}')
    {
        i++;
    }

    i++;

    while (i < strlen(admin[0]))
    {
        p[j] = admin[0][i];
        i++;
        j++;
    }
    i=0;
    j=0;

    pass = inputRet ("***Enter master password: ");
    if (strcmp (pass, p) != 0)
    {
        printf ("***Incorrect master Password- EXITING PASSWORD VAULT***\n");
        return 0;
    }

    //until user wants to exit
    while (1)
    {
        i = 0;
        j = 0;

        //new file or existing file
        int choice = getChoice (choices, 3);

        if (choice == 1)
        {
            FILE *f = fopen ("docs/data.txt", "r");
            fclose (f);
        }
        else if (choice == 2)
        {
            FILE *f = fopen ("docs/data.txt", "w+");
            fseek (f, 0, SEEK_SET);
            fprintf (f, "Master Password{}0000\n");
            fclose (f);
        }
        else if (choice == 3)
        {
            return 0;
        }

        int flag = 0, count = 0;
        systemInfo *retrieve = NULL, *update = NULL, *get = NULL;
        hash *ht = createTable (701);
        char input [51][50];
        FILE *fptr = fopen ("docs/data.txt", "r");
        fseek (fptr, 0, SEEK_SET);

        while (fgets (input[i], 50, fptr))
        {            
            if (input[i][strlen(input[i])-1] == '\n')
            { 
                input[i][strlen(input[i])-1] = '\0';
            }
            
            if (input[i][0] == '\n')
            {
                flag = 1;
            }
            else
            {
                flag = 0;
            }
            
            if (flag == 0)
            {
                //loading data from file to hash table
                get = intoStruct (input[i]);
                keys[count] = get->sys;
                insertData (ht, (unsigned char *)get->sys, (void *) get);
                count++;
            }
            i++;
        }
        fclose (fptr);

        char *temp = NULL, *password = NULL;
        choice = getChoice (choices1, 3);

        //set master password or manage passwords
        switch (choice)
        {
            case 1:
                retrieve = (systemInfo *) lookupData (ht, (unsigned char *) keys[0]);

                temp = retrieve->sys;
                removeData (ht, (unsigned char *) keys[0]);
                
                password = inputRet ("Enter new master password: ");

                while (strlen (password) < 1)
                {
                    print ("Password field can't be empty.");
                    password = inputRet ("Enter new master password: ");
                }

                update = malloc (sizeof (struct systemInfo));

                update->sys = malloc (sizeof (char) * ((strlen(temp)) + 1));
                strcpy (update->sys, temp);
                
                update->password =  malloc (sizeof (char) * ((strlen(password)) + 1));
                strcpy (update->password, password);

                insertData (ht, (unsigned char *) keys[0], (void *) update);

                interface2 (ht, 10, count, keys, "docs/data.txt");
                free (ht);
                break;

            case 2:
                choice = getChoice (choices2, 5);
                if (choice == 5)
                {
                    return 0;
                }
                interface2 (ht, choice, count, keys, "docs/data.txt");
                free (ht);
                break;

            case 3:
                return 0;
                break;
                
            default:
                break;
        }
        
    }
    return 0;
}
