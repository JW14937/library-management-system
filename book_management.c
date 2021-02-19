#include "book_management.h"
#include "books_additional.h"
#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Book books[max_books];
struct Loan loans[max_loans];
int nr_of_books;
int nr_of_loans;

/* --- Save and load functions --- */

int load_books(FILE *file) {

    nr_of_books = 0;

    file = fopen("books.txt", "r");
    if(file == NULL) return 1;
    
    char line[100];

    while(fgets(line, 100, file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        static int i = 1;

        if(i == 1) {
            books[nr_of_books].title = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(books[nr_of_books].title, line);
        }
        else if(i == 2) {
            books[nr_of_books].authors = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(books[nr_of_books].authors, line);
        }
        else if(i == 3) books[nr_of_books].year = atoi(line);
        else if(i == 4) books[nr_of_books].copies = atoi(line);
        
        i += 1;
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
        fprintf(file, "%s\n", books[i].title);
        fprintf(file, "%s\n", books[i].authors);
        fprintf(file, "%d\n", books[i].year);
        fprintf(file, "%d\n", books[i].copies);
    }

    fclose(file);

    return 0;
}

int load_loans(FILE *file) {

    nr_of_loans = 0;

    file = fopen("loans.txt", "r");
    if(file == NULL) return 1;

    char line[100];

    while(fgets(line, 100, file) != NULL) {
        line[strcspn(line, "\n")] = 0;
        static int i = 1;

        //UserID line
        if(i == 1) {
            loans[nr_of_loans].user_id = atoi(line);
            i = 0;
            
        }

        //Book title line
        else if(i == 0) {
            loans[nr_of_loans].book_title = (char*)malloc(strlen(line) * sizeof(char));
            strcpy(loans[nr_of_loans].book_title, line);
            
            i = 1;
            nr_of_loans++;
        }
    }

    return 0;
}

int store_loans(FILE* file) {

    file = fopen("loans.txt", "w");
    if(file == NULL) return 1;

    for(int i=0; i<nr_of_loans; i++) {
        if(loans[i].user_id == -1) { continue; } //Don't save returned books
        fprintf(file, "%d\n", loans[i].user_id);
        fprintf(file, "%s\n", loans[i].book_title);
    }

    fclose(file);

    return 0;
}


/* --- Standard user functions --- */

struct Book found_books_array[max_books];

struct BookArray find_book_by_title (const char *title) {
    
    struct BookArray found_books;
    found_books.array = found_books_array;
    found_books.length = 0;

    // Turn search title into lowercase, to make the search lower/uppercase independent
    char search_title_lcase[strlen(title)];
    for(int i=0; i<=strlen(title); i++) {
        if(title[i] >= 65 && title[i] <= 90) search_title_lcase[i] = title[i]+('a'-'A');
        else search_title_lcase[i] = title[i];
    }

    for(int i=0; i<nr_of_books; i++) {

        // Turn compare title into lowercase
        char full_title_lcase[strlen(books[i].title)];
        for(int j=0; j<=strlen(books[i].title); j++) {
            if(books[i].title[j] >= 65 && books[i].title[j] <= 90) full_title_lcase[j] = books[i].title[j]+('a'-'A');
            else full_title_lcase[j] = books[i].title[j];
        }

        if(strstr(full_title_lcase, search_title_lcase) != NULL) {
            found_books_array[found_books.length] = books[i];
            found_books.length ++;
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

    for(int i=0; i<nr_of_books; i++) {

        // Turn compare author into lowercase
        char full_author_lcase[strlen(books[i].authors)];
        for(int j=0; j<=strlen(books[i].authors); j++) {
            if(books[i].authors[j] >= 65 && books[i].authors[j] <= 90) full_author_lcase[j] = books[i].authors[j]+('a'-'A');
            else full_author_lcase[j] = books[i].authors[j];
        }

        if(strstr(full_author_lcase, search_author_lcase) != NULL) {
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

    for(int i=0; i<nr_of_books; i++) {
        if(books[i].year == year) {
            found_books_array[found_books.length] = books[i];
            found_books.length ++;
        }
    }

    return found_books;
}

void search_procedure() {

    struct BookArray found_books;

    printf("\nType \"title\" to search by TITLE, \"author\" to search by AUTHOR, \"year\" to search by YEAR: ");
    char search_option[20];
    fgets(search_option, 20, stdin);
    search_option[strcspn(search_option, "\n")] = 0;
    
    if(strcmp(search_option, "title")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");

        fgets(search_terms, 50, stdin);
        search_terms[strcspn(search_terms, "\n")] = 0;
        
        found_books = find_book_by_title(search_terms);
    }
    else if(strcmp(search_option, "author")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");

        fgets(search_terms, 50, stdin);
        search_terms[strcspn(search_terms, "\n")] = 0;

        found_books = find_book_by_author(search_terms);
    }
    else if(strcmp(search_option, "year")==0) {
        int search_year;
        printf("\nEnter search year: ");
        scanf("%4d", &search_year);
        getchar(); // Clear the buffer

        if(search_year < 0) {
            printf("\n>> Error << Invalid year (must be >0)\n");
            return;
        }
        found_books = find_book_by_year(search_year);
    }
    else {
        printf("\n>> Error << Option not recognised\n");
        return;
    }

    // Count nr of books that are actually available - have nr of available copies > 0
    int actually_available = 0;
    for(int i=0; i<found_books.length; i++) {
        if((*(found_books.array+i)).copies > 0) { actually_available++; }
    }

    if(actually_available==0) {
        printf("\n--- No results match your search ---\n");
        return;
    }

    printf("\n--- %d search result(s) have been found: ---\n", actually_available);

    for(int i=0; i<found_books.length; i++) {
        if((*(found_books.array+i)).copies < 1) { continue; } // Don't show books with no available copies
        printf("\nResult #%d: \n", i+1);
        printf("Title: %s\n", (*(found_books.array+i)).title);
        printf("Author(s): %s\n", (*(found_books.array+i)).authors);
        printf("Year published: %d\n", (*(found_books.array+i)).year);
        printf("Copies available: %d\n", (*(found_books.array+i)).copies);
    }

    if(actually_available > 1) { printf("\nWould you like to borrow one of these books? (y/n) "); }
    else { printf("\nWould you like to borrow this book? (y/n) "); }
    char answer;
    answer = getchar();
    getchar(); // Clear the buffer

    if(answer == 'y') {

        int result_nr = 1;

        if(actually_available > 1) {
            printf("\nWhich of the results would you like to borrow? Please type the number (without #): ");
            scanf("%3d", &result_nr);
            getchar();
        }
        
        if(result_nr > 0 && result_nr <= found_books.length) {
            int error_code = borrow(found_books_array[result_nr-1], current_user_id);
        }
        else {
            printf("\n>> Error << You were supposed to pick one of the numbers!\n");
        }
    }

}

int borrow(struct Book book, int user_id) {

    if(book.copies < 1) {
        printf("\n>> Error << Sorry, no copies currently available! Try again later\n");
        return 1;
    }

    int book_id = find_id_by_title(book.title);
    if(book_id < 0) {
        printf("\n>> Error << Book id not found\n");
        return 1;
    }
    books[book_id].copies -= 1;

    loans[nr_of_loans].book_title = (char*)malloc(strlen(book.title) * sizeof(char));
    strcpy(loans[nr_of_loans].book_title, book.title);
    loans[nr_of_loans].user_id = current_user_id;
    nr_of_loans++;

    printf("\n\n--- You have borrowed \"%s\" ---\n", book.title);
    
    return 0;
}

void return_procedure() {

    struct Book books_found[nr_of_books];
    int nr_books_found = 0;

    for(int i=0; i<nr_of_loans; i++) {
        if(loans[i].user_id == current_user_id) {
            books_found[nr_books_found] = books[find_id_by_title(loans[i].book_title)];
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
        printf("Title: %s\n", books_found[i].title);
        printf("Author(s): %s\n", books_found[i].authors);
        printf("Year published: %d\n", books_found[i].year);
    }

    if(nr_books_found > 1) { printf("\nWould you like to return one of these books? (y/n) "); }
    else { printf("\nWould you like to return this book? (y/n) "); }
    char answer;
    answer = getchar();
    getchar(); // Clear the buffer

    if(answer == 'y') {

        int result_nr = 1;

        if(nr_books_found > 1) {
            printf("\nWhich of the results would you like to return? Please type the number (without #): ");
            scanf("%3d", &result_nr);
            getchar();
        }
        
        if(result_nr > 0 && result_nr <= nr_books_found) {
            int error_code = return_book(books_found[result_nr-1]);
            if(error_code==0) { printf("\n--- You have successfully returned \"%s\"! ---\n", books_found[result_nr-1].title); }
            else { printf("\n>> Error << Error returning book\n"); }
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers!\n"); }
    }
}

int return_book(struct Book book) {

    for(int i=0; i<nr_of_loans; i++) {
        if(loans[i].user_id == current_user_id) {
            if(strcmp(loans[i].book_title, book.title) == 0) {
                loans[i].user_id = -1;
                books[find_id_by_title(loans[i].book_title)].copies ++;
                break;
            }
        }
    }

    FILE *fp;
    store_loans(fp);
    load_loans(fp);

    return 0;
}


/* --- Librarian functions --- */

void add_procedure() {
    printf("\nWould you like to (1) add new copies to an existing title, or (2) add a new title?");
    printf("\nYour choice (type \"1\" or \"2\"): ");

    int choice;
    scanf("%3d", &choice);
    getchar();

    if(choice == 1) { add_more_copies(); }
    else if(choice == 2) { add_new_book(); }
    else { printf("\n>> Error << You were supposed to type either \"1\" or \"2\";"); }
}

void add_more_copies() {
    printf("\nWhich book do you want to add copies to? Search by title: ");

    char search_terms[50];
    fgets(search_terms, 50, stdin);
    search_terms[strcspn(search_terms, "\n")] = 0;
    
    struct BookArray found_books = find_book_by_title(search_terms);

    if(found_books.length == 0) {
        printf("\n--- No books matching your search were found in the library ---\n");
        return;
    }
    printf("%s", found_books.array->authors);

    for(int i=0; i<found_books.length; i++) {
        printf("\n\nResult nr #%d", i+1);
        printf("\nTitle: %s", (*(found_books.array+i)).title);
        printf("\nAuthor(s): %s", (*(found_books.array+i)).authors);
        printf("\nYear: %d", (*(found_books.array+i)).year);
        printf("\nAvailable copies: %d", (*(found_books.array+i)).copies);
    }

    if(found_books.length > 1) { printf("\n\nWould you like to add more copies of one of these books? (y/n) "); }
    else { printf("\n\nWould you like to add more copies of this book? (y/n) "); }
    char answer;
    answer = getchar();
    getchar(); // Clear the buffer

    if(answer == 'y') {

        int result_nr = 1;

        if(found_books.length > 1) {
            printf("\nWhich of the results would you like to add the copies to? Please type the number (without #): ");
            scanf("%3d", &result_nr);
            getchar();
        }
        
        if(result_nr > 0 && result_nr <= found_books.length) {
            int nr_copies = 0;
            printf("\nEnter the number of copies to add: ");
            scanf("%3d", &nr_copies);
            getchar();
            books[find_id_by_title(found_books_array[result_nr-1].title)].copies += nr_copies;
            printf("\n--- Added %d more copies of \"%s\" ---\n", nr_copies, found_books_array[result_nr-1].title);
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers\n"); }
    }

}

void add_new_book() {

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

    char authors[100];
    printf("\nEnter the author(s) of the new book (max 100 characters): ");

    fgets(authors, 100, stdin);
    authors[strcspn(authors, "\n")] = 0;

    if(strlen(authors) < 3 || strlen(authors) > 100) {
        printf("\n>> Error << Authors name must be at between 3 - 100 characters long!\n");
        return;
    }

    int year;
    printf("\nEnter the year the book was published: ");
    scanf("%4d", year);
    getchar();

    if(year < 0 || year > 2021) {
        printf("\n>> Error << Invalid year - must be between 0 and 2021\n");
        return;
    }

    int nr_copies;
    printf("\nEnter the number of copies available: (max. 999)");
    scanf("%3d", nr_copies);
    getchar();

    if(nr_copies < 0) {
        printf("\n>> Error << Invalid number of copies - must be >= 0\n");
        return;
    }

    struct Book new_book;
    new_book.title = title;
    new_book.authors = authors;
    new_book.year = year;
    new_book.copies = nr_copies;

    if(add_book(new_book) == 0) { printf("\n--- You have added \"%s\" to the library! ---\n", new_book.title); }
    else { printf("\n>> Error << Book could not be added\n"); }

}

int add_book(struct Book book) {

    books[nr_of_books].title = (char*)malloc(strlen(book.title) * sizeof(char));
    strcpy(books[nr_of_books].title, book.title);
    books[nr_of_books].authors = (char*)malloc(strlen(book.authors) * sizeof(char));
    strcpy(books[nr_of_books].authors, book.authors);
    books[nr_of_books].year = book.year;
    books[nr_of_books].copies = book.copies;

    nr_of_books++;

    return 0;
}

void remove_procedure() {
    printf("\nWhich book do you want to remove? Search by title: ");

    char search_terms[50];
    fgets(search_terms, 50, stdin);
    search_terms[strcspn(search_terms, "\n")] = 0;
    
    struct BookArray found_books = find_book_by_title(search_terms);

    if(found_books.length == 0) {
        printf("\n--- No books matching your search were found in the library ---\n");
        return;
    }

    printf("%s", found_books.array->authors);

    for(int i=0; i<found_books.length; i++) {
        printf("\n\nResult nr #%d", i+1);
        printf("\nTitle: %s", (*(found_books.array+i)).title);
        printf("\nAuthor(s): %s", (*(found_books.array+i)).authors);
        printf("\nYear: %d", (*(found_books.array+i)).year);
        printf("\nAvailable copies: %d", (*(found_books.array+i)).copies);
    }

    if(found_books.length > 1) { printf("\n\nWould you like to remove all available copies of one of these books? (y/n) "); }
    else { printf("\n\nWould you like to remove all available copies of this book? (y/n) "); }
    char answer;
    answer = getchar();
    getchar(); // Clear the buffer

    if(answer == 'y') {

        int result_nr = 1;

        if(found_books.length > 1) {
            printf("\nWhich of the results would you like to remove? Please type the number (without #): ");
            scanf("%3d", &result_nr);
            getchar();
        }
        
        if(result_nr > 0 && result_nr <= found_books.length) {
            remove_book(*(found_books.array+result_nr-1));
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers\n"); }
    }
}

int remove_book(struct Book book) {
    books[find_id_by_title(book.title)].copies = 0;
    printf("\n --- You have removed all available copies of %s --- \nThis does not prevent copies already borrowed to be returned\n", book.title);
    return 0;
}

/* --- Utilities --- */

int title_exists(char title[]) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(title, books[i].title) == 0) { return 1; }
    }

    return 0;
}

int find_id_by_title (char title[]) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(books[i].title, title)==0) return i;
    }

    return -1;
}