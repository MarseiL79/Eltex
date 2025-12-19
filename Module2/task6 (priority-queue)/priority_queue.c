#include "priority_queue.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

QueueElement* head = NULL;
QueueElement* tail = NULL;
int elementsCount = 0;
int nextId = 1;

QueueElement* createElement(char* message, unsigned char priority) {
    QueueElement* new_element = (QueueElement*)malloc(sizeof(QueueElement));
    if (!new_element) return NULL;
    
    new_element->id = nextId++;
    strncpy(new_element->message, message, MAX_STRING - 1);
    new_element->message[MAX_STRING - 1] = '\0';
    new_element->priority = priority;
    new_element->prev = NULL;
    new_element->next = NULL;
    
    return new_element;
}

void insertByPriority(QueueElement* element) {
    if (!head) {
        head = tail = element;
        return;
    }
    
    QueueElement* current = head;
    
    while (current) {
        if (element->priority < current->priority) {
            if (current == head) {
                element->next = head;
                head->prev = element;
                head = element;
            } else {
                element->prev = current->prev;
                element->next = current;
                current->prev->next = element;
                current->prev = element;
            }
            return;
        }
        if (!current->next) break;
        current = current->next;
    }
    
    element->prev = tail;
    tail->next = element;
    tail = element;
}

int addElement(char* message, unsigned char priority) {
    if (priority > 255) return -1;
    
    QueueElement* new_element = createElement(message, priority);
    if (!new_element) return -1;
    
    insertByPriority(new_element);
    elementsCount++;
    return new_element->id;
}

int removeFirst() {
    if (!head) return -1;
    
    QueueElement* to_remove = head;
    head = head->next;
    if (head) {
        head->prev = NULL;
    } else {
        tail = NULL;
    }
    
    free(to_remove);
    elementsCount--;
    return 0;
}

int removeByPriority(unsigned char priority) {
    if (!head) return -1;
    
    QueueElement* current = head;
    while (current) {
        if (current->priority == priority) {
            if (current == head) {
                return removeFirst();
            }
            
            if (current->prev) {
                current->prev->next = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            }
            if (current == tail) {
                tail = current->prev;
            }
            
            free(current);
            elementsCount--;
            return 0;
        }
        current = current->next;
    }
    
    return -1;
}

int removeByPriorityOrHigher(unsigned char priority) {
    if (!head) return -1;
    
    QueueElement* current = head;
    int removed = 0;
    
    while (current) {
        QueueElement* next = current->next;
        if (current->priority <= priority) {
            if (current == head) {
                head = head->next;
                if (head) {
                    head->prev = NULL;
                } else {
                    tail = NULL;
                }
            } else {
                if (current->prev) {
                    current->prev->next = current->next;
                }
                if (current->next) {
                    current->next->prev = current->prev;
                }
                if (current == tail) {
                    tail = current->prev;
                }
            }
            
            free(current);
            elementsCount--;
            removed = 1;
        }
        current = next;
    }
    
    return removed ? 0 : -1;
}

QueueElement* getFirst() {
    return head;
}

QueueElement* getByPriority(unsigned char priority) {
    QueueElement* current = head;
    while (current) {
        if (current->priority == priority) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

QueueElement* getByPriorityOrHigher(unsigned char priority) {
    QueueElement* current = head;
    while (current) {
        if (current->priority <= priority) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

QueueElement* getAllElements(int* count) {
    *count = elementsCount;
    return head;
}

void freeQueue() {
    QueueElement* current = head;
    while (current) {
        QueueElement* next = current->next;
        free(current);
        current = next;
    }
    head = tail = NULL;
    elementsCount = 0;
    nextId = 1;
}