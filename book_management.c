#include "book_management.h"
#include "books_additional.h"
#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Book books[max_books];
int nr_of_books = 0;

int load_books(FILE *file) {

    nr_of_books = 0; // Reset in case of re-load

    file = fopen("books.txt", "r");
    if(file == NULL) return 1;

    char line[256];

    while(fscanf(file, "%[^\n]\n", line) != -1) {
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

    file = fopen("buffer_books.txt", "w");
    if(file == NULL) return 1;

    for(int i=0; i<nr_of_books; i++) {
        fprintf(file, "%s\n", books[i].title);
        fprintf(file, "%s\n", books[i].authors);
        fprintf(file, "%d\n", books[i].year);
        fprintf(file, "%d\n", books[i].copies);
    }

    fclose(file);

    // Replace the out-of-date books.txt with the buffer file
    if(remove("books.txt") != 0) { return 1; }
    if(rename("buffer_books.txt", "books.txt") != 0) { return 1; }

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
    scanf("%s", search_option);
    
    if(strcmp(search_option, "title")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");
        scanf(" %[^\n]%*c", search_terms);
        found_books = find_book_by_title(search_terms);
    }
    else if(strcmp(search_option, "author")==0) {
        char search_terms[50];
        printf("\nEnter search term(s): ");
        scanf(" %[^\n]%*c", search_terms);
        found_books = find_book_by_author(search_terms);
    }
    else if(strcmp(search_option, "year")==0) {
        int search_year;
        printf("\nEnter search year: ");
        scanf("%d", &search_year);

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
    

    if(found_books.length==0) {
        printf("\n--- No results match your search ---\n");
        return;
    }

    printf("\n--- %d search result(s) have been found: ---\n", found_books.length);

    for(int i=0; i<found_books.length; i++) {
        printf("\nResult #%d: \n", i+1);
        printf("Title: %s\n", found_books_array[i].title);
        printf("Author(s): %s\n", found_books_array[i].authors);
        printf("Year published: %d\n", found_books_array[i].year);
        printf("Copies available: %d\n", found_books_array[i].copies);
    }

    printf("\nWould you like to borrow one of these books? (y/n) ");
    char answer;
    getchar(); //Get rid of newline from buffer
    answer = getchar();

    if(answer == 'y') {

        printf("\nWhich of the results would you like to borrow? Please type the number (without #): ");
        int result_nr;
        scanf("%1d", &result_nr);
        
        if(result_nr > 0 && result_nr <= found_books.length) {
            int error_code = borrow(found_books_array[result_nr-1], current_user_id);
        }
        else {
            printf("\n>> Error << You were supposed to pick one of the numbers!\n");
        }
    }

}

int borrow(struct Book book, int user_id) {

    if(book.copies <= 0) {
        printf("\n>> Error << Sorry, no copies currently available! Try again later\n");
        return 1;
    }

    FILE *fp;
    fp = fopen("loans.txt", "a");
    if(fp == NULL) {
        printf("\n Error opening file \n");
        return 1;
    }

    int book_id = find_id_by_title(book.title);
    if(book_id < 0) {
        printf("\n>> Error << Book id not found\n");
        return 1;
    }
    books[book_id].copies -= 1;

    fprintf(fp, "%d\n%s\n", user_id, book.title);

    printf("\n\n--- You have borrowed \"%s\" ---\n", book.title);
    
    fclose(fp);
    return 0;
}

void return_procedure() {

    int nr_books_found = 0;

    FILE *fp;
    fp = fopen("loans.txt", "r");
    if(fp == NULL) {
        printf("\n Error opening file \n");
        return;
    }

    char line[256];
    int line_is_user_id = 1; //To tell between user id lines and title lines

    while(fscanf(fp, "%[^\n]\n", line) != -1) {

        if(line_is_user_id==1) {

            line_is_user_id = 0;

            if(atoi(line)==current_user_id) {
                fscanf(fp, "%[^\n]\n", line);
                found_books_array[nr_books_found] = books[find_id_by_title(line)];
                line_is_user_id = 1;
                nr_books_found++;
            }
        }
        
        else { line_is_user_id = 1; }
    }

    fclose(fp);

    if(nr_books_found == 0) {
        printf("\n--- You have 0 borrowed books ---");
        return;
    }

    printf("\n--- You have %i borrowed book(s) ---\n", nr_books_found);

    for(int i=0; i<nr_books_found; i++) {
        printf("\nResult #%d: \n", i+1);
        printf("Title: %s\n", found_books_array[i].title);
        printf("Author(s): %s\n", found_books_array[i].authors);
        printf("Year published: %d\n", found_books_array[i].year);
    }

    printf("\nWould you like to return one of these books? (y/n) ");
    char answer;
    getchar();
    answer = getchar();

    if(answer == 'y') {
        printf("\nWhich of the results would you like to return? Please type the number (without #): ");
        int result_nr;
        scanf("%1d", &result_nr);
        
        if(result_nr > 0 && result_nr <= nr_books_found) {
            int error_code = return_book(found_books_array[result_nr-1], current_user_id);
            if(error_code==0) { printf("\n--- You have successfully returned \"%s\"! ---\n", found_books_array[result_nr-1].title); }
            else { printf("\n>> Error << Error returning book\n"); }
        }
        else { printf("\n>> Error << You were supposed to pick one of the numbers!\n"); }
    }
}

int return_book(struct Book book, int user_id) {

    /* --- Find line number to be deleted from loans.txt --- */
    int line_nr_to_delete = -1;

    FILE *fp;
    fp = fopen("loans.txt", "r");
    if(fp == NULL) {
        printf("\n Error opening file \n");
        return 1;
    }

    char line[256];
    int line_is_user_id = 1;
    int line_count = 1;

    while(fscanf(fp, "%[^\n]\n", line) != -1) {

        //User ID line
        if(line_is_user_id==1) {
            line_is_user_id = 0;

            if(atoi(line)==current_user_id) {
                fscanf(fp, "%[^\n]\n", line);
                line_count++;
                line_is_user_id = 1;

                if(strcmp(line, book.title)==0) {
                    line_nr_to_delete = line_count-1;
                    break;
                }
            }
        }

        //Book title line
        else { line_is_user_id = 1; }

        line_count++;
    }

    if(line_nr_to_delete == -1) { return 2; } //Error - couldn't find line containing book to return in loans.txt


    /* --- Copy loans.txt to buffer.txt, except for copying the found lines --- */

    rewind(fp);

    FILE *fp_buffer;
    fp_buffer = fopen("buffer_loans.txt", "w");
    if(fp_buffer == NULL) {
        printf("\n Error opening file \n");
        return 1;
    }

    line_count = 1;

    while(fscanf(fp, "%[^\n]\n", line) != -1) {

        // line_nr_to_delete = user id line; line_nr_to_delete+1 = title line
        if(line_count != line_nr_to_delete && line_count != line_nr_to_delete+1) {
            fprintf(fp_buffer, "%s\n", line);
        }

        line_count++;
    }

    fclose(fp);
    fclose(fp_buffer);

    // Replace the out-of-date loans.txt with the buffer file
    if(remove("loans.txt") != 0) { return 3; }
    if(rename("buffer_loans.txt", "loans.txt") != 0) { return 4; }
    
    books[find_id_by_title(book.title)].copies++;

    return 0;
}

int find_id_by_title (char title[]) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(books[i].title, title)==0) return i;
    }

    return -1;
}


