
#ifndef LAB4_FUNC_H
#define LAB4_FUNC_H

#pragma once
#include "ring.h"
#include "stack.h"
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define FILL_SEM_NAME "/fill_sem"
#define EXTRACT_SEM_NAME "/extract_sem"
#define QUEUE_ACCESS_SEM_NAME "/queue_sem"
#define SHARED_MEMORY_NAME "/queue_messages"

extern int continuing;

void fillMessages();          // Заполнять.
void extractMessages();       // Извлекать.

void changeContinuingStatus();//


#endif //LAB4_FUNC_H
