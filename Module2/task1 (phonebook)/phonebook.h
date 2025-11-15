#include <stdio.h>
#include <stdarg.h>

#define MAX_PHONES 50
#define MAX_EMAILS 50
#define MAX_STRING 50

typedef struct {
    int id;
    char name[MAX_STRING];
    char surname[MAX_STRING];
    char patronymic[MAX_STRING];
    char phoneNumber[MAX_PHONES][MAX_STRING];
    char email[MAX_EMAILS][MAX_STRING];
    char job[MAX_STRING];
} Contact;

// Объявления функций (только заголовки)
int addContact(Contact contact);
int editContact(int id, Contact updatedContact);
int removeContact(int id);
Contact* findContactById(int id);
Contact* getAllContacts(int* count);

// GUI функции
void displayMenu(void);
void addContactConsole(void);
void editContactConsole(void);
void removeContactConsole(int count, ...);
void displayAllContacts(void);