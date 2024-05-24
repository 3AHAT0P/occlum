#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    struct rusage usage;
    struct rusage usage2;
    struct timeval start, end;
    int i, j, k, g = 0;

    signed char err1 = getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_stime;

    for (g = 0; g < 3; g++)
    {
        for (i = 0; i < 10000; i++)
        {
            /* Double loop for more interesting results. */
            for (j = 0; j < 10000; j++)
            {
                k += 20;
            }
        }

        printf("End of global loop %ld times\n", g);
    }
    signed int err2 = getrusage(RUSAGE_SELF, &usage2);
    end = usage2.ru_stime;

    printf("err1 is %d, err2 is %d \n", err1, err2);

    printf("Started at: %ld.%lds\n", start.tv_sec, start.tv_usec);
    printf("Ended at: %ld.%lds\n", end.tv_sec, end.tv_usec);
    printf("ru_maxrss at: %ld %ld\n", usage.ru_maxrss, usage2.ru_maxrss);
    printf("ru_ixrss at: %ld %ld\n", usage.ru_ixrss, usage2.ru_ixrss);
    printf("ru_idrss at: %ld %ld\n", usage.ru_idrss, usage2.ru_idrss);
    printf("ru_isrss at: %ld %ld\n", usage.ru_isrss, usage2.ru_isrss);
    printf("ru_minflt at: %ld %ld\n", usage.ru_minflt, usage2.ru_minflt);
    printf("ru_majflt at: %ld %ld\n", usage.ru_majflt, usage2.ru_majflt);
    printf("ru_nswap at: %ld %ld\n", usage.ru_nswap, usage2.ru_nswap);
    printf("ru_inblock at: %ld %ld\n", usage.ru_inblock, usage2.ru_inblock);
    printf("ru_oublock at: %ld %ld\n", usage.ru_oublock, usage2.ru_oublock);
    printf("ru_msgsnd at: %ld %ld\n", usage.ru_msgsnd, usage2.ru_msgsnd);
    printf("ru_msgrcv at: %ld %ld\n", usage.ru_msgrcv, usage2.ru_msgrcv);
    printf("ru_nsignals at: %ld %ld\n", usage.ru_nsignals, usage2.ru_nsignals);
    printf("ru_nvcsw at: %ld %ld\n", usage.ru_nvcsw, usage2.ru_nvcsw);
    printf("ru_nivcsw at: %ld %ld\n", usage.ru_nivcsw, usage2.ru_nivcsw);
    return 0;
}
