#include "book_management.h"
#include <stdlib.h>
#include <string.h>

int isLibrarian() {
    return 0;
}

int main() {

    printf("\n******************************\n");
    printf("Welcome to the library system\n");
    printf("******************************\n");
    printf("Type 'q' to quit\n");
    
    int loggedIn = 0; //Boolean

    char username[20] = "empty";

    char userCommand[50];

    //Interface loop

    while(strcmp(userCommand, "q") != 0) {

        //User not logged in

        if(loggedIn == 0) {
            printf("\nType \"login\" or \"register\": ");
            scanf("%s", &userCommand);

            if(strcmp(userCommand, "login")==0) {
                //loginProcedure();
            }

            else if(strcmp(userCommand, "register")==0) {
                //registerProcedure();
            }

            else if(strcmp(userCommand, "q")==0) {
                printf("\nThank you for using the system. Goodbye!\n");
            }

            else {
                printf("\nSorry, command \"%s\" not recognised!\n", &userCommand);
            }
        }

        //User logged in

        if(loggedIn == 1) {
            printf("\n\n******************************\n");
            printf("You are logged in, %s!\n", &username);
            printf("******************************\n\n");
            printf("Here are the options available to you:\n");
            printf("-> To LOG OUT, type \"out\"\n");
            printf("-> To SEARCH for books, type \"search\"\n");
            printf("-> To BORROW books, type \"borrow\"\n");
            printf("-> To RETURN books, type \"return\"\n");

            if(isLibrarian(username)) {
                printf("-> To ADD books to the library, type \"add\"\n");
                printf("-> To REMOVE books from the library, type \"remove\"\n");
            }

            scanf("%s", &userCommand);

            if(strcmp(userCommand, "out")==0) {
                //logoutProcedure();
            }

            else if(strcmp(userCommand, "search")==0) {
                //searchProcedure();
            }

            else if(strcmp(userCommand, "borrow")==0) {
                //borrowProcedure();
            }

            else if(strcmp(userCommand, "return")==0) {
                //returnProcedure();
            }

            else if(strcmp(userCommand, "add")==0 && isLibrarian(username)) {
                //addProcedure();
            }

            else if(strcmp(userCommand, "remove")==0 && isLibrarian(username)) {
                //removeProcedure();
            }

            else if(strcmp(userCommand, "q")==0) {
                printf("\nThank you for using the system. Goodbye!\n");
            }

            else {
                printf("\nSorry, command \"%s\" not recognised!\n", &userCommand);
            }

        }

    }

    return 0;
}