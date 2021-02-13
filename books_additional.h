#define max_books 200 //Maximum allowed number of different titles in the library, doesn't count copies

extern struct Book found_books_array[max_books];
void search_procedure(); //Search interface

int borrow(struct Book book, int user_id);

int find_id_by_title(char title[]);

void return_procedure(); //Return interface
int return_book(struct Book book, int user_id);

void add_procedure(); //Add book interface
int title_exists(char title[]); //Returns 1 if book with title provided already exists, 0 otherwise