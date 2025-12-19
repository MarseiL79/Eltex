#include "phonebook.h"
#include <stdlib.h>
#include <string.h>

ContactNode* root = NULL;
int contactsCount = 0;
int nextId = 1;
int operationsSinceBalance = 0;

int max(int a, int b) {
    return (a > b) ? a : b;
}

ContactNode* createNode(Contact contact) {
    ContactNode* node = (ContactNode*)malloc(sizeof(ContactNode));
    contact.id = nextId++;
    node->data = contact;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int getHeight(ContactNode* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(ContactNode* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

ContactNode* rightRotate(ContactNode* y) {
    ContactNode* x = y->left;
    ContactNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

ContactNode* leftRotate(ContactNode* x) {
    ContactNode* y = x->right;
    ContactNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

ContactNode* insertNode(ContactNode* node, Contact contact) {
    if (node == NULL)
        return createNode(contact);

    if (strcmp(contact.surname, node->data.surname) < 0)
        node->left = insertNode(node->left, contact);
    else if (strcmp(contact.surname, node->data.surname) > 0)
        node->right = insertNode(node->right, contact);
    else {
        if (strcmp(contact.name, node->data.name) < 0)
            node->left = insertNode(node->left, contact);
        else
            node->right = insertNode(node->right, contact);
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(contact.surname, node->left->data.surname) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(contact.surname, node->right->data.surname) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(contact.surname, node->left->data.surname) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(contact.surname, node->right->data.surname) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

ContactNode* minValueNode(ContactNode* node) {
    ContactNode* current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

ContactNode* deleteNode(ContactNode* root, int id) {
    if (root == NULL)
        return root;

    if (id < root->data.id)
        root->left = deleteNode(root->left, id);
    else if (id > root->data.id)
        root->right = deleteNode(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            ContactNode* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            ContactNode* temp = minValueNode(root->right);

            root->data = temp->data;

            root->right = deleteNode(root->right, temp->data.id);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void storeContactsInOrder(ContactNode* node, Contact* array, int* index) {
    if (node != NULL) {
        storeContactsInOrder(node->left, array, index);
        array[*index] = node->data;
        (*index)++;
        storeContactsInOrder(node->right, array, index);
    }
}

void freeTree(ContactNode* node) {
    if (node != NULL) {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

void balanceTreeIfNeeded() {
    if (operationsSinceBalance >= BALANCE_THRESHOLD) {
        if (contactsCount > 0) {
            Contact* contacts = (Contact*)malloc(contactsCount * sizeof(Contact));
            int index = 0;
            
            storeContactsInOrder(root, contacts, &index);
            
            freeTree(root);
            root = NULL;
            nextId = 1;
            
            for (int i = 0; i < contactsCount; i++) {
                contacts[i].id = nextId++;
                root = insertNode(root, contacts[i]);
            }
            
            free(contacts);
        }
        operationsSinceBalance = 0;
    }
}

ContactNode* findNodeById(ContactNode* node, int id) {
    if (node == NULL || node->data.id == id)
        return node;

    if (id < node->data.id)
        return findNodeById(node->left, id);

    return findNodeById(node->right, id);
}

int addContact(Contact contact) {
    if (contactsCount >= MAX_PHONES) {
        return -1;
    }
    
    root = insertNode(root, contact);
    contactsCount++;
    operationsSinceBalance++;
    
    balanceTreeIfNeeded();
    
    return nextId - 1;
}

int editContact(int id, Contact updatedContact) {
    ContactNode* node = findNodeById(root, id);
    if (node == NULL) {
        return -1;
    }
    
    updatedContact.id = id;
    node->data = updatedContact;
    operationsSinceBalance++;
    
    balanceTreeIfNeeded();
    
    return 0;
}

int removeContact(int id) {
    ContactNode* node = findNodeById(root, id);
    if (node == NULL) {
        return -1;
    }
    
    root = deleteNode(root, id);
    contactsCount--;
    operationsSinceBalance++;
    
    balanceTreeIfNeeded();
    
    return 0;
}

Contact* findContactById(int id) {
    ContactNode* node = findNodeById(root, id);
    if (node == NULL) {
        return NULL;
    }
    return &(node->data);
}

Contact* getAllContacts(int* count) {
    *count = contactsCount;
    if (contactsCount == 0) {
        return NULL;
    }
    
    Contact* contacts = (Contact*)malloc(contactsCount * sizeof(Contact));
    int index = 0;
    storeContactsInOrder(root, contacts, &index);
    
    return contacts;
}