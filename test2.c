#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

pthread_cond_t g_condition;  // 조건변수는 단지 상태정보만을 알리기 위해서 사용되며, (※ 조건변수는 Lock 기능 없음.)
pthread_mutex_t g_mutex;     // 상태정보를 원자적(atomic)으로 주고받기 위해서는 뮤텍스와 함께 사용해야 한다.

void *time_thread()
{
	struct timeval now;
	struct timespec ts;

	gettimeofday(&now, NULL);
	ts.tv_sec = now.tv_sec + 3;
	ts.tv_nsec = now.tv_usec * 1000;

	pthread_mutex_lock(&g_mutex);
	printf("[client]now waiting wakeup signal about 5 sec..\n");
	pthread_cond_wait(&g_condition, &g_mutex);
	//pthread_cond_timedwait(&g_condition, &g_mutex, &ts);
	printf("[client]i wakeup..\n");
	pthread_mutex_unlock(&g_mutex);
	return NULL;
}


void *main_thread()
{
	pthread_mutex_lock(&g_mutex);
	printf("[main]i will send wakeup signal 7sec later.. \n");
	sleep(7);
	pthread_cond_signal(&g_condition);
	printf("[main]i send wakeup signal now.. \n");
	pthread_mutex_unlock(&g_mutex);
	return NULL;
}
int main(int argc, char **argv)
{

	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_condition, NULL);

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, time_thread, NULL);
	getchar();
	pthread_create(&tid2, NULL, main_thread, NULL);


	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("thread's working is done.\n");


	pthread_cond_destroy(&g_condition);
	pthread_mutex_destroy(&g_mutex);
	return 0;
}


