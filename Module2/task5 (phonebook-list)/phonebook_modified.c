#include "phonebook_modified.h"
#include <stdlib.h>
#include <string.h>

Contact* head = NULL;
Contact* tail = NULL;
int contactsCount = 0;
int nextId = 1;

Contact* createContactNode(Contact contact) {
    Contact* new_contact = (Contact*)malloc(sizeof(Contact));
    if (!new_contact) return NULL;
    
    *new_contact = contact;
    new_contact->prev = NULL;
    new_contact->next = NULL;
    return new_contact;
}

void insertSorted(Contact* new_contact) {
    if (!head) {
        head = tail = new_contact;
        return;
    }
    
    Contact* current = head;
    while (current) {
        int cmp = strcmp(new_contact->surname, current->surname);
        if (cmp < 0 || (cmp == 0 && strcmp(new_contact->name, current->name) < 0)) {
            if (current == head) {
                new_contact->next = head;
                head->prev = new_contact;
                head = new_contact;
            } else {
                new_contact->prev = current->prev;
                new_contact->next = current;
                current->prev->next = new_contact;
                current->prev = new_contact;
            }
            return;
        }
        if (!current->next) break;
        current = current->next;
    }
    
    new_contact->prev = tail;
    tail->next = new_contact;
    tail = new_contact;
}

void removeFromList(Contact* contact) {
    if (contact->prev) {
        contact->prev->next = contact->next;
    } else {
        head = contact->next;
    }
    if (contact->next) {
        contact->next->prev = contact->prev;
    } else {
        tail = contact->prev;
    }
}

void reinsertContact(Contact* contact) {
    removeFromList(contact);
    contact->prev = NULL;
    contact->next = NULL;
    insertSorted(contact);
}

int addContact(Contact contact) {
    if (contactsCount >= MAX_PHONES) return -1;
    
    contact.id = nextId++;
    contact.phone_count = 1;
    contact.email_count = 1;
    
    Contact* new_contact = createContactNode(contact);
    if (!new_contact) return -1;
    
    insertSorted(new_contact);
    contactsCount++;
    return contact.id;
}

int editContactField(int id, int field_choice, const char* new_value) {
    Contact* contact = findContactById(id);
    if (!contact) return -1;
    
    char old_surname[MAX_STRING], old_name[MAX_STRING];
    strcpy(old_surname, contact->surname);
    strcpy(old_name, contact->name);
    
    switch (field_choice) {
        case 1:
            strncpy(contact->name, new_value, MAX_STRING - 1);
            break;
        case 2:
            strncpy(contact->surname, new_value, MAX_STRING - 1);
            break;
        case 3:
            strncpy(contact->patronymic, new_value, MAX_STRING - 1);
            break;
        case 4:
            strncpy(contact->phoneNumber[0], new_value, MAX_STRING - 1);
            break;
        case 5:
            strncpy(contact->email[0], new_value, MAX_STRING - 1);
            break;
        case 6:
            strncpy(contact->job, new_value, MAX_STRING - 1);
            break;
        default:
            return -1;
    }
    
    if (field_choice == 1 || field_choice == 2) {
        if (strcmp(old_surname, contact->surname) != 0 || 
            strcmp(old_name, contact->name) != 0) {
            reinsertContact(contact);
        }
    }
    
    return 0;
}

int removeContact(int id) {
    Contact* current = head;
    while (current) {
        if (current->id == id) {
            removeFromList(current);
            free(current);
            contactsCount--;
            return 0;
        }
        current = current->next;
    }
    return -1;
}

Contact* findContactById(int id) {
    Contact* current = head;
    while (current) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

Contact* getAllContacts(int* count) {
    *count = contactsCount;
    return head;
}

void freePhoneBook(void) {
    Contact* current = head;
    while (current) {
        Contact* next = current->next;
        free(current);
        current = next;
    }
    head = tail = NULL;
    contactsCount = 0;
}