#include <stdio.h>
#include <stdarg.h>

#define MAX_PHONES 5
#define MAX_EMAILS 5
#define MAX_STRING 50

typedef struct Contact {
    int id;
    char name[MAX_STRING];
    char surname[MAX_STRING];
    char patronymic[MAX_STRING];
    char phoneNumber[MAX_PHONES][MAX_STRING];
    char email[MAX_EMAILS][MAX_STRING];
    char job[MAX_STRING];
    int phone_count;
    int email_count;
    struct Contact* prev;
    struct Contact* next;
} Contact;

extern Contact* head;
extern Contact* tail;
extern int contactsCount;
extern int nextId;

int addContact(Contact contact);
int editContactField(int id, int field_choice, const char* new_value);
int removeContact(int id);
Contact* findContactById(int id);
Contact* getAllContacts(int* count);
void freePhoneBook(void);

void displayMenu(void);
void addContactConsole(void);
void editContactConsole(void);
void removeContactConsole(int count, ...);
void displayAllContacts(void);