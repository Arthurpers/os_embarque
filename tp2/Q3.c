#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    int pid = atoi(argv[1]);
    int prio1 = getpriority(PRIO_PROCESS, pid);
    printf("prio : %d \n",prio1);

    struct sched_param sp = { .sched_priority = 50 };
    int ret;
    ret = sched_setscheduler(pid, SCHED_FIFO, &sp);
    if (ret == -1) {
    perror("sched_setscheduler");
    return 1;
    }
    printf("ret : %d\n", ret);

    int prio2 = getpriority(PRIO_PROCESS, pid);
    printf("prio : %d \n",prio2);

    return 0;
}
