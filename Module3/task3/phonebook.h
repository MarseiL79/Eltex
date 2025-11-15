#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


#define MAX_PHONES 50
#define MAX_EMAILS 50
#define MAX_STRING 50
#define PHONEBOOK_FILE "phonebook.dat"

typedef struct {
    int id;
    char name[MAX_STRING];
    char surname[MAX_STRING];
    char patronymic[MAX_STRING];
    char phoneNumber[MAX_PHONES][MAX_STRING];
    char email[MAX_EMAILS][MAX_STRING];
    char job[MAX_STRING];
} Contact;

int addContact(Contact contact);
int editContact(int id, Contact updatedContact);
int removeContact(int id);
Contact* findContactById(int id);
Contact* getAllContacts(int* count);

int saveContacts();
int loadContacts();

void displayMenu(void);
void addContactConsole(void);
void editContactConsole(void);
void removeContactConsole(int count, ...);
void displayAllContacts(void);