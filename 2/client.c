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
int main(int argc, char *argv[])
{	
	char dir[N];
	if (getcwd(dir, N) == NULL) {
		printf("getcwd error\n");
			exit(errno);
	}
	char path[N];
	
	sprintf(path, "%s/file.txt", dir);
        if(argc < 2){
                perror("argv");
                return errno;
        }
	struct stat *file = (struct stat *) malloc (sizeof(struct stat));
        if (!file){
              	perror("malloc error\n");
                return(errno);
        }
        if ( stat(argv[1],file) == -1){
                perror("stat error\n");
                return(errno);
        }
        long int file_size = file->st_size;

	
        key_t shm_key, shm_key1;
	struct sembuf my_sem[4];
        if ( (shm_key = ftok(path, 0)) == -1){
		perror("ftok error\n");
		return (errno);
       	}
	if ( (shm_key1 = ftok(path , 1)) < 0){
                perror("ftok error\n");
                return (errno);
        }

        int shm_id;
        if ( (shm_id =  shmget(shm_key, 4096, IPC_CREAT|0666)) == -1){
		perror("shmget error\n");
		return(errno);
        }

        char *buf;
	if ( (buf = (char *) shmat (shm_id, NULL, 0)) == (char *)(-1) ){
       	perror("shmat error\n");
		return(errno);
	}
	int sem_id;
	if ( (sem_id =  semget(shm_key1, 3, IPC_CREAT|0666)) < 0){
		perror("semget");
		return errno;
	}

	 
	int fd;
        if (( fd = open (argv[1], O_RDONLY) ) == -1) {
                printf("error open\n");
                exit (errno);
        }
//	semctl(sem_id, 0 , SETVAL, 1 );
  //      (semctl(sem_id, 1, SETVAL, 0));
  	while(1){

		my_sem[0].sem_num = 0;
                my_sem[0].sem_op = -1;
                my_sem[0].sem_flg = 0;

                my_sem[1].sem_num = 1;
                my_sem[1].sem_op = 1;
                my_sem[1].sem_flg = 0;

                my_sem[2].sem_num = 1;
                my_sem[2].sem_op = -1;
                my_sem[2].sem_flg = SEM_UNDO;

                my_sem[3].sem_num = 1;
                my_sem[3].sem_op = 0;
                my_sem[3].sem_flg = IPC_NOWAIT;
               

//	buf[0] = 19;
//	buf[1] = 27;
//	buf[2] = 5;
		
		if ( semop (sem_id, my_sem, 4 ) == -1){
        	        printf("mop2 error\n");
               		exit (errno);
       		}
		int rd = read(fd, buf+size, page_size);//читаем текст 
		int new_rd = rd;
		if( rd  == -1){
			printf("read from file error\n");
			exit (errno);
		}
		for (int i = 1; i <= size; i ++){
			if (new_rd % 2 == 0)
				*(buf + size - i) = '0';//записываем в буф размер
			else
				*(buf + size - i) = '1';
			new_rd = new_rd / 2;
		}
		if( rd == 0){
			break;
		}
		
		my_sem[0].sem_num = 1;
	        my_sem[0].sem_op = 1;
	        my_sem[0].sem_flg = 0;
		if ( semop (sem_id, my_sem, 1 ) == -1){
                        printf("mop3 error\n");
                        exit(errno);
                }

	}
}
