#include "book_management.h"
#include "books_additional.h"
#include "user_management.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define max_books 200

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

struct Book books_array[10]; // For search

struct BookArray find_book_by_title (const char *title) {
    int nr_books_found = 0;
    struct BookArray books_found;
    books_found.array = &books_array[0];

    for(int i=0; i<nr_of_books; i++) {
        if(strstr(books[i].title, title) != NULL && nr_books_found <= 10) {
            books_array[nr_books_found] = books[i];
            nr_books_found += 1;
        }
    }
    books_found.length = nr_books_found;
    return books_found;
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
        printf("Title: %s\n", books_array[i].title);
        printf("Author(s): %s\n", books_array[i].authors);
        printf("Year published: %d\n", books_array[i].year);
        printf("Copies available: %d\n", books_array[i].copies);
    }

    printf("\nWould you like to borrow one of these books? (y/n) ");
    char answer;
    answer = getchar();
    if(answer == 'y') {
        printf("\nWhich of the results would you like to borrow? Please type the number (without #): ");
        int result_nr;
        scanf("%d", &result_nr);
        //result nr
        if(result_nr > 0 && result_nr <= found_books.length) {
            borrow(books_array[result_nr-1], current_user_id);
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

    fprintf(fp, "%s, %d\n", book.title, user_id);

    printf("\n\n --- You have borrowed \"%s\" ---\n", book.title);
    
    fclose(fp);
    return 0;
}

int find_id_by_title (char title[]) {
    for(int i=0; i<nr_of_books; i++) {
        if(strcmp(books[i].title, title)==0) return i;
    }

    return -1;
}


/* --- Librarian functions --- */