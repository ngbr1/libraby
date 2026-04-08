#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 9999
#define MAX_USERS 1000
#define MAX_TRANS 5000

struct Book {
    int id;
    char title[100];
    char author[100];
    int year;
    int quantity;
    int available;
    int borrow_count; 
};

struct User {
    int id;
    char name[100];
    int borrowed_count;
};

struct Transaction {
    int user_id;
    int book_id;
    char date[20];
    int type; 
};

struct Book books[MAX_BOOKS];
int book_count = 0;

struct User users[MAX_USERS];
int user_count = 0;

struct Transaction transactions[MAX_TRANS];
int trans_count = 0;

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInt() {
    int value;
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Please enter a number: ");
        clearInputBuffer();
    }
    return value;
}

void sanitizeString(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '|') {
            str[i] = '-';
        }
    }
}

char* getCurrentDate() {
    static char date[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    return date;
}

int findUserIndex(int id) {
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == id) return i;
    }
    return -1;
}

int findBookIndex(int id) {
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) return i;
    }
    return -1;
}

void saveData() {
    FILE *fb = fopen("books.txt", "w");
    if (fb) {
        for (int i = 0; i < book_count; i++) {
            fprintf(fb, "%d|%s|%s|%d|%d|%d|%d\n", books[i].id, books[i].title, books[i].author, books[i].year, books[i].quantity, books[i].available, books[i].borrow_count);
        }
        fclose(fb);
    }

    FILE *fu = fopen("users.txt", "w");
    if (fu) {
        for (int i = 0; i < user_count; i++) {
            fprintf(fu, "%d|%s|%d\n", users[i].id, users[i].name, users[i].borrowed_count);
        }
        fclose(fu);
    }

    FILE *ft = fopen("transactions.txt", "w");
    if (ft) {
        for (int i = 0; i < trans_count; i++) {
            fprintf(ft, "%d|%d|%s|%d\n", transactions[i].user_id, transactions[i].book_id, transactions[i].date, transactions[i].type);
        }
        fclose(ft);
    }
}

void addPresetBook(int id, const char* title, const char* author, int year, int quantity) {
    if (book_count < MAX_BOOKS) {
        books[book_count].id = id;
        strncpy(books[book_count].title, title, 99);
        books[book_count].title[99] = '\0';
        strncpy(books[book_count].author, author, 99);
        books[book_count].author[99] = '\0';
        books[book_count].year = year;
        books[book_count].quantity = quantity;
        books[book_count].available = quantity;
        books[book_count].borrow_count = 0;
        book_count++;
    }
}

void loadData() {
    FILE *fb = fopen("books.txt", "r");
    if (fb) {
        while (fscanf(fb, "%d|%99[^|]|%99[^|]|%d|%d|%d|%d\n", &books[book_count].id, books[book_count].title, books[book_count].author, &books[book_count].year, &books[book_count].quantity, &books[book_count].available, &books[book_count].borrow_count) == 7) {
            if (book_count < MAX_BOOKS - 1) book_count++;
            else break;
        }
        fclose(fb);
    } else {
        addPresetBook(154, "cheeseburger", "beeangoodshape", 2021, 1);
        addPresetBook(153, "that one infinite game", "beeangoodshape", 2025, 1000);
        addPresetBook(152, "hes in your wall", "beeangoodshape", 2023, 4);
        addPresetBook(7922, "doyouknow", "themuffinman", 1820, 4);
        addPresetBook(7923, "wholivesin", "drurylane", 1889, 4);
        addPresetBook(333, "tellmewhy", "aint nothing", 1999, 12);
        addPresetBook(42, "buta", "hearthache", 1999, 4);
        
    }

    FILE *fu = fopen("users.txt", "r");
    if (fu) {
        while (fscanf(fu, "%d|%99[^|]|%d\n", &users[user_count].id, users[user_count].name, &users[user_count].borrowed_count) == 3) {
            if (user_count < MAX_USERS - 1) user_count++;
            else break;
        }
        fclose(fu);
    }

    FILE *ft = fopen("transactions.txt", "r");
    if (ft) {
        while (fscanf(ft, "%d|%d|%19[^|]|%d\n", &transactions[trans_count].user_id, &transactions[trans_count].book_id, transactions[trans_count].date, &transactions[trans_count].type) == 4) {
            if (trans_count < MAX_TRANS - 1) trans_count++;
            else break;
        }
        fclose(ft);
    }
}

void showMenu() {
    printf("\n⠀⣀⣤⣤⣤⣀⠀⠀⠀⠀⣀⣤⣤⣤⣀⠀      ========== LIBRARY SYSTEM ==========\n");
    printf("⣼⣿⣿⣿⣿⣿⣷⡆⢰⣾⣿⣿⣿⣿⣿⣷      1. Add new book\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿      2. Display book list\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿      3. Search book\n");
    printf("⣿⣿⣿⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⣿      4. Borrow book\n");
    printf("⠛⠛⠛⠿⠿⣿⣿⡇⢸⣿⣿⠿⠿⠛⠛⠛      5. Return book\n");
    printf("⠙⠓⠒⠶⠦⣄⡉⡃⢘⢉⣠⠴⠶⠒⠚⠋      6. Add User\n");
    printf("      ⠀⠁⠈⠀⠀⠀⠀         7. Display Users\n");
    printf("                      8. View Transactions\n");
    printf("                      9. Statistics\n");
    printf("                      0. Exit & Save\n");
    printf("⠀⠀⠀⠀⠀⠀⠀         ⠀⠀⠀      Choose: ");
}

