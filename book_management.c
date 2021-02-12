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

void search_procedure() {

    printf("\nSearch by title - Enter search terms: ");
    char search_terms[50];
    
    scanf(" %[^\n]%*c", search_terms);

    struct BookArray found_books = find_book_by_title(search_terms);

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
    answer = getchar();
    if(answer == 'y') {
        printf("\nWhich of the results would you like to borrow? Please type the number (without #): ");
        int result_nr;
        scanf("%1d", &result_nr);
        //result nr
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
                    line_nr_to_delete = line_count;
                    break;
                }
            }
        }

        //Book title line
        else { line_is_user_id = 1; }

        line_count++;
    }

    if(line_nr_to_delete == -1) { return 2; } //Error - couldn't find line containing book to return in loans.txt


    /* --- Copy loans.txt to buffer.txt, without the loan data lines to delete --- */

    rewind(fp);

    FILE *fp_buffer;
    fp_buffer = fopen("buffer.txt", "w");
    if(fp_buffer == NULL) {
        printf("\n Error opening file \n");
        return 1;
    }

    line_count = 1;

    while(fscanf(fp, "%[^\n]\n", line) != -1) {

        if(line_count != line_nr_to_delete && line_count != line_nr_to_delete+1) {
            fprintf(fp_buffer, "%s\n", line);
        }

        line_count++;
    }

    fclose(fp);
    fclose(fp_buffer);

    // Make replace out-of-date loans.txt with the buffer file
    if(remove("loans.txt") != 0) { return 3; }
    rename("buffer.txt", "loans.txt");
    //add error check to rename
    //add 1 copy

    return 0;
}

int find_id_by_title (char title[]) {
    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(books[i].title, title)==0) return i;
    }

    return -1;
}


/* --- Librarian functions --- */