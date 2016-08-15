#include <unistd.h>
#include <stdio.h>
#include <sched.h>

int main() {
    for (int i = 0; i < 20; i++) {
        if (fork() == 0) {
            printf("%d\n", sched_getcpu());
            return 0;
        }
    }
    return 0;
}