void addBook() {
    if (book_count >= MAX_BOOKS) {
        printf("Error: Library database is full (Max %d books).\n", MAX_BOOKS);
        return;
    }

    struct Book b;
    printf("Enter book ID: ");
    b.id = getValidInt();
    
    if (findBookIndex(b.id) != -1) {
        printf("Error: Book with ID %d already exists.\n", b.id);
        return;
    }
    if (b.id <= 0) {
        printf("Error: Book ID must be a positive number.\n");
        return;
    }

    printf("Enter title: ");
    scanf(" %99[^\n]", b.title);
    sanitizeString(b.title);

    printf("Enter author: ");
    scanf(" %99[^\n]", b.author);
    sanitizeString(b.author);

    printf("Enter year: ");
    b.year = getValidInt();

    printf("Enter quantity: ");
    b.quantity = getValidInt();
    if (b.quantity <= 0) {
        printf("Error: Quantity must be greater than 0.\n");
        return;
    }
    
    b.available = b.quantity;
    b.borrow_count = 0;
    books[book_count++] = b;
    printf("Book added successfully!\n");
}

void displayBooks() {
    if (book_count == 0) {
        printf("\nThe library is currently empty!\n");
        return;
    }

    int sort_choice;
    printf("\n--- Display Options ---\n");
    printf("1. Sort by ID\n");
    printf("2. Sort by Title (Alphabetical)\n");
    printf("3. Sort by Year (Newest to Oldest)\n");
    printf("4. Sort by Year (Oldest to Newest)\n");
    printf("5. Sort by Available Quantity\n");
    printf("Choose how to view the list: ");
    sort_choice = getValidInt();

    for (int i = 0; i < book_count - 1; i++) {
        for (int j = 0; j < book_count - i - 1; j++) {
            int should_swap = 0;
            
            if (sort_choice == 1 && books[j].id > books[j+1].id) should_swap = 1;
            else if (sort_choice == 2 && strcmp(books[j].title, books[j+1].title) > 0) should_swap = 1;
            else if (sort_choice == 3 && books[j].year < books[j+1].year) should_swap = 1;
            else if (sort_choice == 4 && books[j].year > books[j+1].year) should_swap = 1;
            else if (sort_choice == 5 && books[j].available < books[j+1].available) should_swap = 1;

            if (should_swap == 1) {
                struct Book temp = books[j];
                books[j] = books[j+1];
                books[j+1] = temp;
            }
        }
    }

    printf("\nID\t%-25s\t%-15s\tYear\tAvail/Qty\n", "Title", "Author");
    printf("------------------------------------------------------------------------------\n");
    for(int i = 0; i < book_count; i++) {
        printf("%d\t%-25.25s\t%-15.15s\t%d\t%d/%d\n",
            books[i].id, books[i].title, books[i].author, 
            books[i].year, books[i].available, books[i].quantity);
    }
}

void searchBook() {
    int choice;
    printf("\n--- Advanced Search ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Title\n");
    printf("3. Search by Author\n");
    printf("4. Search by Year\n");
    printf("Choose option: ");
    choice = getValidInt();

    int found = 0;
    
    if (choice == 1 || choice == 4) {
        int search_num;
        printf("Enter ID/Year: ");
        search_num = getValidInt();
        for (int i = 0; i < book_count; i++) {
            if ((choice == 1 && books[i].id == search_num) || (choice == 4 && books[i].year == search_num)) {
                printf("\nFound: [%d] '%s' by %s (%d) - %d/%d available\n", books[i].id, books[i].title, books[i].author, books[i].year, books[i].available, books[i].quantity);
                found = 1;
            }
        }
    } else if (choice == 2 || choice == 3) {
        char search_str[100];
        printf("Enter Title/Author to search: ");
        scanf(" %99[^\n]", search_str);
        for (int i = 0; i < book_count; i++) {
            if ((choice == 2 && strstr(books[i].title, search_str) != NULL) || 
                (choice == 3 && strstr(books[i].author, search_str) != NULL)) {
                printf("\nFound: [%d] '%s' by %s (%d) - %d/%d available\n", books[i].id, books[i].title, books[i].author, books[i].year, books[i].available, books[i].quantity);
                found = 1;
            }
        }
    } else {
        printf("Invalid search option.\n");
        return;
    }

    if (found == 0) printf("\nNo books matched your search.\n");
}

