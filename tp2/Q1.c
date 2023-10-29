#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int prio = getpriority(PRIO_PROCESS, atoi(argv[1]));
    printf("%d \n",prio);

    FILE *fp = fopen("/proc/self/sched", "r");
    char line[300];
    int priority;

    while (fgets(line, 300, fp)) {
        if (strstr(line, "prio") && sscanf(line, "prio : %d", &priority) == 1) {
            printf("%d\n", priority);
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 0;
}