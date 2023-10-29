#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PID>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);
    int priority_before = sched_getscheduler(pid);
    printf("La priorité du processus (PID %d) avant le changement est : %d\n", pid, priority_before);

    struct sched_param param;
    param.sched_priority = 50;

    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) {
        perror("Erreur de sched_setscheduler");
        return 1;
    }

    int priority_after = sched_getscheduler(pid);
    printf("La priorité du processus (PID %d) après le changement est : %d\n", pid, priority_after);

    return 0;
}
