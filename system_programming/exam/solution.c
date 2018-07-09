
// 사번과 이름을 적으시오.
// 사번:
// 이름:

#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
time_t current_alarm = 0; // 가장 먼저 실행해야 할 알람의 시간

alarm_t *waiting_alarm = NULL;

void display_list() {
	pthread_mutex_lock(&alarm_mutex);

	if (alarm_list == NULL && waiting_alarm == NULL) {
		printf("[no alarm]\n");
		goto unlock;
	}

	if (waiting_alarm)
		printf("[(%lu) %s]", waiting_alarm->time - time(NULL), waiting_alarm->message);

	for (alarm_t *cur = alarm_list; cur != NULL; cur = cur->next)
		printf("[(%lu) %s]", cur->time - time(NULL), cur->message);
	printf("\n");

unlock:
	pthread_mutex_unlock(&alarm_mutex);
}

void alarm_insert (alarm_t *alarm){
	alarm_t **last = &alarm_list;
	alarm_t *next = *last;

	while (next != NULL) {
		// 삽입된 알람의 시간이 리스트의 알람보다작다면 헤드에 삽입하고 탈출한다.
		if (next->time >= alarm->time) {
			alarm->next = next;
			*last = alarm;
			break;
		}
		// 삽입된 알람의 시간이 리스트의 알람보다  더 크다면 다음 노드로 이동
		last = &next->next;
		next = next->next;
	}
	
	if (next == NULL) {
		*last = alarm;
		alarm->next = NULL;
	}
}

void *alarm_thread (void *arg) {
	int status = pthread_mutex_lock (&alarm_mutex);
	if (status != 0)
		err_abort (status, "Lock mutex");

	while (1) {
		current_alarm = 0;

		// 알람이 없다면 무한정 대기
		if (alarm_list == NULL) {
			waiting_alarm = NULL;
			status = pthread_cond_wait (&alarm_cond, &alarm_mutex);
			if (status != 0)
				err_abort (status, "Wait on cond");
		}

		waiting_alarm = alarm_list;	// for display alarm

		// 알람 리스트에서 앞의 알람을 뜯어온다.
		alarm_t *alarm = alarm_list;
		alarm_list = alarm->next;
		alarm->next = NULL;

		time_t now = time (NULL);
		int expired = 0;
		if (alarm->time > now) {	// 알람이 아직 미래의 시간이라면
			struct timespec cond_time = { 0, };
			cond_time.tv_sec = alarm->time;

			current_alarm = alarm->time;
			while (current_alarm == alarm->time) {
				status = pthread_cond_timedwait(&alarm_cond, &alarm_mutex, &cond_time);
				if (status == ETIMEDOUT) {
					expired = 1;
					break;
				}

				if (status != 0)
					err_abort (status, "Cond timedwait");
			} // end of while

			// 현재 쓰레드의 알람이 만료되지 않은 상태이고 더 늦게 실행해야 하는 경우
			// 알림 리스트에 삽입
			if (!expired)
				alarm_insert (alarm);
		}
		else
			expired = 1;

		if (expired) {
			printf ("%s\n", alarm->message);
			free (alarm);
		}
	}
}

int main() {
	pthread_t thread;
	int status = pthread_create(&thread, NULL, alarm_thread, NULL);
	if (status != 0)
		err_abort (status, "Create alarm thread");

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

		alarm_t *alarm = (alarm_t*)malloc (sizeof (alarm_t));
		if (alarm == NULL)
			errno_abort ("Allocate alarm");

		if (sscanf (line, "%d %64[^\n]", &alarm->seconds, alarm->message) < 2) {
			fprintf (stderr, "Bad command\n");
			free (alarm);
		}
		else {
			status = pthread_mutex_lock (&alarm_mutex);
			if (status != 0)
				err_abort (status, "Lock mutex");

			alarm->time = time(NULL) + alarm->seconds;
			alarm_insert (alarm);
			// 실행할 알람이 없거나 새로운 알람이 가장 먼저 실행애햐 할 알람보다 빠르다면
			if (current_alarm == 0 || alarm->time < current_alarm) {
				current_alarm = alarm->time;
				status = pthread_cond_signal (&alarm_cond);
				if (status != 0)
					err_abort (status, "Signal cond");
			}
			status = pthread_mutex_unlock (&alarm_mutex);
			if (status != 0)
				err_abort (status, "Unlock mutex");
		}
	}
}


