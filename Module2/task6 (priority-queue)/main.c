#include "priority_queue.h"
#include <stdlib.h>
#include <time.h>

void displayMenu() {
    printf("\n=== PRIORITY QUEUE ===\n");
    printf("1. Add element\n");
    printf("2. Remove first element\n");
    printf("3. Show all elements\n");
    printf("4. Simulate message generation\n");
    printf("5. Simulate element retrieval\n");
    printf("6. Exit\n");
    printf("Choose action: ");
}

void addElementConsole() {
    char message[MAX_STRING];
    unsigned char priority;
    
    printf("\n--- Add element ---\n");
    
    printf("Message (max %d chars): ", MAX_STRING - 1);
    scanf(" %[^\n]", message);
    
    printf("Priority (0-255): ");
    scanf("%hhu", &priority);
    
    if (priority > 255) {
        printf("Priority must be between 0 and 255\n");
        return;
    }
    
    int id = addElement(message, priority);
    if (id > 0) {
        printf("Element added with ID: %d\n", id);
    } else {
        printf("Error: impossible to add element\n");
    }
}

void removeElementConsole() {
    int choice;
    unsigned char priority;
    
    printf("\n--- Remove element ---\n");
    printf("1. Remove first element\n");
    printf("2. Remove by exact priority\n");
    printf("3. Remove by priority or higher\n");
    printf("Choose option: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            if (removeFirst() == 0) {
                printf("First element removed successfully\n");
            } else {
                printf("Queue is empty\n");
            }
            break;
        case 2:
            printf("Enter priority to remove: ");
            scanf("%hhu", &priority);
            if (removeByPriority(priority) == 0) {
                printf("Element with priority %d removed\n", priority);
            } else {
                printf("No element with priority %d found\n", priority);
            }
            break;
        case 3:
            printf("Enter priority (or higher): ");
            scanf("%hhu", &priority);
            if (removeByPriorityOrHigher(priority) == 0) {
                printf("Elements with priority <= %d removed\n", priority);
            } else {
                printf("No elements with priority <= %d found\n", priority);
            }
            break;
        default:
            printf("Invalid choice\n");
    }
}

void showElementsConsole() {
    int count;
    QueueElement* elements = getAllElements(&count);
    
    printf("\n--- All elements (%d) ---\n", count);
    
    if (count == 0) {
        printf("Queue is empty\n");
        return;
    }
    
    QueueElement* current = elements;
    while (current) {
        printf("ID: %d | Priority: %d | Message: %s\n", 
               current->id, current->priority, current->message);
        current = current->next;
    }
}

void simulateMessages() {
    srand(time(NULL));
    
    printf("\n--- Simulating message generation ---\n");
    
    char* messages[] = {
        "System startup",
        "User login detected",
        "Disk space low",
        "Network connection lost",
        "Backup completed",
        "Error in module A",
        "Warning: temperature high",
        "Database connection OK",
        "Security alert",
        "Scheduled task completed"
    };
    
    int num_messages = sizeof(messages) / sizeof(messages[0]);
    int messages_to_generate = 5 + rand() % 6;
    
    printf("Generating %d random messages...\n", messages_to_generate);
    
    for (int i = 0; i < messages_to_generate; i++) {
        int msg_index = rand() % num_messages;
        unsigned char priority = rand() % 256;
        
        int id = addElement(messages[msg_index], priority);
        printf("  Added: Priority=%d, Message='%s' (ID: %d)\n", 
               priority, messages[msg_index], id);
    }
    
    printf("Generation complete\n");
}

void simulateRetrieval() {
    srand(time(NULL));
    
    printf("\n--- Simulating element retrieval ---\n");
    
    int count;
    getAllElements(&count);
    
    if (count == 0) {
        printf("Queue is empty, nothing to retrieve\n");
        return;
    }
    
    int method = 1 + rand() % 3;
    
    switch (method) {
        case 1: {
            QueueElement* elem = getFirst();
            if (elem) {
                printf("Retrieving first element:\n");
                printf("  ID: %d, Priority: %d, Message: %s\n", 
                       elem->id, elem->priority, elem->message);
            }
            break;
        }
        case 2: {
            unsigned char priority = rand() % 256;
            QueueElement* elem = getByPriority(priority);
            printf("Looking for element with priority %d:\n", priority);
            if (elem) {
                printf("  Found: ID: %d, Message: %s\n", 
                       elem->id, elem->message);
            } else {
                printf("  No element with priority %d found\n", priority);
            }
            break;
        }
        case 3: {
            unsigned char priority = rand() % 256;
            QueueElement* elem = getByPriorityOrHigher(priority);
            printf("Looking for element with priority <= %d:\n", priority);
            if (elem) {
                printf("  Found: ID: %d, Priority: %d, Message: %s\n", 
                       elem->id, elem->priority, elem->message);
            } else {
                printf("  No element with priority <= %d found\n", priority);
            }
            break;
        }
    }
}

int main() {
    int choice;
    
    srand(time(NULL));
    
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                addElementConsole();
                break;
            case 2:
                removeElementConsole();
                break;
            case 3:
                showElementsConsole();
                break;
            case 4:
                simulateMessages();
                break;
            case 5:
                simulateRetrieval();
                break;
            case 6:
                printf("Exiting...\n");
                freeQueue();
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}