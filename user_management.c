#include "user_management.h"
#include <stdlib.h>
#include <string.h>

struct User users[max_users];
int nr_of_users = 0;

// Loads account data of all users saved in file
int load_users () {
    FILE *fp;
    fp = fopen("account_data.txt", "r");
    if(fp == NULL) return 1;

    char line[256];

    while(fscanf(fp, "%[^\n]\n", line) != -1) {
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

    fclose(fp);
    return 0;
}

int login_procedure() {
    char username[30];
    printf("Enter username: ");
    scanf("%s", &username);

    int user_id = find_username(username);

    if(user_id < 0) {
        printf("\n>> Error << Username \"%s\" not found! Double-check your spelling, or register with a new username.\n", &username);
        return 1;
    }

    char password[30];
    printf("Enter password: ");
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
