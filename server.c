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

#define path "fifo.c"
#define pathname "inp2"

int main(int argc, char *argv[])
{	
	
        key_t shm_key, shm_key1;
	struct sembuf my_sem[4];
        if ( (shm_key = ftok(path, 0)) == -1){
		printf("ftok error\n");
		exit (errno);
       	}
	if ( (shm_key1 = ftok(pathname , 0)) < 0){
                printf("ftok error\n");
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

	int i = 0; 
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

                my_sem[3].sem_num = 1;
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
      		printf ("%c", *(buf + i) );
		i++;

		my_sem[0].sem_num = 0;
	        my_sem[0].sem_op = 1;
	        my_sem[0].sem_flg = 0;
		if ( semop (sem_id, my_sem, 1 ) == -1){
                        perror("mop3 error\n");
                        return errno;
                }

	}
}

