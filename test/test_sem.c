#include "../include/requires.h"

const char *sem_name1 = "sem1";
const char *sem_name2 = "sem2";

sem_t *sem1, *sem2;


void *test_func(void *arg) {
    (void)arg; 
    sem_wait(sem1);
    sem_wait(sem2);

    printf("test func after sem_wait\n");
    pthread_exit(NULL);
}


int main() {
    pthread_t tid;

    sem_unlink(sem_name1);
    sem_unlink(sem_name2);
    if ((sem1 = sem_open(sem_name1, O_CREAT | O_RDWR, FILE_MODE, 0)) == SEM_FAILED) {
        err_quit("create sem fail: %s", sem_name1);
    }
    if ((sem2 = sem_open(sem_name2, O_CREAT | O_RDWR, FILE_MODE, 0)) == SEM_FAILED) {
        err_quit("create sem fail: %s", sem_name2);
    }

    pthread_create(&tid, NULL, test_func, NULL);

    printf("start sleeping...\n");
    sleep(2);
    printf("main wake up from sleep\n");
    sem_post(sem1);
    sem_post(sem2);
    pthread_join(tid, NULL);

}
