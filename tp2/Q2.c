#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <PID> <new_priority>\n", argv[0]);
        return 1;
    }

    int pid = atoi(argv[1]);
    int prio1 = getpriority(PRIO_PROCESS, pid);
    printf("La priorité du processus (PID %d) avant le changement est : %d\n", pid, prio1);

    int newpriority = atoi(argv[2]);
    int result = setpriority(PRIO_PROCESS, pid, newpriority);
    printf("Return de setpriority : %d\n", result);

    int prio2 = getpriority(PRIO_PROCESS, pid);
    printf("La priorité du processus (PID %d) après le changement est : %d\n", pid, prio2);

    return 0;
}
