#include "phonebook.h"

void displayMenu() {
    printf("\n=== PHONE BOOK ===\n");
    printf("1. Add contact\n");
    printf("2. Edit contact\n");
    printf("3. Remove contact\n");
    printf("4. Show all contacts\n");
    printf("5. Exit\n");
    printf("Choose action: ");
}

void addContactConsole() {
    Contact newContact = {0};
    
    printf("\n--- Contact adding ---\n");
    
    printf("Name: ");
    scanf("%49s", newContact.name);
    
    printf("Surname: ");
    scanf("%49s", newContact.surname);
    
    printf("Patronymic: ");
    scanf("%49s", newContact.patronymic);
    
    printf("Phone number: ");
    scanf("%49s", newContact.phoneNumber[0]);
    
    printf("Email: ");
    scanf("%49s", newContact.email[0]);
    
    printf("Job: ");
    scanf("%49s", newContact.job);
    
    int id = addContact(newContact);
    if (id > 0) {
        printf("Contact added with ID: %d\n", id);
    } else {
        printf("Error: impossible to add contact\n");
    }
}

void editContactConsole() {
    int id;
    printf("\n--- Contact editing ---\n");
    printf("Enter contact's id to edit: ");
    scanf("%d", &id);
    
    Contact* existing = findContactById(id);
    if (existing == NULL) {
        printf("Contact with ID %d not found\n", id);
        return;
    }
    
    Contact updatedContact = *existing;
    
    printf("New name [%s]: ", existing->name);
    scanf("%49s", updatedContact.name);
    
    printf("New surname [%s]: ", existing->surname);
    scanf("%49s", updatedContact.surname);
    
    printf("New patronymic [%s]: ", existing->patronymic);
    scanf("%49s", updatedContact.patronymic);
    
    printf("New phone number [%s]: ", existing->phoneNumber[0]);
    scanf("%49s", updatedContact.phoneNumber[0]);
    
    printf("New email [%s]: ", existing->email[0]);
    scanf("%49s", updatedContact.email[0]);
    
    printf("New Job [%s]: ", existing->job);
    scanf("%49s", updatedContact.job);
    
    if (editContact(id, updatedContact) == 0) {
        printf("Contact updated successfully\n");
    } else {
        printf("Error while updating contact\n");
    }
}

void removeContactConsole(int count, ...) {
    int id;
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++)
    {
        id = va_arg(args, int);
        printf("\n--- Removing contact ---\n");
        
        if (removeContact(id) == 0) {
            printf("Contact removed successfully\n");
        } else {
            printf("Contact with ID %d not found\n", id);
        }
    }
    va_end(args);
}

void displayAllContacts() {
    int count;
    Contact* contacts = getAllContacts(&count);
    
    printf("\n--- All contacts (%d) ---\n", count);
    
    if (count == 0) {
        printf("No contacts\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", contacts[i].id);
        printf("  Name: %s %s %s\n", contacts[i].surname, contacts[i].name, contacts[i].patronymic);
        printf("  Phone Number: %s\n", contacts[i].phoneNumber[0]);
        printf("  Email: %s\n", contacts[i].email[0]);
        printf("  Job: %s\n", contacts[i].job);
        printf("  ---\n");
    }
}


int main() {
    int choice;
    
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addContactConsole();
                break;
            case 2:
                editContactConsole();
                break;
            case 3:
                int num = 0;
                int id_array[3];
                printf("Number of contacts to remove (MAXIMUM 3): ");
                scanf("%d", &num);
                for (int i = 0; i < num; i++)
                {
                    printf("Enter %d contact's ID to remove: ", i+1);
                    scanf("%d", &id_array[i]);
                }
                
                switch (num)
                {
                case 1:
                    removeContactConsole(num, id_array[0]);
                    break;
                
                case 2:
                    removeContactConsole(num, id_array[0], id_array[1]);
                    break;
                case 3:
                    removeContactConsole(num, id_array[0], id_array[1], id_array[2]);
                    break;
                }
                break;

            case 4:
                displayAllContacts();
                break;
            case 5:
                printf("Exit...\n");
                break;
            default:
                printf("Uncorrect choice!\n");
        }
    } while (choice != 5);

    return 0;
}