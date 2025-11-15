#include "phonebook_modified.h"
#include <stdlib.h>

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
    int id, field_choice;
    char new_value[MAX_STRING];
    
    printf("\n--- Contact editing ---\n");
    printf("Enter contact ID to edit: ");
    scanf("%d", &id);
    
    Contact* existing = findContactById(id);
    if (existing == NULL) {
        printf("Contact with ID %d not found\n", id);
        return;
    }
    
    printf("\nChoose field to edit:\n");
    printf("1. Name (%s)\n", existing->name);
    printf("2. Surname (%s)\n", existing->surname);
    printf("3. Patronymic (%s)\n", existing->patronymic);
    printf("4. Phone (%s)\n", existing->phoneNumber[0]);
    printf("5. Email (%s)\n", existing->email[0]);
    printf("6. Job (%s)\n", existing->job);
    printf("Choose field (1-6): ");
    scanf("%d", &field_choice);
    
    if (field_choice < 1 || field_choice > 6) {
        printf("Invalid field choice!\n");
        return;
    }
    
    printf("Enter new value: ");
    scanf("%49s", new_value);
    
    if (editContactField(id, field_choice, new_value) == 0) {
        printf("Contact field updated successfully\n");
    } else {
        printf("Error while updating contact\n");
    }
}

void removeContactConsole(int count, ...) {
    int id;
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        id = va_arg(args, int);
        printf("\n--- Removing contact ID %d ---\n", id);
        
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
    
    Contact* current = contacts;
    while (current) {
        printf("ID: %d\n", current->id);
        printf("  Name: %s %s %s\n", current->surname, current->name, current->patronymic);
        printf("  Phone: %s\n", current->phoneNumber[0]);
        printf("  Email: %s\n", current->email[0]);
        printf("  Job: %s\n", current->job);
        printf("  ---\n");
        current = current->next;
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
            case 3: {
                int num = 0;
                int id_array[3];
                printf("Number of contacts to remove (MAXIMUM 3): ");
                scanf("%d", &num);
                if (num < 1 || num > 3) {
                    printf("Invalid number!\n");
                    break;
                }
                for (int i = 0; i < num; i++) {
                    printf("Enter %d contact's ID to remove: ", i + 1);
                    scanf("%d", &id_array[i]);
                }
                
                switch (num) {
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
            }
            case 4:
                displayAllContacts();
                break;
            case 5:
                printf("Exit...\n");
                freePhoneBook();
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}