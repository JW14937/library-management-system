#include "book_management.h"
#include "book_additional.h"
#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- 
        This file contains functions related to managing books 
        General descriptions of all functions in header files       ---------- */


struct Book books[max_books]; // Array of all titles in the library
struct Loan loans[max_loans]; // Array of all active loans
int nr_of_books; // Counting all titles in the library
int nr_of_loans; // Counting all active loans

/* --- Save and load functions --- */

int load_books(FILE *file) {

    nr_of_books = 0; // Zero it out, in case of reload

    file = fopen("books.txt", "r");
    if(file == NULL) return 1;
    
    char line[100];

    // Reading books.txt line by line
    while(fgets(line, 100, file) != NULL) {

        line[strcspn(line, "\n")] = 0;
        static int i = 1; // Used to distinguish between lines containing different data

        // Title line
        if(i == 1) {
            books[nr_of_books].title = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(books[nr_of_books].title, line);
        }

        // Authors line
        else if(i == 2) {
            books[nr_of_books].authors = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(books[nr_of_books].authors, line);
        }

        // Year published line
        else if(i == 3) books[nr_of_books].year = atoi(line);

        // Nr of available copies line
        else if(i == 4) books[nr_of_books].copies = atoi(line);
        
        i += 1;

        // Reset the line data counter, once all data for 1 book has been read
        if(i > 4) {
            i = 1;
            nr_of_books += 1;
        }
    }

    fclose(file);
    return 0;
}

int store_books(FILE *file) {

    file = fopen("books.txt", "w");
    if(file == NULL) return 1;

    for(int i=0; i<nr_of_books; i++) {

        // Book marked to be removed - don't save
        if(books[i].year == 9999) { 
            free(books[i].title);
            free(books[i].authors);
            continue; 
        }
        fprintf(file, "%s\n", books[i].title);
        fprintf(file, "%s\n", books[i].authors);
        fprintf(file, "%d\n", books[i].year);
        fprintf(file, "%d\n", books[i].copies);

        free(books[i].title);
        free(books[i].authors);
    }

    fclose(file);

    return 0;
}

int load_loans(FILE *file) {

    nr_of_loans = 0; // Zero it out, in case of reload

    file = fopen("loans.txt", "r");
    if(file == NULL) return 1;

    char line[100];

    // Read loans.txt line by line
    while(fgets(line, 100, file) != NULL) {
        
        line[strcspn(line, "\n")] = 0;
        static int i = 1; // Used to distinguish between lines containing different data

        // UserID line
        if(i == 1) {
            loans[nr_of_loans].user_id = atoi(line);
            i = 0; // Next line contains book title
        }

        // Book title line
        else if(i == 0) {
            loans[nr_of_loans].book_title = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(loans[nr_of_loans].book_title, line);
            
            i = 1; // Next line contains UserID of next loan
            nr_of_loans++; // All data read for 1 loan
        }
    }

    return 0;
}

int store_loans(FILE* file) {

    file = fopen("loans.txt", "w");
    if(file == NULL) return 1;

    for(int i=0; i<nr_of_loans; i++) {

        // Loan marked returned - don't save
        if(loans[i].user_id == -1) { 
            free(loans[i].book_title);
            continue; 
        } 
        fprintf(file, "%d\n", loans[i].user_id);
        fprintf(file, "%s\n", loans[i].book_title);
        free(loans[i].book_title);
    }

    fclose(file);

    return 0;
}


/* --- Search functions --- */

struct Book found_books_array[max_books]; // Array used to store found books

struct BookArray find_book_by_title (const char *title) {
    
    struct BookArray found_books;
    found_books.array = found_books_array;
    found_books.length = 0;

    // Turn search title into lowercase, to make the search case-independent
    char search_title_lcase[strlen(title)];
    for(int i=0; i<=strlen(title); i++) {
        if(title[i] >= 65 && title[i] <= 90) search_title_lcase[i] = title[i]+('a'-'A');
        else search_title_lcase[i] = title[i];
    }

