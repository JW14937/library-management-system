#define max_books 200 // Maximum allowed number of different titles in the library, doesn't count copies
#define max_loans 500

/* ---------- 
        Additional header file for book_management.c ---------- */

struct Loan {
    int user_id; // ID of the user who borrowed the book
    char* book_title; // Title of the borrowed book
};

extern struct Book books[max_books]; // All books in the library
extern struct Loan loans[max_loans]; // All current loans
extern int nr_of_books; // Number of all books in the library
extern int nr_of_loans; // Number of all current loans


// Restores data from file
// Returns 0 if successful, 1 otherwise
int load_loans(FILE *fp);

// Saves data to file
// Returns 0 if successful, 1 otherwise
int store_loans(FILE *fp); 


/* --- Standard user functions --- */

// Search interface, with option to borrow found books
// Can call borrow
void search_procedure(); 

// Can be called by search_procedure
// Returns 0 if successful, 1 otherwise
static int borrow(struct Book book); 

// View borrowed books, with option to return them
// Can call return_book
void return_procedure(); 

// Called by return_procedure
// Returns 0 if successful
static int return_book(struct Book book); 


/* --- Librarian functions --- */

// Interface to either add more copies, or add a new book
// Can call add_more copies or add_new_book
void add_procedure(); 

// Called by add_procedure
// Adds more copies to an existing title
static void add_more_copies(); 

// Called by add_procedure
// Interface to add a new title
static void add_new_book(); 

// Interface to remove a book
// Can call remove_book
void remove_procedure(); 


/* --- Utilities --- */

// Reads input from user
// Returns the numeric input, or -1 if input wasn't numeric
int get_nr_input(); 

// Reads and returns 1 character input from user
char get_char_input(); 

// Returns 1 if string only contains digits, 0 otherwise
int only_digits(const char * title); 

// Returns nr of copies of given book, borrowed by the current user
static int nr_copies_borrowed(const char * title);

 // Returns id of the book with given title, or -1 if none were found
static int find_id_by_title(const char * title);

// Returns 1 if book with title provided already exists, 0 otherwise
static int title_exists(const char * title); 

// Returns 1 if at least 1 copy of the title is currently borrowed, 0 otherwise
static int is_borrowed(struct Book book); 