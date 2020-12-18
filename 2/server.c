#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <errno.h>


#define pathname "/home/podles/Downloads/serv.c"

#define page_size 4096
#define N 256
#define size 13
char dir[N];
int main(int argc, char *argv[])
{	
	
	if (getcwd(dir, N) == NULL) {
		printf("getcwd error\n");
		exit(errno);
	}
	
	char path[N];
	
	sprintf(path, "%s/file.txt", dir);
	open(path, O_CREAT);
        key_t shm_key, shm_key1;
	struct sembuf my_sem[4];
        if ( (shm_key = ftok(path, 0)) == -1){
		printf("ftok error1\n");
		exit (errno);
        }
	if ( (shm_key1 = ftok(path , 1)) < 0){
                printf("ftok error2\n");
                exit (errno);
        }

        int shm_id;
        if ( (shm_id =  shmget(shm_key, 4096, IPC_CREAT|0666)) == -1){
		printf("shmget error\n");
		exit(errno);
	}
	char *buf;
        if ( (buf = (char *) shmat (shm_id, NULL, 0)) == (char *)(-1) ){
       	printf("shmat error\n");
		exit(errno);
	}
	int sem_id;
	if ( (sem_id =  semget(shm_key1, 3, IPC_CREAT|0666)) < 0){
		perror("semget");
		return errno;
      	}
	my_sem[0].sem_num = 0;
        my_sem[0].sem_op = 1;
        my_sem[0].sem_flg = 0;
	if ( semop (sem_id, my_sem, 1) == -1){
                printf("semop1 error\n");
                exit(errno);
        }


//	semctl(sem_id, 0 , SETVAL, 1 );
  //      (semctl(sem_id, 1, SETVAL, 0));
  	while(1){

		my_sem[0].sem_num = 1;
        	my_sem[0].sem_op = -1;
      		my_sem[0].sem_flg = 0;

		my_sem[1].sem_num = 0;
		my_sem[1].sem_op = 1 ;
		my_sem[1].sem_flg = 0;

		my_sem[2].sem_num = 0;
                my_sem[2].sem_op = -1;
                my_sem[2].sem_flg = SEM_UNDO;

                my_sem[3].sem_num = 0;
                my_sem[3].sem_op = 0;
                my_sem[3].sem_flg = IPC_NOWAIT;


//	buf[0] = 19;
//	buf[1] = 27;
//	buf[2] = 5;
		
		if ( semop (sem_id, my_sem, 4 ) == -1){
        	       // perror("mop2 error\n");
               		//return errno;
			exit(errno);
       		}

//	printf("sem1 %d,sem 2 %d ", (semctl(sem_id, 0, GETVAL, 0 )), (semctl(sem_id, 1, GETVAL, 0 )));
      		int razm = 0;
		int x = 1;
		for (int i = 1; i <= size; i++){
			if ( *(buf+size-i) == '1'){
				razm += x;//считаем требуемый размер файла
			}
			x = x * 2;
		}
		if ((write(1, buf+size, razm)) == -1) {//
			printf("write error\n");
			exit(errno);
		}

		my_sem[0].sem_num = 0;
	        my_sem[0].sem_op = 1;
	        my_sem[0].sem_flg = 0;
		if ( semop (sem_id, my_sem, 1 ) == -1){
                        printf("mop3 error\n");
                        exit (errno);
                }

	}
}
