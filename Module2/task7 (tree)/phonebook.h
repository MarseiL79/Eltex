#include <stdio.h>
#include <stdarg.h>

#define MAX_PHONES 5
#define MAX_EMAILS 5
#define MAX_STRING 50
#define BALANCE_THRESHOLD 5

typedef struct ContactNode {
    Contact data;
    struct ContactNode* left;
    struct ContactNode* right;
    int height;
} ContactNode;

typedef struct {
    int id;
    char name[MAX_STRING];
    char surname[MAX_STRING];
    char patronymic[MAX_STRING];
    char phoneNumber[MAX_PHONES][MAX_STRING];
    char email[MAX_EMAILS][MAX_STRING];
    char job[MAX_STRING];
} Contact;

extern ContactNode* root;
extern int contactsCount;
extern int nextId;
extern int operationsSinceBalance;

ContactNode* createNode(Contact contact);
int getHeight(ContactNode* node);
int getBalance(ContactNode* node);
ContactNode* rightRotate(ContactNode* y);
ContactNode* leftRotate(ContactNode* x);
ContactNode* insertNode(ContactNode* node, Contact contact);
ContactNode* minValueNode(ContactNode* node);
ContactNode* deleteNode(ContactNode* root, int id);
void storeContactsInOrder(ContactNode* node, Contact* array, int* index);
void freeTree(ContactNode* node);
void balanceTreeIfNeeded();
ContactNode* findNodeById(ContactNode* node, int id);

int addContact(Contact contact);
int editContact(int id, Contact updatedContact);
int removeContact(int id);
Contact* findContactById(int id);
Contact* getAllContacts(int* count);

void displayMenu(void);
void addContactConsole(void);
void editContactConsole(void);
void removeContactConsole(int count, ...);
void displayAllContacts(void);