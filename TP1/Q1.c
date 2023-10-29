#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

void display_info(const char *filename, const char *keywords[], int num_keywords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    
    int keywords_to_display[num_keywords];
    for (int i = 0; i < num_keywords; i++) {
        keywords_to_display[i] = 1;  
    }

    char buffer[MAX_BUFFER_SIZE];

    while (fgets(buffer, MAX_BUFFER_SIZE, file) != NULL) {
        for (int i = 0; i < num_keywords; i++) {
            if (keywords_to_display[i] && strstr(buffer, keywords[i]) != NULL) {
                printf("%s", buffer);
                keywords_to_display[i] = 0;
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

int main() {
    const char *cpu_keywords[] = {"model name", "cpu MHz", "cache size", "address sizes"};
    const char *memory_keywords[] = {"MemTotal", "MemFree"};
    const char *kernel_version_keyword[] = {"Linux version"};

    printf("=== CPU Info ===\n");
    display_info("/proc/cpuinfo", cpu_keywords,4);

    printf("\n=== Memory Info ===\n");
    display_info("/proc/meminfo", memory_keywords,2);

    printf("\n=== Partitions Info ===\n");
    display_file("/proc/partitions");

    printf("\n=== Kernel Version ===\n");
    display_info("/proc/version", kernel_version_keyword,1);

    return 0;
}