    // Look for the search terms inside titles of all books
    for(int i=0; i<nr_of_books; i++) {

        // Turn compare title into lowercase
        char full_title_lcase[strlen(books[i].title)];
        for(int j=0; j<=strlen(books[i].title); j++) {
            if(books[i].title[j] >= 65 && books[i].title[j] <= 90) full_title_lcase[j] = books[i].title[j]+('a'-'A');
            else full_title_lcase[j] = books[i].title[j];
        }

        if(strstr(full_title_lcase, search_title_lcase) != NULL) {

            // Search terms found inside the title of this book with index i
            found_books_array[found_books.length] = books[i];
            found_books.length++;
        }
    }
    
    return found_books;
}

struct BookArray find_book_by_author (const char *author) {

    struct BookArray found_books;
    found_books.array = found_books_array;
    found_books.length = 0;

    // Turn search author into lowercase, to make the search lower/uppercase independent
    char search_author_lcase[strlen(author)];
    for(int i=0; i<=strlen(author); i++) {
        if(author[i] >= 65 && author[i] <= 90) search_author_lcase[i] = author[i]+('a'-'A');
        else search_author_lcase[i] = author[i];
    }

    // Look for the search terms inside authors of all books
    for(int i=0; i<nr_of_books; i++) {

        // Turn compare author into lowercase
        char full_author_lcase[strlen(books[i].authors)];
        for(int j=0; j<=strlen(books[i].authors); j++) {
            if(books[i].authors[j] >= 65 && books[i].authors[j] <= 90) full_author_lcase[j] = books[i].authors[j]+('a'-'A');
            else full_author_lcase[j] = books[i].authors[j];
        }

        if(strstr(full_author_lcase, search_author_lcase) != NULL) {

            // Search terms found inside authors of this book with index i
            found_books_array[found_books.length] = books[i];
            found_books.length ++;
        }
    }

    return found_books;
}

struct BookArray find_book_by_year (unsigned int year) {

    struct BookArray found_books;
    found_books.array = found_books_array;
    found_books.length = 0;

    // Find all books where year exactly matches search year
    for(int i=0; i<nr_of_books; i++) {
        if(books[i].year == year) {
            found_books_array[found_books.length] = books[i];
            found_books.length++;
        }
    }

    return found_books;
}

// The interface to search for books with option of borrowing
void search_procedure() {

    struct BookArray found_books;

    // 3 options of search

    printf("\nType \"title\" to search by TITLE, \"author\" to search by AUTHOR, \"year\" to search by YEAR: ");
    char search_option[20];
    fgets(search_option, 20, stdin);
    search_option[strcspn(search_option, "\n")] = 0;
    
    // Search by title
    if(strcmp(search_option, "title")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");

        fgets(search_terms, 50, stdin);
        search_terms[strcspn(search_terms, "\n")] = 0;
        
        found_books = find_book_by_title(search_terms);
    }

    // Search by author
    else if(strcmp(search_option, "author")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");

        fgets(search_terms, 50, stdin);
        search_terms[strcspn(search_terms, "\n")] = 0;

        // Check for valid user input
        if(only_digits(search_terms) == 1) {
            printf("\n>> Error << Author field cannot only contain digits\n");
            return;
        }

        found_books = find_book_by_author(search_terms);
    }

    // Search by year
    else if(strcmp(search_option, "year")==0) {
        char search_year_str[6];
        printf("\nEnter search year (digits only): ");

        fgets(search_year_str, 6, stdin);
        search_year_str[strcspn(search_year_str, "\n")] = 0;

        // Check for valid user input
        if(!only_digits(search_year_str)) {
            printf("\n>> Error << Year can only contain digits\n");
            return;
        }

        int search_year = atoi(search_year_str);

        if(search_year < 0) {
            printf("\n>> Error << Invalid year (must be > 0)\n");
            return;
        }
        found_books = find_book_by_year(search_year);
    }

    // Invalid user input for option
    else {
        printf("\n>> Error << Option not recognised\n");
        return;
    }

    // No books found
    if(found_books.length == 0) {
        printf("\n--- No results match your search ---\n");
        return;
    }

    // Books found
    printf("\n--- %d search result(s) found: ---\n", found_books.length);

    for(int i=0; i<found_books.length; i++) {
        printf("\nResult #%d: \n", i+1);
        printf("Title: %s\n", (*(found_books.array+i)).title);
        printf("Author(s): %s\n", (*(found_books.array+i)).authors);
        printf("Year published: %d\n", (*(found_books.array+i)).year);
        printf("Copies available: %d\n", (*(found_books.array+i)).copies);
    }

    // Option to borrow

    if(found_books.length > 1) { printf("\nWould you like to borrow one of these books? (y/n) "); }
    else { printf("\nWould you like to borrow this book? (y/n) "); }
    char answer = get_char_input();

    if(answer == 'y') {

        int result_nr = 1;

        // Pick one of the found results to borrow
        if(found_books.length > 1) {
            printf("\nWhich of the results would you like to borrow? Please type the number (without #): ");
            result_nr = get_nr_input();
        }
        
        // Check for valid user input
        if(result_nr > 0 && result_nr <= found_books.length) {
            int error_code = borrow(found_books_array[result_nr-1]); // Send to borrow function

            if(error_code == 0) { printf("\n\n--- You have borrowed \"%s\" ---\n", found_books_array[result_nr-1].title); }
            else { printf("\nCouldn't borrow %s\n", found_books_array[result_nr-1].title); }
        }
        else {
            printf("\n>> Error << You were supposed to pick one of the numbers!\n");
        }
    }

}

