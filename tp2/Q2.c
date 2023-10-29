#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int pid = atoi(argv[1]);
    int prio1 = getpriority(PRIO_PROCESS, pid);
    printf("%d \n",prio1);

    int newpriority = atoi(argv[2]);
    int result = setpriority(PRIO_PROCESS, pid, newpriority);
    printf("return setpriority: %d \n", result);
    int prio2 = getpriority(PRIO_PROCESS, pid);
    printf("%d \n",prio2);

    return 0;
}