#include <stdio.h>
#include <stdarg.h>

#define MAX_STRING 100

typedef struct QueueElement {
    int id;
    char message[MAX_STRING];
    unsigned char priority;
    struct QueueElement* prev;
    struct QueueElement* next;
} QueueElement;

extern QueueElement* head;
extern QueueElement* tail;
extern int elementsCount;
extern int nextId;

int addElement(char* message, unsigned char priority);
int removeFirst();
int removeByPriority(unsigned char priority);
int removeByPriorityOrHigher(unsigned char priority);
QueueElement* getFirst();
QueueElement* getByPriority(unsigned char priority);
QueueElement* getByPriorityOrHigher(unsigned char priority);
QueueElement* getAllElements(int* count);
void freeQueue();

void displayMenu();
void addElementConsole();
void removeElementConsole();
void showElementsConsole();
void simulateMessages();
void simulateRetrieval();