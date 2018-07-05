#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

int is_full = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 	cond  = PTHREAD_COND_INITIALIZER; 

//생산자 스레드
void* download(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		//---------------------------------
		if(!is_full)
		{
			printf(" downloading...\n");
			sleep(3);
			is_full = 1;
			printf("complete!\n");
		}
		//---------------------------------
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
	}
}
//소비자 스레드
void* play(void *arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond, &mutex); // ->pthread_mutex_unlock();
		//---------------------------------
		if(is_full)
		{
			printf("playing...\n");
			sleep(3);
			is_full=0;
			printf("it's_done!\n");

		}
		//---------------------------------
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}


int main()
{
	pthread_t tid1, tid2;
	pthread_create(&tid2, 0, play, 0);
	pthread_create(&tid1, 0, download, 0);

	pthread_join(tid1, 0);
	pthread_join(tid2, 0);

	return 0;
}


#if 0
//정적 초기화
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 	cond  = PTHREAD_COND_INITIALIZER; 

int loop_cnt = 0;
void* thread_main(void* arg)
{
	const char *str = (const char*)arg;

	for(int i=0;i<5;i++)
	{
		sleep(1);
		printf("%s \n", str);
	}

	return "complete...";
}


void* loop_thread(void* arg)
{

	for(int i=0;i<500000;i++)
	{
		pthread_mutex_lock(&mutex);
		//---------------------------
		++loop_cnt;
		//---------------------------
		pthread_mutex_unlock(&mutex);
	}

	return NULL;
}




int main()
{
	pthread_t tid1, tid2;

	pthread_create(&tid1, NULL, loop_thread, NULL);
	pthread_create(&tid2, NULL, loop_thread, NULL);
	//	pthread_mutex_init(&mutex, NULL);
	//	pthread_cond_init(&cond, NULL);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	printf("loop_cnt = %d\n",loop_cnt);
	//	char *msg;
	//	pthread_join(tid, (void**)&msg);
	//	printf("%s \n", msg);;

	return 0;
}
#endif
