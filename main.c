#include "book_management.h"
#include "user_management.h"
#include "books_additional.h"
#include <stdlib.h>
#include <string.h>

int current_user_id;

void print_welcome_message() {
    printf("\n**************************************\n");
    printf("  ~  Welcome to the library system  ~  \n");
    printf("**************************************\n");
    printf("To QUIT at any point, type \"q\"\n");
}

int main() {

    if(load_users() != 0) {
        printf("Error loading users");
        exit(1);
    }
    
    FILE *fp;
    if(load_books(fp) != 0) {
        printf("Error loading books");
        exit(1);
    }

    print_welcome_message();

    int logged_in = 0; //Boolean
    char user_command[20];

    /* --- The interface --- */

    while(1) {

        /* --- Section for logged out users --- */

        if(logged_in == 0) {
            printf("\nType \"login\" or \"register\": ");
            scanf("%s", user_command);

            if(strcmp(user_command, "login")==0) {
                if(login_procedure() == 0) {
                    logged_in = 1;
                    printf("\n\n******************************\n");
                    printf("  Hello, %s!\n", &(users[current_user_id].name));
                    printf("******************************\n");
                }
            }

            else if(strcmp(user_command, "register")==0) {
                if(register_procedure() == 0) {
                    load_users();
                    printf("\n******************************************");
                    printf("\nRegistration successful! You may now log in.");
                    printf("\n******************************************\n");
                }
            }

            else if(strcmp(user_command, "q")==0) {
                printf("\nThank you for using the system. Goodbye!\n");
                exit(0);
            }

            else {
                printf("\n>> Error << Sorry, command \"%s\" not recognised!\n", &user_command);
            }
        }


        /* --- Section for logged in users --- */

        if(logged_in == 1) {
            printf("\n-> To SEARCH and then BORROW books, type \"search\"\n");
            printf("-> To see BORROWED books and RETURN them, type \"return\"\n");

            if(users[current_user_id].is_librarian) {
                printf("-> To ADD books to the library, type \"add\"\n");
                printf("-> To REMOVE books from the library, type \"remove\"\n");
            }

            printf("-> To LOG OUT, type \"out\"\n");

            scanf("%s", user_command);

            if(strcmp(user_command, "out")==0) {
                current_user_id = -1;
                logged_in = 0;
            }

            else if(strcmp(user_command, "search")==0) {
                search_procedure();
            }

            else if(strcmp(user_command, "return")==0) {
                return_procedure();
            }

            else if(strcmp(user_command, "add")==0 && users[current_user_id].is_librarian) {
                add_procedure();
            }

            else if(strcmp(user_command, "remove")==0 && users[current_user_id].is_librarian) {
                //removeProcedure();
            }

            else if(strcmp(user_command, "q")==0) {
                printf("\nThank you for using the system. Goodbye!\n");
                exit(0);
            }

            else {
                printf("\n>> Error << Sorry, command \"%s\" not recognised!\n", &user_command);
            }

        }

    }

    return 0;
}