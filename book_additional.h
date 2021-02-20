#define max_books 200 // Maximum allowed number of different titles in the library, doesn't count copies
#define max_loans 500

struct Loan {
    int user_id;
    char* book_title;
};

extern struct Book books[max_books];
extern struct Loan loans[max_loans];
extern int nr_of_books;
extern int nr_of_loans;

int load_loans(FILE *fp);
int store_loans(FILE *fp);

/* --- Standard user functions --- */
void search_procedure(); // Search interface
int borrow(struct Book book, int user_id);
void return_procedure(); // Return interface
int return_book(struct Book book);

/* --- Librarian functions --- */
void add_procedure(); // Add book interface
void add_more_copies(); // Add more copies to an existing title
void add_new_book(); // Add a new title
void remove_procedure(); // Remove book interface

/* --- Utilities --- */
int get_nr_input(); // Reads input from user, returns the numeric input, or -1 if input wasn't numeric
char get_char_input(); // Reads and returns 1 character input from user

int only_digits(char title[]); // Returns 1 if string only contains digits, 0 otherwise
int nr_copies_borrowed(char title[]); // Returns nr of copies of given book, borrowed by the current user
int find_id_by_title(char title[]); // Returns id of the book with given title, or -1 if none were found
int title_exists(char title[]); // Returns 1 if book with title provided already exists, 0 otherwise
int is_borrowed(struct Book book); // Returns 1 if at least 1 copy of the title is currently borrowed, 0 otherwise