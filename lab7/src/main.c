#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct record_s{
    char name[80];
    char address[80];
    u_int8_t semester;
};
int get_index(int fd,struct record_s* original){
    struct record_s record;
    int index = 0;
    while (read(fd, &record, sizeof(record)) == sizeof(record)) {
        if(strcmp(record.name,original->name) == 0 && strcmp(record.address,original->address) == 0 && record.semester == original->semester){
            return index;
        }
        index++;
    }
    return index;
}
int is_record_locked(int fd, int rec_no) {
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = rec_no * sizeof(struct record_s);
    lock.l_len = sizeof(struct record_s);

    if (fcntl(fd, F_GETLK, &lock) == -1) {
        // Ошибка при вызове fcntl()
        return 1;
    }

    return lock.l_type != F_UNLCK;
}
void lock_record(int fd, int rec_no, int lock_type) {
    struct flock lock;
    lock.l_type = lock_type;
    lock.l_whence = SEEK_SET;
    lock.l_start = rec_no * sizeof(struct record_s);
    lock.l_len = sizeof(struct record_s);
    while (fcntl(fd, F_SETLKW, &lock) == -1 && errno == EINTR);
}

void unlock_record(int fd, int rec_no) {
    struct flock lock;
    lock.l_type = F_UNLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = rec_no * sizeof(struct record_s);
    lock.l_len = sizeof(struct record_s);
    fcntl(fd, F_SETLK, &lock);
}
void put_record(int fd, int rec_no, struct record_s *record) {
    lseek(fd, rec_no * sizeof(struct record_s), SEEK_SET);
    write(fd, record, sizeof(struct record_s));
}
struct record_s* get_record(int fd, int rec_no) {
    if(is_record_locked(fd,rec_no)){
        printf("Запись залочена\n");
        return NULL;
    }
    struct record_s *record = malloc(sizeof (struct record_s));
    lseek(fd, rec_no * sizeof(struct record_s), SEEK_SET);
    read(fd, record, sizeof(struct record_s));
    return record;
}
int compare_record(struct record_s* record,struct record_s* original){
    if(strcmp(record->name,original->name) == 0 && strcmp(record->address,original->address) == 0 && record->semester == original->semester)
    {
        return 1;
    }
    return 0;
}

void display_records(int fd) {
    for(int i = 0;i<10;i++){
        if(is_record_locked(fd,i)){
            printf("В файле есть лок\n");
            return;
        }
    }
    struct record_s record;
    int index = 0;
    while (read(fd, &record, sizeof(record)) == sizeof(record)) {
        printf("Record %d:\n", index++);
        printf("  Name: %s\n", record.name);
        printf("  Address: %s\n", record.address);
        printf("  Semester: %d\n", record.semester);
    }

    close(fd);
}
int main() {
    char file_name[80];
    printf("Введите имя файла\n");
    scanf("%s",file_name);
    while (1){
        struct record_s* original;
        struct record_s* record;
        int fd = open(file_name,O_RDWR);
        getchar();
        char select = 0;
        printf("1. LST\n2. get\n3. mod\n4. quit\n");
        scanf("%c",&select);
        if(select == '1'){
            display_records(fd);
        }
        if(select == '2'){
            int n = 0;
            printf("Введите индекс\n");
            scanf("%d",&n);
            if(n<0 || n>9){
                printf("Выход за пределы массива\n");
                return 0;
            }
            record = get_record(fd,n);
            if(record == NULL){
                continue;
            }
            printf("Record %d:\n", n);
            printf("  Name: %s\n", record->name);
            printf("  Address: %s\n", record->address);
            printf("  Semester: %d\n", record->semester);
            close(fd);

        }
        if(select == '3'){
            int n = 0;
            printf("Введите индекс\n");
            scanf("%d",&n);
            if(is_record_locked(fd,n)){
                printf("Запись залочена\n");
                continue;
            }
            lock_record(fd,n,F_RDLCK);
            if(n<0 || n>9){
                printf("Выход за пределы массива\n");
                return 0;
            }
            record = get_record(fd,n);
            char c;
            scanf("%s",&c);
            printf("Old semester = %d\n",record->semester);
            record->semester++;
            printf("New semester = %d\n",record->semester);
            put_record(fd,n,record);
            unlock_record(fd,n);
            close(fd);
        }
        if(select == '4'){
            close(fd);
            break;
        }

    }




    return 0;
}
