#include "phonebook.h"

Contact phoneBook[MAX_PHONES];
int contactsCount = 0;
int nextId = 1;

int addContact(Contact contact) {
    if (contactsCount >= MAX_PHONES) {
        return -1; 
    }
    
    contact.id = nextId++;
    phoneBook[contactsCount] = contact;
    contactsCount++;
    
    return contact.id;
}

int editContact(int id, Contact updatedContact) {
    for (int i = 0; i < contactsCount; i++) {
        if (phoneBook[i].id == id) {
            updatedContact.id = id; // Сохраняем оригинальный ID
            phoneBook[i] = updatedContact;
            return 0; 
        }
    }
    return -1; 
}

int removeContact(int id) {
    for (int i = 0; i < contactsCount; i++)
    {
        if(phoneBook[i].id == id) {
            for (int j = i; j < contactsCount - 1; j++)
            {
                phoneBook[j] = phoneBook[j+1];
            }
            contactsCount--;
            return 0;
        }
    }
    return -1;
}

Contact* findContactById(int id) {
    for (int i = 0; i < contactsCount; i++) {
        if (phoneBook[i].id == id) {
            return &phoneBook[i];
        }
    }
    return NULL; // Не найден
}

Contact* getAllContacts(int* count) {
    *count = contactsCount;
    return phoneBook;
}


int saveContacts() {
    int fd = open(PHONEBOOK_FILE, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
    if (fd == -1) {
        return -1;
    }

    write(fd, &contactsCount, sizeof(int));
    write(fd, &nextId, sizeof(int));
    
    if(contactsCount > 0) {
        write(fd, phoneBook, sizeof(Contact) * contactsCount);
    }

    fsync(fd);
    close(fd);
    return 0;
}

int loadContacts() {
    int fd = open(PHONEBOOK_FILE, O_RDONLY | O_CREAT, 0644);
    if (fd == -1) {
        return -1;
    }

    ssize_t bytes = read(fd, &contactsCount, sizeof(int));
    if (bytes != sizeof(int)) {
        contactsCount = 0;
    }

    read(fd, &nextId, sizeof(int));

    if (contactsCount > 0) {
        bytes = read(fd, phoneBook, sizeof(Contact) * contactsCount);
        if (bytes != sizeof(Contact) * contactsCount) {
            contactsCount = 0;
            nextId = 1;
        }
    }

    close(fd);
    return 0;
}