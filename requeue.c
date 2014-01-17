#include <linux/futex.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdio.h>

// Test behavior of futex requeue operation.

// Code copied from http://locklessinc.com/articles/futex_cheat_sheet/
static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
    return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

static int ft_wait;
static int ft_requ;

void *wait(void *dummy)
{
    printf("futex_wait on ft_wait\n");
    sys_futex(&ft_wait, FUTEX_WAIT, ft_wait, NULL, NULL, 0);
    printf("futex_wait on ft_wait returned\n");
}

void *wake(void *dummy)
{
    sleep(1);
    printf("requeue from ft_wait to ft_requ\n");
    sys_futex(&ft_wait, FUTEX_REQUEUE, 0, (void *)1, &ft_requ, 0);

    sleep(1);
    printf("futex_wake on ft_wait\n");
    // This will not wake up the wait thread, as it has been moved from ft_wait
    // futex.
    sys_futex(&ft_wait, FUTEX_WAKE, 1, NULL, NULL, 0);

    sleep(1);
    printf("futex_wake on ft_requ\n");
    // This will wake up the wait thread.
    sys_futex(&ft_requ, FUTEX_WAKE, 1, NULL, NULL, 0);
}

int main(int argc, const char *argv[])
{
    pthread_t thr_wait, thr_wake;

    pthread_create(&thr_wait, NULL, wait, NULL);
    pthread_create(&thr_wake, NULL, wake, NULL);

    pthread_join(thr_wait, NULL);
    pthread_join(thr_wake, NULL);

    return 0;
}

