#ifndef LIST_H
#define LIST_H

#include <sys/types.h>

typedef struct Node {
    pid_t pid;
    struct Node* next;
} Node;

Node* createNode(pid_t pid);
void addNode(Node* head,Node* node);
Node* getNode(Node* head,int i);
void displayList(const Node*);
int pop(Node* head);
void clearList(Node*);

#endif //LIST_H