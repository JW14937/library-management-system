#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct User users[max_users];
int nr_of_users = 0;

int load_users (FILE *file) {

    nr_of_users = 0;

    file = fopen("account_data.txt", "r");
    if(file == NULL) return 1;

    char line[256];

    while(fscanf(file, "%[^\n]\n", line) != -1) {
        static int i = 1;

        if(i == 1) users[nr_of_users].id = atoi(line);
        else if(i == 2) users[nr_of_users].is_librarian = atoi(line);
        else if(i == 3) strcpy(users[nr_of_users].name, line);
        else if(i == 4) strcpy(users[nr_of_users].email, line);
        else if(i == 5) strcpy(users[nr_of_users].username, line);
        else if(i == 6) strcpy(users[nr_of_users].password, line);
        
        i += 1;
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
    }

    fclose(file);

    return 0;
}

int login_procedure() {
    char username[30];
    printf("\nEnter username: ");
    scanf("%s", &username);

    int user_id = find_username(username);

    if(user_id < 0) {
        printf("\n>> Error << Username \"%s\" not found! Double-check your spelling, or register with a new username.\n", &username);
        return 1;
    }

    char password[30];
    printf("\nEnter password: ");
    scanf("%s", &password);

    if(compare_password(user_id, password) != 0) {
        printf("\n>> Error << Wrong password!\n");
        return 1;
    }

    current_user_id = user_id;
    return 0;
}

int find_username(char username[30]) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(users[i].username, username)==0) return users[i].id;
    }
    return -1;
}

int compare_password(int user_id, char password[30]) {
    if(strcmp(users[user_id].password, password)==0) return 0;
    else return 1;
}

int register_procedure() {

    if(nr_of_users >= max_users) {
        printf("\n>> Error << Maximum number of users (%d) has been reached", max_users);
        return(1);
    }

    /* --- Name --- */

    char name[30];
    printf("\nEnter your name (max. 30 characters): ");
    scanf(" %[^\n]%*c", name); 
    name[strcspn(name, "\n")] = '\0';

    if(strlen(name) < 3) {
        printf("\n>> Error << Name must be at least 3 characters long\n");
        return(1);
    }

    if(!only_letters_spaces(name)) {
        printf("\n>> Error << Name can only contain letters and spaces\n");
        return(1);
    }

    /* --- Email --- */
    
    char email[30];
    printf("Enter your email (max. 30 characters): ");
    scanf("%s", email);

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

    /* --- Username --- */

    char username[30];
    printf("Enter your username (max. 30 characters, no spaces): ");
    scanf("%s", username);

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
    }

    /* --- Password --- */

    char password[30];
    printf("Enter your password (max. 30 characters, no spaces): ");
    scanf("%s", password);
    
    if(strlen(password) < 3 || strlen(password) > 30) {
        printf("\n>> Error << Password must be 3-30 characters long\n");
        return(1);
    }

    if(!no_spaces(password)) {
        printf("\n>> Error << Password cannot contain spaces\n");
        return(1);
    }

    /* --- Add to array --- */

    users[nr_of_users].id = nr_of_users;
    users[nr_of_users].is_librarian = 0;
    strcpy(users[nr_of_users].name, name);
    strcpy(users[nr_of_users].email, email);
    strcpy(users[nr_of_users].username, username);
    strcpy(users[nr_of_users].password, password);

    nr_of_users++;

    return 0;
}

int email_exists(char text[50]) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(text, users[i].email) == 0) { return 1; }
    }
    return 0;
}

int username_exists (char text[50]) {
    for(int i=0; i<nr_of_users; i++) {
        if(strcmp(text, users[i].username) == 0) { return 1; }
    }
    return 0;
}

int only_letters_spaces (char text[50]) {
    for(int i=0; i<strlen(text); i++) {
        if(!(isalpha(text[i]) || text[i] == ' ')) {
            return 0;
        }
    }

    return 1;
}

int no_spaces (char text[50]) {
    for(int i=0; i<strlen(text); i++) {
        if(text[i] == ' ') {
            return 0;
        }
    }

    return 1;
}

int at_counter (char text[50]) {
    int counter = 0;
    for(int i=0; i<strlen(text); i++) {
        if(text[i] == '@') counter++;
    }

    return counter;
}