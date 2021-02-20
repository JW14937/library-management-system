#include <stdio.h>
#define max_users 200

struct User {
        int id; // Unique UserID
        int is_librarian; // 1 is user is a librarian (admin), 0 otherwise
		char *name; // Full name, can contain spaces
		char *email; 
        char *username;
        char *password;
};

extern struct User users[max_users]; // Array of all users
extern int nr_of_users; // The actual number of registered users, maximum 200

extern int current_user_id; // UserID of currently logged in user


/* Load and store */

// Loads account data of all users saved in text file
// Returns 0 if successful, 1 otherwise
int load_users(FILE *file); 

// Stores account data of all users to text file
// Returns 0 if successful, 1 otherwise
int store_users(FILE *file);


/* Login and register */

// Interface to log in
// Returns 0 if user successfully logged in, 1 otherwise
int login_procedure();

// Returns UserID that matches provided username, -1 if none found
static int find_username(const char *username); 

//Returns 0 if passwords match, 1 otherwise
static int compare_password(int user_id, const char *password); 

// Interface to register a new user
// Returns 0 if user successfully registered, 1 otherwise
int register_procedure();


/* Utilities */

// Returns 1 if user with provided email already exists, 0 otherwise
int email_exists(const char *text); 

// Returns 1 if user with provided username already exists, 0 otherwise
int username_exists(const char *text);

// Returns 1 if text contains only alphanumeric characters and spaces, 0 otherwise
int only_letters_spaces(const char *text); 

// Returns 1 if text contains no spaces, 0 if it does
int no_spaces(const char *text); 

// Returns the number of '@' in text
int at_counter(const char *text); 