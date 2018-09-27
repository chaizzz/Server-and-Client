#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)
typedef struct _file_info
{
        char name[51];
        unsigned int size;

}file_info;

//恢复默认的对齐方式
#pragma pack()


int main(int argc, char** argv)
{
	//第一步：创建套接字
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	/*
	//第二步：绑定地址（可省略）
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(99);

	if(bind(sock, (struct sockaddr*)&myaddr, sizeof(myaddr)) == -1)
	{
		perror("bind error");
		//exit(1);
	}
	*/
	//第三步：连接服务器
	//指定服务器地址
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr	= inet_addr(argv[1]);
	srvaddr.sin_port = htons(atoi(argv[2]));


	//连接服务器
	if(connect(sock, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) == -1)
	{
		perror("connect");
		exit(1);
	}



	//第四步：收发数据
	file_info msg;
	int ret = 0, ret1 = 0;

	//收数据
	ret = recv(sock, &msg, sizeof(msg), 0);
	printf("%d\n", ret);

	char* buff = malloc(msg.size);
	if(ret > 0)
	{
		FILE* fp = fopen(msg.name, "wb");
		while(ret < msg.size)
		{
			ret1 = recv(sock, buff, msg.size, 0);
			fwrite(buff, 1, ret1, fp);
			ret += ret1;
			printf("%d%/%d%\n", ret/msg.size*100, msg.size/msg.size*100);
			usleep(10000);
			system("clear");
		}
		fclose(fp);
	}

	/*
	   char mmm[1000];
	   scanf("%s", mmm);	

	//发数据
	strcpy(msg, mmm);
	send(sock, msg, strlen(msg), 0);
	*/

	close(sock);

	return 0;
}
