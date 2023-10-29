#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    int prio = getpriority(PRIO_PROCESS, pid);
    printf("La priorité du processus (PID %d) est : %d\n", pid, prio);

    FILE *fp = fopen("/proc/self/sched", "r");
    char line[300];
    int priority;

    while (fgets(line, 300, fp)) {
        if (strstr(line, "prio") && sscanf(line, "prio : %d", &priority) == 1) {
            printf("La priorité du processus dans /proc/self/sched est : %d\n", priority);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 0;
}
