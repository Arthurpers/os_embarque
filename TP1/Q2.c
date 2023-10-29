#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256

void display_info(const char *filename, const char *keywords[], int num_keywords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    char buffer[MAX_BUFFER_SIZE];

    while (fgets(buffer, MAX_BUFFER_SIZE, file) != NULL) {
        for (int i = 0; i < num_keywords; i++) {
            if (strstr(buffer, keywords[i]) != NULL) {
                printf("%s", buffer);
                break;
            }
        }
    }
    fclose(file);
}

void display_file(const char *path) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            if (buffer[i] != '\0') {
                printf("%c", buffer[i]);
            } else {
                printf("\n");
            }
        }
    }
    fclose(file);
}


int get_ppid_from_pid(const char *pid) {
    char path[100];
    sprintf(path,"/proc/%s/stat", pid);

    int ppid;

    FILE *status_file = fopen(path, "r");
    fscanf(status_file, "%*d %*s %*c %d", &ppid);
    fclose(status_file);
    return ppid;
}



void display_process_info(const char *pid){
    char path[100];
    sprintf(path, "/proc/%s", pid);
    char path_cmd[200];
    sprintf(path_cmd, "%s/cmdline",path);
    char path_status[200];
    sprintf(path_status, "%s/status",path);

    int ppid = get_ppid_from_pid(pid);
    char parent_path[200];
    sprintf(parent_path, "/proc/%d", ppid);
    char parent_path_cmd[300];
    sprintf(parent_path_cmd, "%s/cmdline",parent_path);

    const char *keywords[] = {"Pid","Tgid","PPid","Threads","Uid","Gid"};

    printf("===CMD===\n");
    display_file(path_cmd);
    printf("\n");
    printf("=========\n");
    display_info(path_status,keywords,6);
    printf("===PARENT CMD===\n");
    display_file(parent_path_cmd);
    printf("\n");



}

int main(int argc, char *argv[]) {

    display_process_info(argv[1]);
    return 0;
}
