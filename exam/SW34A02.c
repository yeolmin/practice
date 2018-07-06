
// 사번과 이름을 적으시오.
// 사번:	79559
// 이름:	성열민

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#define err_abort(code,text) do {       					\
	fprintf(stderr, "%s at \"%s\":%d: %s\n",  			\
			text, __FILE__, __LINE__, strerror(code));  \
	abort();    																		\
}while(0)

#define errno_abort(text) do {       								\
	fprintf(stderr, "%s at \"%s\":%d: %s\n",  				\
			text, __FILE__, __LINE__, strerror(errno));		\
	abort();    																			\
} while(0)

typedef struct alarm_tag {
	struct alarm_tag    *next;
	int                 seconds;
	time_t              time;   /* seconds from EPOCH */
	char                message[64];
} alarm_t;


pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;
alarm_t *alarm_list = NULL;

// 전역 변수는 최대 2개까지 추가 가능합니다.
// 아래의 함수를 구현하세요.
void display_list() {
	
	
	alarm_t*  temp;
	for(temp = alarm_list;temp!=NULL;temp =temp->next)
		printf("%s  %d \n", temp->message, temp->seconds);

}

// 아래의 함수를 구현하세요.
void alarm_insert (alarm_t *alarm){


	//printf("%s  %d \n", alarm->message, alarm->seconds);

	if(alarm_list == NULL)
		alarm_list = alarm;

	else
	{
		if(alarm_list->seconds > alarm->seconds)
		{
			alarm->next = alarm_list;
			alarm_list = alarm;
		}

		else
		{

			alarm->next = alarm_list->next;
			alarm_list->next = alarm;
			
		}
	}
}
void *alarm_thread (void *arg) {
	int status = pthread_mutex_lock (&alarm_mutex);
	if (status != 0)
		err_abort (status, "Lock mutex");

	while (1) {

		if(alarm_list == NULL)
			pthread_cond_wait(&alarm_cond, &alarm_mutex);


		time_t T;
		time(&T);


		//	printf("time : %s", ctime(T - alarm_list->time));
		// 나머지를 구현하세요.
		//	pthread_cond_timedwait(&g_condition, &g_mutex, &ts);

		return NULL;
	}
}

int main() {
	pthread_t thread;
	int status = pthread_create(&thread, NULL, alarm_thread, NULL);
	if (status != 0)
		err_abort (status, "Create alarm thread");

	int flag;
	while (1) {
		printf("Alarm> ");

		char line[128];
		if (fgets(line, sizeof (line), stdin) == NULL)
			exit (0);

		if (strlen(line) <= 1)
			continue;

		if (strncmp(line, "list", 4) == 0) {
			display_list();
			continue;
		}

		alarm_t* temp = malloc(sizeof(alarm_t));
		temp->next = NULL;
		char *token;
		flag = 0;

		token = strtok( line, "  \n");
		while(token)
		{
			if(flag == 0){
				strcpy(temp->message, token);
			}
			else
			{
				temp->seconds = atoi(token);
			}
			token = strtok(NULL, " \n");
			flag =1;
		}

		time_t T;
		struct timespec ts;

		time(&T);
		ts.tv_sec = T;

		temp->time = T;

		//pthread_mutex_lock(&alarm_mutex);
		//------------------------------
		alarm_insert(temp);
		display_list();
		pthread_cond_signal(&alarm_cond);
		//pthread_mutex_unlock(&alarm_mutex);
		/*
		 */
		// 이곳에서 알람 생성 및 리스트 추가 그리고 시그널 전송 코드를
		// 구현합니다.
	}
}


