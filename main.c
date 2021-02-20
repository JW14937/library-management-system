#include "book_management.h"
#include "user_management.h"
#include "book_additional.h"
#include <stdlib.h>
#include <string.h>

int current_user_id;

static void print_welcome_message() {
    printf("\n**************************************\n");
    printf("  --  Welcome to the library system  --  \n");
    printf("**************************************\n");
}

int main() {

    FILE *fp;

    if(load_users(fp) != 0) {

        // File not created yet - add admin account

        users[0].id = 0;
        users[0].is_librarian = 1;
        users[0].name = (char*)malloc(strlen("Janine Hopkins") * sizeof(char));
        strcpy(users[0].name, "Janine Hopkins");
        users[0].email = (char*)malloc(strlen("janine_hopkins@library.co.uk") * sizeof(char));
        strcpy(users[0].email, "janine_hopkins@library.co.uk");
        users[0].username = (char*)malloc(strlen("librarian1") * sizeof(char));
        strcpy(users[0].username, "librarian1");
        users[0].password = (char*)malloc(strlen("Password123") * sizeof(char));
        strcpy(users[0].password, "Password123");

        nr_of_users = 1;
    }

    if(load_books(fp) != 0) {

        // File not created yet - add starter books

        // Crime and Punishment
        books[0].title = (char*)malloc(strlen("Crime and Punishment") * sizeof(char));
        strcpy(books[0].title, "Crime and Punishment");
        books[0].authors = (char*)malloc(strlen("Fyodor Dostoevsky") * sizeof(char));
        strcpy(books[0].authors, "Fyodor Dostoevsky");
        books[0].year = 1866;
        books[0].copies = 15;

        // Dune
        books[1].title = (char*)malloc(strlen("Dune") * sizeof(char));
        strcpy(books[1].title, "Dune");
        books[1].authors = (char*)malloc(strlen("Frank Herbert") * sizeof(char));
        strcpy(books[1].authors, "Frank Herbert");
        books[1].year = 1965;
        books[1].copies = 20;

        nr_of_books = 2;
    }
    
    load_loans(fp);

    print_welcome_message();

    int logged_in = 0;
    char user_command[20];

    /* --- The interface --- */

    while(1) {

        /* --- Section for logged out users --- */

        if(logged_in == 0) {
            printf("\n--- You are logged out ---");
            printf("\nType \"q\" to quit the system");
            printf("\nType \"login\" or \"register\": ");
            
            fgets(user_command, 20, stdin);
            user_command[strcspn(user_command, "\n")] = 0;

            if(strcmp(user_command, "login")==0) {

                if(login_procedure() == 0) {
                    logged_in = 1;
                    printf("\n\n******************************\n");
                    printf("  Hello, %s!\n", users[current_user_id].name);
                    printf("******************************\n");
                }
            }

            else if(strcmp(user_command, "register")==0) {

                if(register_procedure() == 0) {
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
            printf("-> To see BORROWED books and RETURN them, type \"borrowed\"\n");

            if(users[current_user_id].is_librarian) {
                printf("-> To ADD books to the library, type \"add\"\n");
                printf("-> To REMOVE books from the library, type \"remove\"\n");
            }

            printf("-> To SAVE and QUIT, type \"q\"\n");

            fgets(user_command, 20, stdin);
            user_command[strcspn(user_command, "\n")] = 0;

            if(strcmp(user_command, "search")==0) {
                search_procedure();
            }

            else if(strcmp(user_command, "borrowed")==0) {
                return_procedure();
            }

            else if(strcmp(user_command, "add")==0 && users[current_user_id].is_librarian) {
                add_procedure();
            }

            else if(strcmp(user_command, "remove")==0 && users[current_user_id].is_librarian) {
                remove_procedure();
            }

            else if(strcmp(user_command, "q")==0) {
                store_books(fp);
                store_loans(fp);
                store_users(fp);

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