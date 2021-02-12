#define max_books 200

void search_procedure();
const char* lowercase(const char* text);
int borrow(struct Book book, int user_id);
int find_id_by_title(char title[]);
extern struct Book found_books_array[max_books];
void return_procedure();
int return_book(struct Book book, int user_id);