/* --- Standard user functions: borrow and return --- */

static int borrow(struct Book book) {

    // Checks if possible to borrow this book
    if(book.copies < 1) {
        printf("\n>> Error << Sorry, no copies currently available! Try again later\n");
        return 1;
    }

    if(nr_copies_borrowed(book.title) >= 2) {
        printf("\n>> Error << You cannot borrow more than 2 copies of the same title\n");
        return 1;
    }

    // Passed the checks
    int book_id = find_id_by_title(book.title);
    if(book_id < 0) {
        printf("\n>> Error << Book id not found\n");
        return 1;
    }
    books[book_id].copies--;

    // Add a new loan
    loans[nr_of_loans].book_title = (char*)malloc(strlen(book.title) * sizeof(char));
    strcpy(loans[nr_of_loans].book_title, book.title);
    loans[nr_of_loans].user_id = current_user_id;
    nr_of_loans++;
    
    return 0;
}

// The interface to see borrowed books with option of returning
void return_procedure() {

    // Print out all books borrowed by this user

    int book_ids[nr_of_books]; // Array of ID numbers of found books
    int nr_books_found = 0;

    for(int i=0; i<nr_of_loans; i++) {
        if(loans[i].user_id == current_user_id) {
            book_ids[nr_books_found] = find_id_by_title(loans[i].book_title);
            nr_books_found++;
        }
    }

    if(nr_books_found == 0) {
        printf("\n--- You have 0 borrowed books ---\n");
        return;
    }

    printf("\n--- You have %i borrowed book(s) ---\n", nr_books_found);

    for(int i=0; i<nr_books_found; i++) {
        printf("\nResult #%d: \n", i+1);
        printf("Title: %s\n", books[book_ids[i]].title);
        printf("Author(s): %s\n", books[book_ids[i]].authors);
        printf("Year published: %d\n", books[book_ids[i]].year);
    }


    // Option of returning 1 of the borrowed books

    if(nr_books_found > 1) { printf("\nWould you like to return one of these books? (y/n) "); }
    else { printf("\nWould you like to return this book? (y/n) "); }
    char answer = get_char_input();

    if(answer == 'y') {

        int result_nr = 1;

        if(nr_books_found > 1) {
            // Pick one of the found results to borrow
            printf("\nWhich of the results would you like to return? Please type the number (without #): ");
            result_nr = get_nr_input();
        }
        
        // Check validity of user input
        if(result_nr > 0 && result_nr <= nr_books_found) {
            int error_code = return_book(books[book_ids[result_nr-1]]); // Send to return_book function
            if(error_code==0) { printf("\n--- You have successfully returned \"%s\"! ---\n", books[book_ids[result_nr-1]].title); }
            else { printf("\n>> Error << Error returning book\n"); }
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers!\n"); }
    }
}

static int return_book(struct Book book) {

    for(int i=0; i<nr_of_loans; i++) {

        if(loans[i].user_id == current_user_id) {

            if(strcmp(loans[i].book_title, book.title) == 0) {

                loans[i].user_id = -1; // Mark for deletion for the next time store_loans runs
                books[find_id_by_title(loans[i].book_title)].copies++;
                break; // Only return 1 copy
            }
        }
    }

    // Reload the loans to remove the returned book
    FILE *fp;
    store_loans(fp);
    load_loans(fp);

    return 0;
}


/* --- Librarian functions: add and remove --- */

// The interface to pick between 2 options
void add_procedure() {

    printf("\nWould you like to (1) add new copies to an existing title, or (2) add a new title?");
    printf("\nYour choice (type \"1\" or \"2\"): ");

    int choice = get_nr_input();

    if(choice == 1) { add_more_copies(); }
    else if(choice == 2) { add_new_book(); }
    else { printf("\n>> Error << You were supposed to type either \"1\" or \"2\"\n"); }
}

static void add_more_copies() {

    // First, find the right book

    printf("\nWhich book do you want to add copies to? Search by title: ");

    char search_terms[50];
    fgets(search_terms, 50, stdin);
    search_terms[strcspn(search_terms, "\n")] = 0;
    
    struct BookArray found_books = find_book_by_title(search_terms);

    if(found_books.length == 0) {
        printf("\n--- No books matching your search were found in the library ---\n");
        return;
    }

    for(int i=0; i<found_books.length; i++) {
        printf("\n\nResult nr #%d", i+1);
        printf("\nTitle: %s", (*(found_books.array+i)).title);
        printf("\nAuthor(s): %s", (*(found_books.array+i)).authors);
        printf("\nYear: %d", (*(found_books.array+i)).year);
        printf("\nAvailable copies: %d", (*(found_books.array+i)).copies);
    }

    // Now, option of adding more copies

    if(found_books.length > 1) { printf("\n\nWould you like to add more copies of one of these books? (y/n) "); }
    else { printf("\n\nWould you like to add more copies of this book? (y/n) "); }
    char answer = get_char_input();

    if(answer == 'y') {

        int result_nr = 1;

        if(found_books.length > 1) {
            printf("\nWhich of the results would you like to add the copies to? Please type the number (without #): ");
            result_nr = get_nr_input();
        }
        
        // Check for validity of user input
        if(result_nr > 0 && result_nr <= found_books.length) {
            
            printf("\nEnter the number of copies to add: ");
            int nr_copies = get_nr_input();
            if(nr_copies < 1) {
                printf("\n>> Error << Invalid input for nr of copies - must be >0\n");
                return;
            }

            // Increase the nr of copies
            books[find_id_by_title(found_books_array[result_nr-1].title)].copies += nr_copies;
            printf("\n--- Added %d more copies of \"%s\" ---\n", nr_copies, found_books_array[result_nr-1].title);
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers\n"); }
    }

}

// The interface of adding a new book
static void add_new_book() {

    // Title
    char title[50];
    printf("\nEnter the title of the new book (max 50 characters): ");

    fgets(title, 50, stdin);
    title[strcspn(title, "\n")] = 0;

    if(title_exists(title) == 1) {
        printf("\n>> Error << This book is already in the library!\n");
        return;
    }
    if(strlen(title) < 1 || strlen(title) > 50) {
        printf("\n>> Error << Book title must be between 1 - 50 characters long!\n");
        return;
    }

    // Authors
    char authors[100];
    printf("\nEnter the author(s) of the new book (max 100 characters): ");

    fgets(authors, 100, stdin);
    authors[strcspn(authors, "\n")] = 0;

    if(strlen(authors) < 3 || strlen(authors) > 100) {
        printf("\n>> Error << Authors name must be at between 3 - 100 characters long!\n");
        return;
    }

    // Year
    printf("\nEnter the year the book was published: ");
    int year = get_nr_input();

    if(year < 0 || year > 2050) {
        printf("\n>> Error << Invalid year - must be between 0 and 2050\n");
        return;
    }

    // Copies available
    printf("\nEnter the number of copies available (max. 999): ");
    int nr_copies = get_nr_input();

    if(nr_copies < 0 || nr_copies > 999) {
        printf("\n>> Error << Invalid number of copies - must be between 0 and 999\n");
        return;
    }

    // Passed all the checks
    struct Book new_book;
    new_book.title = title;
    new_book.authors = authors;
    new_book.year = year;
    new_book.copies = nr_copies;

    if(add_book(new_book) == 0) { printf("\n--- You have added \"%s\" to the library! ---\n", new_book.title); }
    else { printf("\n>> Error << Book could not be added.\n"); }

}

int add_book(struct Book book) {

    if(nr_of_books >= max_books) { 
        printf("\nThe maximum capacity of the library has been reached\n");
        return 1; 
    }

    books[nr_of_books].title = (char*)malloc(strlen(book.title) * sizeof(char));
    strcpy(books[nr_of_books].title, book.title);
    books[nr_of_books].authors = (char*)malloc(strlen(book.authors) * sizeof(char));
    strcpy(books[nr_of_books].authors, book.authors);
    books[nr_of_books].year = book.year;
    books[nr_of_books].copies = book.copies;

    nr_of_books++;

    return 0;
}

// The interface to remove a book from the library
void remove_procedure() {

    // Exact title
    printf("\nWhich book do you want to remove? Enter the exact title: ");
    char title[50];
    fgets(title, 50, stdin);
    title[strcspn(title, "\n")] = 0;

    int book_id = find_id_by_title(title);

    if(book_id == -1) {
        printf("\n>> Error << Book with this exact title doesn't exist\n");
        return;
    }

    // Exact author
    printf("Enter the exact author: ");
    char author[100];
    fgets(author, 100, stdin);
    author[strcspn(author, "\n")] = 0;

    if(strcmp(author, books[book_id].authors)!=0) {
        printf("\n>> Error << Book with this exact title and author doesn't exist\n");
        return;
    }

    // Exact year
    printf("Enter the year book was published: ");
    int year = get_nr_input();

    if(year != books[book_id].year) {
        printf("\n>> Error << Book with this exact title, author and year doesn't exist\n");
        return;
    }

    if(is_borrowed(books[book_id])) {
        printf("\n>> Error << You cannot remove this title, because some copies are currently borrowed\n");
        return;
    }
    
    // Passed all the checks - send to remove_book function
    if(remove_book(books[book_id])==0) {
        printf("\n--- You have removed %s ---\n", title);
    }

    else { printf("\n>> Error << There was a problem removing the book\n"); }
}

int remove_book(struct Book book) {

    books[find_id_by_title(book.title)].year = 9999; // Mark title to be deleted with next save

    // Reload books to update the books array
    FILE *fp;
    if(store_books(fp) != 0) { return 1; }
    if(load_books(fp) != 0) { return 1; }

    return 0;
}

/* --- Utilities --- */

int get_nr_input() {

    char input_str[10];
    fgets(input_str, 10, stdin);
    input_str[strcspn(input_str, "\n")] = 0;

    if(!only_digits(input_str)) { return -1; }
    return(atoi(input_str));
}

char get_char_input() {

    char input = getchar();
    getchar(); // Clear the buffer
    
    return input;
}

int only_digits(const char * title) {

    for(int i=0; i<strlen(title); i++) {
        if(isdigit(title[i]) == 0) { return 0; }
    }

    return 1;
}

static int title_exists(const char * title) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(title, books[i].title) == 0) { return 1; }
    }

    return 0;
}

static int find_id_by_title (const char * title) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(books[i].title, title)==0) return i;
    }

    return -1;
}

static int nr_copies_borrowed(const char * title) {

    int count = 0;

    for(int i=0; i<nr_of_loans; i++) {
        if(loans[i].user_id == current_user_id && strcmp(loans[i].book_title, title)==0) { count++; }
    }

    return count;
}

static int is_borrowed(struct Book book) {

    for(int i=0; i<nr_of_loans; i++) {
        if(strcmp(book.title, loans[i].book_title)==0) { return 1; }
    }

    return 0;
}