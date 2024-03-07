
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <malloc.h>

struct options{
    int f;
    int l;
    int d;
    int s;
    int startOfPath;
};
struct options processData(char*argv[],int argc){
    struct options option;
    option.s = 0;
    option.f = 0;
    option.startOfPath = -1;
    option.d = 0;
    option.l = 0;

    for(int i = 1;i<argc;i++){
        if(argv[i][0] == '-'){
            for(int b= 0;b<4;b++){
                if(argv[i][b] == ' '){
                    break;
                }
                if(argv[i][b] == 'f')  option.f = 1;
                if(argv[i][b] == 'd')  option.d = 1;
                if(argv[i][b] == 'l')  option.l = 1;
                if(argv[i][b] == 's')  option.s = 1;
            }
        } else{
            option.startOfPath = i;
            break;
        }
    }
    return option;
}
int compare(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}
int main(int argc, char *argv[]) {
    struct options option = processData(argv,argc);
    char* path;
    if(option.startOfPath == -1){
        path = "./";
    }
    else{path = argv[option.startOfPath];}
    if(!option.d && !option.l && !option.f){
        option.f =1;
        option.l =1;
        option.d = 1;
    }
    struct dirent **namelist;
    int n;
    if(option.s == 0){
        n = scandir(path,&namelist,NULL,NULL);
    } else{
        n = scandir(path,&namelist,NULL,compare);
    }
    for (int i = 0;i<n;i++){
        if(option.f && namelist[i]->d_type == DT_REG) printf("%s\n",namelist[i]->d_name);
        if (option.d && namelist[i]->d_type == DT_DIR) { printf("%s\n", namelist[i]->d_name); }
        if(option.l && namelist[i]->d_type == DT_LNK) printf("%s\n", namelist[i]->d_name);
    }
    for (int i = 0; i < n; i++) {
        free(namelist[i]);
    }
    free(namelist);
    return 0;
}
