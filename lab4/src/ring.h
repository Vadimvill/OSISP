//
// Created by vadim on 1.5.24.
//

#ifndef LAB4_RING_H
#define LAB4_RING_H

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_MESSAGE_COUNT 20

typedef struct {
    u_int8_t type;
    u_int16_t hash;
    u_int8_t size;
    u_int8_t data[256];
} Message;

typedef struct Node {
    uintptr_t message;
    uintptr_t next;
    uintptr_t prev;
} Node;

typedef struct {
    int countAdded;
    int countDeleted;
    uintptr_t ringHead;
    uintptr_t ringTail;
    uintptr_t currentPlaceToWrite;
} Queue;

#define START_PLACE_TO_WRITE sizeof(Queue)
#define END_PLACE_TO_WRITE (sizeof(Queue)+MAX_MESSAGE_COUNT*sizeof(Node)+MAX_MESSAGE_COUNT*sizeof(Message))
#define STEP_TO_WRITE (sizeof(Node) + sizeof(Message))

void push(Queue*);
void pop(Queue*);

void initMessage(Message*);
void printMessage(Message*);

#endif //LAB4_RING_H
