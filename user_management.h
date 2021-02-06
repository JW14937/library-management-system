#include <stdio.h>
#define max_users 200

struct User {
        int id;
        int is_librarian;
		char name[30];
		char email[30];
        char username[30];
        char password[30];
};

extern struct User users[max_users];
extern int nr_of_users;

extern int current_user_id;

int load_users();
int login_procedure();
int find_username(char username[30]);
int compare_password(int user_id, char password[30]);