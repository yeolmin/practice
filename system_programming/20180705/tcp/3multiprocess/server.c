
// 이전 서버의 경우, 넌블럭 기반으로 작성되어 CPU의 자원을 많이 사용한다는
// 단점이 있습니다. 이를 해결하기 위해 멀티 프로세스 기반으로 구현합니다.
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h> // for struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void __error_quit(const char *msg, int line) {
	char buf[1024];
	sprintf(buf, "%s(%d)", msg, line);
	perror(buf);
	exit(-1);
}
#define error_quit(msg) __error_quit(msg, __LINE__) 

void sig_handler(int sig_no) {
	while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
	if (signal(SIGCHLD, sig_handler) == SIG_ERR)
		error_quit("signal");
	
	int ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (ssock < 0)
		error_quit("socket");

	struct sockaddr_in saddr = { 0, };
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(8080);
	saddr.sin_addr.s_addr = INADDR_ANY;

	int val = 1;
	int ret = setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR,
			(char*)&val, sizeof(val));
	if (ret < 0)
		perror("setsockopt");

	ret = bind(ssock, (struct sockaddr*)&saddr, sizeof(saddr));
	if (ret < 0)
		error_quit("bind");

	ret = listen(ssock, 10);
	if (ret < 0)
		error_quit("listen");

	printf("[server] running...\n");
	while (1) {
		struct sockaddr_in caddr = { 0, };
		int addr_len = sizeof(caddr);
		int csock = accept(ssock, (struct sockaddr*)&caddr, &addr_len);
		if (csock < 0)
			error_quit("accept");
		printf("[server] %s is connected\n", inet_ntoa(caddr.sin_addr));
		pid_t pid = fork();
		if (pid == 0) {
			close(ssock);
			char buf[4096];
			ret = read(csock, buf, sizeof(buf)); 
			if (ret < 0)
				error_quit("read");
			buf[ret] = 0;
			write(csock, buf, ret);
			close(csock);
			return 0;
		}
		else if (pid > 0) {
			close(csock);
		}
		else {
			perror("[server] fork");
		}
	}
	close(ssock);
	return 0;
} 
