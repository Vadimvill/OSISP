//
// Created by vadim on 7.4.24.
//
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void addNode(Node* head,Node* node) {
    if (head == NULL) {
        head = node;
        node->next = NULL;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
        node->next = NULL;
    }
}

Node* getNode(Node * head,int i) {
    Node* current = head;
    int count = 0;
    while (current != NULL && count < i) {
        current = current->next;
        count++;
    }
    if (current != NULL) {
        return current;
    } else {
        return NULL;
    }
}
int pop(Node* head){
    Node* current = head;
    if(head->next == NULL){
        return -1;
    }
    while (current->next != NULL){
        current = current->next;
    }
    Node* preCurrent = head;
    while (preCurrent->next != current){
        preCurrent = preCurrent->next;
    }
    int res = current->pid;
    free(current);
    preCurrent->next = NULL;
    return res;
}

Node* createNode(pid_t pid){
    Node* node = malloc(sizeof (struct Node));
    node->next = NULL;
    node->pid = pid;
    return node;
}

void displayList(const Node* head) {
    const Node* current = head;
    printf("Linked List: ");
    while (current != NULL) {
        printf("%d ", current->pid);
        current = current->next;
    }
    printf("\n");
}

void clearList(Node* head) {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}