/* --- Librarian functions --- */

void add_procedure() {

    char potential_title[50];
    printf("\nEnter the title of the new book (max 50 characters): ");
    scanf(" %[^\n]%*c", potential_title);

    if(title_exists(potential_title) == 1) {
        printf("\n>> Error << This book is already in the library!\n");
        return;
    }
    if(strlen(potential_title) < 1 || strlen(potential_title) > 50) {
        printf("\n>> Error << Book title must be between 1 - 50 characters long!\n");
        return;
    }

    char potential_author[100];
    printf("\nEnter the author(s) of the new book (max 100 characters): ");
    scanf(" %[^\n]%*c", potential_author);

    if(strlen(potential_author) < 3 || strlen(potential_author) > 100) {
        printf("\n>> Error << Authors name must be at between 3 - 100 characters long!\n");
        return;
    }

    int potential_year;
    printf("\nEnter the year the book was published: ");
    scanf("%d", &potential_year);

    if(potential_year < 0 || potential_year > 2021) {
        printf("\n>> Error << Invalid year - must be between 0 and 2021\n");
        return;
    }

    int potential_nr_of_copies;
    printf("\nEnter the number of copies available: ");
    scanf("%d", &potential_nr_of_copies);

    if(potential_nr_of_copies < 0) {
        printf("\n>> Error << Invalid number of copies - must be >= 0\n");
        return;
    }

    struct Book new_book;
    new_book.title = potential_title;
    new_book.authors = potential_author;
    new_book.year = potential_year;
    new_book.copies = potential_nr_of_copies;

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

    FILE* fp;
    if(store_books(fp) != 0) { return 1; }

    return 0;
}

int title_exists(char title[]) {

    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(title, books[i].title) == 0) { return 1; }
    }

    return 0;
}