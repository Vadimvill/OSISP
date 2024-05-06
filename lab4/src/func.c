#include "func.h"

void fillMessages() {
    int shmFd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);     // Открыть совместно используемую память.
    if(shmFd == -1) {
        printf("Error while opening shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Отобразить её.
    Queue* queue = mmap(NULL, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Открыть семафоры.
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* fillSem = sem_open(FILL_SEM_NAME, 0);
    if(fillSem == SEM_FAILED) {
        printf("Error while opening filling semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while (continuing) {
        sem_wait(queueAccess);   // Ожидание доступа к памяти и заполнению.
        sem_wait(fillSem);

        if (queue->countAdded - queue->countDeleted < MAX_MESSAGE_COUNT) {    // Добавить, если есть место.
            push(queue);
            queue->countAdded++;
            printf("Added %d message:\n", queue->countAdded);
            printMessage((Message*)(((Node*)(queue->ringTail + (uintptr_t)queue))->message));
        } else
            printf("Queue is full!\n");

        sem_post(fillSem);   // Освободить семафоры.
        sem_post(queueAccess);
        sleep(3);
    }

    // Отсоединить память.
    munmap(queue, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message));
    close(shmFd);

    // Закрыть семафоры.
    sem_close(queueAccess);
    sem_close(fillSem);
}

void extractMessages() {
    int shmFd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0666);     // Открыть совместо используемую память.
    if(shmFd == -1) {
        printf("Error while opening shared memory. Code: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    // Отобразить совместо используемую память.
    Queue* queue = mmap(NULL, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0);
    if(queue == MAP_FAILED) {
        printf("Error while mapping shared memory.\n");
        exit(EXIT_FAILURE);
    }

    // Открыть семафоры.
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* extractSem = sem_open(EXTRACT_SEM_NAME, 0);
    if(extractSem == SEM_FAILED) {
        printf("Error while opening extracting semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while(continuing) {
        sem_wait(queueAccess);   // Ожидание доступа к памяти и чтению.
        sem_wait(extractSem);

        if(queue->countAdded - queue->countDeleted > 0) {    // Чтение, если есть что читать.
            printf("Delete %d message:\n", queue->countDeleted + 1);
            printMessage((Message*)(((Node*)(queue->ringHead + (uintptr_t)queue))->message));
            pop(queue);
            queue->countDeleted++;
        } else
            printf("Queue is empty!\n");

        sem_post(extractSem);   // Разблокировать семафоры.
        sem_post(queueAccess);
        sleep(3);
    }

    // Отсоединить память.
    munmap(queue, sizeof(Queue) + MAX_MESSAGE_COUNT * sizeof(Node) + MAX_MESSAGE_COUNT * sizeof(Message));
    close(shmFd);

    // Закрыть семафоры.
    sem_close(queueAccess);
    sem_close(extractSem);
}

void changeContinuingStatus() {
    continuing ^= 1;
}//

