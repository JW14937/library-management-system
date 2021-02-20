#include <stdio.h>
#define max_users 200

struct User {
        int id;
        int is_librarian;
		char *name;
		char *email;
        char *username;
        char *password;
};

extern struct User users[max_users];
extern int nr_of_users; //The actual number of registered users, maximum 200

extern int current_user_id; //UserID of currently logged in user

int load_users(FILE *file); //Loads account data of all users saved in text file
int store_users(FILE *file);

int login_procedure();

int find_username(char username[30]); //Returns UserID that matches provided username, -1 if none found
int compare_password(int user_id, char password[30]); //Returns 0 if passwords match, 1 otherwise

int register_procedure();

int email_exists(char text[50]); //Returns 1 if user with provided email already exists, 0 otherwise
int username_exists(char text[50]); //Returns 1 if user with provided username already exists, 0 otherwise
int only_letters_spaces(char text[50]); //Returns 1 if text contains only alphanumeric characters and spaces, 0 otherwise
int no_spaces(char text[50]); //Returns 1 if text contains no spaces, 0 if it does
int at_counter(char text[50]); //Returns the number of '@' in text