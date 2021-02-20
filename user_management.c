#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- 
        This file contains functions related to managing users 
        General descriptions of all functions in the header file       ---------- */

struct User users[max_users]; // Array of all users
int nr_of_users = 0; // Number of all users

/* --- Load and store --- */

int load_users (FILE *file) {

    nr_of_users = 0;

    file = fopen("account_data.txt", "r");
    if(file == NULL) return 1;

    char line[100];

    // Read line by line from account_data.txt
    while(fgets(line, 100, file) != NULL) {

        line[strcspn(line, "\n")] = 0;
        static int i = 1; // Counter to distinguish between lines containing different data

        // UserID line
        if(i == 1) users[nr_of_users].id = atoi(line);

        // Line indicating whether user is librarian (1) or not (0)
        else if(i == 2) users[nr_of_users].is_librarian = atoi(line);

        // Name line
        else if(i == 3) {
            users[nr_of_users].name = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(users[nr_of_users].name, line);
        }

        // Email line
        else if(i == 4) {
            users[nr_of_users].email = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(users[nr_of_users].email, line);
        }

        // Username line
        else if(i == 5) {
            users[nr_of_users].username = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(users[nr_of_users].username, line);
        }

        // Password line
        else if(i == 6) {
            users[nr_of_users].password = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(users[nr_of_users].password, line);
        }
        
        i += 1;

        // Reset the counter - read all data of 1 user
        if(i > 6) {
            i = 1;
            nr_of_users += 1;
        }
    }

    fclose(file);

    return 0;
}

int store_users(FILE* file) {

    file = fopen("account_data.txt", "w");
    if(file == NULL) return 1;

    for(int i=0; i<nr_of_users; i++) {
        fprintf(file, "%d\n", users[i].id);
        fprintf(file, "%d\n", users[i].is_librarian);
        fprintf(file, "%s\n", users[i].name);
        fprintf(file, "%s\n", users[i].email);
        fprintf(file, "%s\n", users[i].username);
        fprintf(file, "%s\n", users[i].password);

        free(users[i].name);
        free(users[i].email);
        free(users[i].username);
        free(users[i].password);
    }

    fclose(file);

    return 0;
}

/* --- Login and registration --- */

int login_procedure() {

    // Username
    char username[30];
    printf("\nEnter username: ");

    fgets(username, 30, stdin);
    username[strcspn(username, "\n")] = 0;

    int user_id = find_username(username);

    if(user_id < 0) {
        printf("\n>> Error << Username \"%s\" not found! Double-check your spelling, or register with a new username.\n", &username);
        return 1;
    }

    // Password
    char password[30];
    printf("Enter password: ");
    fgets(password, 30, stdin);
    password[strcspn(password, "\n")] = 0;

    // Check if password correct
    if(compare_password(user_id, password) != 0) {
        printf("\n>> Error << Wrong password!\n");
        return 1;
    }

    // Login successful
    current_user_id = user_id;
    return 0;
}

// Returns UserID for this username, -1 if none found
static int find_username(const char *username) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(users[i].username, username)==0) return users[i].id;
    }
    return -1;
}

static int compare_password(int user_id, const char *password) {
    if(strcmp(users[user_id].password, password)==0) return 0;
    else return 1;
}

// Interface to register
int register_procedure() {

    if(nr_of_users >= max_users) {
        printf("\n>> Error << Maximum number of users (%d) has been reached", max_users);
        return(1);
    }

    // Name

    char name[30];
    printf("\nEnter your name (max. 30 characters): ");

    fgets(name, 30, stdin);
    name[strcspn(name, "\n")] = 0;

    if(strlen(name) < 3) {
        printf("\n>> Error << Name must be at least 3 characters long\n");
        return(1);
    }

    if(!only_letters_spaces(name)) {
        printf("\n>> Error << Name can only contain letters and spaces\n");
        return(1);
    }

    // Email
    
    char email[30];
    printf("Enter your email (max. 30 characters): ");

    fgets(email, 30, stdin);
    email[strcspn(email, "\n")] = 0;

    if(strlen(email) < 3 || strlen(email) > 30) {
        printf("\n>> Error << Email must be 3-30 characters long\n");
        return(1);
    }

    if(!no_spaces(email)) {
        printf("\n>> Error << Email cannot contain spaces\n");
        return(1);
    }
    
    if(at_counter(email) != 1) {
        printf("\n>> Error << Email must contain one \"@\" character\n");
        return(1);
    }

    if(email_exists(email)) {
        printf("\n>> Error << User with this email address already exists\n");
        return(1);
    }

    // Username

    char username[30];
    printf("Enter your username (max. 30 characters, no spaces): ");

    fgets(username, 30, stdin);
    username[strcspn(username, "\n")] = 0;

    if(strlen(username) < 3 || strlen(username) > 30) {
        printf("\n>> Error << Username must be 3-30 characters long\n");
        return(1);
    }

    if(!no_spaces(username)) {
        printf("\n>> Error << Username cannot contain spaces\n");
        return(1);
    }

    if(username_exists(username)) {
        printf("\n>> Error << User \"%s\" already exists!\n", username);
        return(1);
    }

    // Password

    char password[30];
    printf("Enter your password (max. 30 characters, no spaces): ");
    fgets(password, 30, stdin);
    password[strcspn(password, "\n")] = 0;
    
    if(strlen(password) < 3 || strlen(password) > 30) {
        printf("\n>> Error << Password must be 3-30 characters long\n");
        return(1);
    }

    if(!no_spaces(password)) {
        printf("\n>> Error << Password cannot contain spaces\n");
        return(1);
    }

    // All checks passed - add new user to user array

    users[nr_of_users].id = nr_of_users;
    users[nr_of_users].is_librarian = 0;
    users[nr_of_users].name = (char*)malloc(strlen(name) * sizeof(char));
    strcpy(users[nr_of_users].name, name);
    users[nr_of_users].email = (char*)malloc(strlen(email) * sizeof(char));
    strcpy(users[nr_of_users].email, email);
    users[nr_of_users].username = (char*)malloc(strlen(username) * sizeof(char));
    strcpy(users[nr_of_users].username, username);
    users[nr_of_users].password = (char*)malloc(strlen(password) * sizeof(char));
    strcpy(users[nr_of_users].password, password);

    nr_of_users++;

    return 0;
}


/* --- Utilities --- */

int email_exists(const char *text) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(text, users[i].email) == 0) { return 1; }
    }
    return 0;
}

int username_exists (const char *text) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(text, users[i].username) == 0) { return 1; }
    }
    return 0;
}

int only_letters_spaces (const char *text) {
    for(int i=0; i<strlen(text); i++) {
        if(!(isalpha(text[i]) || text[i] == ' ')) {
            return 0;
        }
    }

    return 1;
}

int no_spaces (const char *text) {
    for(int i=0; i<strlen(text); i++) {
        if(text[i] == ' ') {
            return 0;
        }
    }

    return 1;
}

int at_counter (const char *text) {
    int counter = 0;
    for(int i=0; i<strlen(text); i++) {
        if(text[i] == '@') counter++;
    }

    return counter;
}