void borrowBook() {
    if (trans_count >= MAX_TRANS) {
        printf("Error: Transaction limit reached. Contact Admin.\n");
        return;
    }

    int user_id, book_id;
    printf("Enter User ID: ");
    user_id = getValidInt();
    
    int u_idx = findUserIndex(user_id);
    if (u_idx == -1) {
        printf("Error: User ID not found.\n");
        return;
    }

    printf("Enter Book ID to borrow: ");
    book_id = getValidInt();

    int b_idx = findBookIndex(book_id);
    if (b_idx == -1) {
        printf("Error: Book ID not found.\n");
        return;
    }

    if (books[b_idx].available > 0) {
        books[b_idx].available--;
        books[b_idx].borrow_count++;
        users[u_idx].borrowed_count++;
        
        transactions[trans_count].user_id = user_id;
        transactions[trans_count].book_id = book_id;
        strcpy(transactions[trans_count].date, getCurrentDate());
        transactions[trans_count].type = 1;
        trans_count++;

        printf("Successfully borrowed '%s'.\n", books[b_idx].title);
    } else {
        printf("Sorry, '%s' is currently out of stock.\n", books[b_idx].title);
    }
}

void returnBook() {
    if (trans_count >= MAX_TRANS) {
        printf("Error: Transaction limit reached. Contact Admin.\n");
        return;
    }

    int user_id, book_id;
    printf("Enter User ID: ");
    user_id = getValidInt();
    
    int u_idx = findUserIndex(user_id);
    if (u_idx == -1) {
        printf("Error: User ID not found.\n");
        return;
    }

    printf("Enter Book ID to return: ");
    book_id = getValidInt();

    int b_idx = findBookIndex(book_id);
    if (b_idx == -1) {
        printf("Error: Book ID not found.\n");
        return;
    }

    if (books[b_idx].available < books[b_idx].quantity) {
        books[b_idx].available++;
        if(users[u_idx].borrowed_count > 0) users[u_idx].borrowed_count--;

        transactions[trans_count].user_id = user_id;
        transactions[trans_count].book_id = book_id;
        strcpy(transactions[trans_count].date, getCurrentDate());
        transactions[trans_count].type = 2;
        trans_count++;

        printf("Successfully returned '%s'.\n", books[b_idx].title);
    } else {
        printf("Error: All copies of '%s' are already in the library.\n", books[b_idx].title);
    }
}

void addUser() {
    if (user_count >= MAX_USERS) {
        printf("Error: User database is full (Max %d users).\n", MAX_USERS);
        return;
    }

    struct User u;
    
    int max_id = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].id > max_id) {
            max_id = users[i].id;
        }
    }
    
    u.id = max_id + 1; 

    printf("Auto-assigned User ID: %05d\n", u.id);
    
    printf("Enter User Name: ");
    scanf(" %99[^\n]", u.name);
    sanitizeString(u.name);
    u.borrowed_count = 0;
    
    users[user_count++] = u;
    printf("User '%s' (ID: %05d) added successfully!\n", u.name, u.id);
}

void displayUsers() {
    if (user_count == 0) {
        printf("\nNo users registered.\n");
        return;
    }
    printf("\nID\t\tName\t\t\tBorrowed\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < user_count; i++) {
        printf("%05d\t\t%-20.20s\t%d\n", users[i].id, users[i].name, users[i].borrowed_count);
    }
}

void viewTransactions() {
    if (trans_count == 0) {
        printf("\nNo transactions recorded yet.\n");
        return;
    }
    printf("\nDate\t\tUser ID\tBook ID\tType\n");
    printf("-------------------------------------------------------------------\n");
    for (int i = 0; i < trans_count; i++) {
        printf("%s\t%05d\t%d\t%s\n", 
            transactions[i].date, 
            transactions[i].user_id, 
            transactions[i].book_id, 
            transactions[i].type == 1 ? "BORROW" : "RETURN");
    }
}

void showStatistics() {
    int total_books = 0;
    int total_borrowed = 0;
    int most_borrowed_idx = -1;
    int max_borrows = -1;

    for (int i = 0; i < book_count; i++) {
        total_books += books[i].quantity;
        total_borrowed += (books[i].quantity - books[i].available);
        
        if (books[i].borrow_count > max_borrows) {
            max_borrows = books[i].borrow_count;
            most_borrowed_idx = i;
        }
    }

    printf("\n--- Library Statistics ---\n");
    printf("Total distinct titles: %d\n", book_count);
    printf("Total books in system: %d\n", total_books);
    printf("Total books currently borrowed: %d\n", total_borrowed);
    
    if (most_borrowed_idx != -1 && max_borrows > 0) {
        printf("Most frequently borrowed book: '%s' (%d times)\n", 
            books[most_borrowed_idx].title, max_borrows);
    }
}

int main() {
    loadData();
    int choice;
    while(1) {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid choice! Please enter a number.\n");
            continue;
        }
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: addUser(); break;
            case 7: displayUsers(); break;
            case 8: viewTransactions(); break;
            case 9: showStatistics(); break;
            case 0:
                saveData();
                printf("Data saved. Exiting Library System. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}