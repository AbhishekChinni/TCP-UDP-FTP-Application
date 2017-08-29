/* tcpclient.c */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<wait.h>
#define Jan 1
#define Feb 2
#define Mar 3
#define Apr 4
#define May 5
#define Jun 6
#define Jul 7
#define Aug 8
#define Sep 9
#define Oct 10
#define Nov 11
#define Dec 12
int FHflag=0;
void parser(char*,char*,char*,char*,char*,char*);
void parser1();
void parser2(char *);
int main()
{
	int sock, bytes_recieved,connected,true=1;  
	char send_data[1024],recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;
	int sin_size;
	host = gethostbyname("127.0.0.1");
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);  // set to non-blocking
	fcntl(sock, F_SETFL, O_ASYNC);     // set to asynchronous I/O
	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1) 
	{
	struct sockaddr_in client_addr;
		server_addr.sin_family = AF_INET;         
		server_addr.sin_port = htons(5000);     
		server_addr.sin_addr.s_addr = INADDR_ANY; 
		bzero(&(server_addr.sin_zero),8); 

		if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
				== -1) {
			perror("Unable to bind");
			exit(1);
		}

		if (listen(sock, 5) == -1) {
			perror("Listen");
			exit(1);
		}

		printf("\nTCPServer Waiting for client on port 5000");
		fflush(stdout);
		while(1)
		{  

			sin_size = sizeof(struct sockaddr_in);

			connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

			printf("\n I got a connection from (%s , %d)",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

			while (1)
			{
				printf("\n SEND (q or Q to quit) : ");
				gets(send_data);

				if (strcmp(send_data , "q") == 0 || strcmp(send_data , "Q") == 0)
				{
					send(connected, send_data,strlen(send_data), 0); 
					close(connected);
					break;
				}

				else
					send(connected, send_data,strlen(send_data), 0);  

				bytes_recieved = recv(connected,recv_data,1024,0);

				recv_data[bytes_recieved] = '\0';

				if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
				{
					close(connected);
					break;
				}

				else 
					printf("\n RECIEVED DATA = %s " , recv_data);
				fflush(stdout);
			}
		}
	}
	else
	{
	while(1)
	{
		bytes_recieved=recv(sock,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';
		char *str=strtok(recv_data," ");

		if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
		{
			close(sock);
			break;
		}
		else if(strcmp(str,"IndexGet")==0)
		{
			str=strtok(NULL," ");
			if(strcmp(str,"ShortList")==0)
			{
				str=strtok(NULL," ");
				char * month1=str;
				str=strtok(NULL," ");
				char * date1 = str;
				str=strtok(NULL," ");
				char * t1=str;
				str=strtok(NULL," ");
				char * month2=str;
				str=strtok(NULL," ");
				char * date2 = str;
				str=strtok(NULL," ");
				char * t2 = str;
				int pid;
				pid=fork();
				if(pid==0)
				{
					execlp("./1.sh","./1.sh",NULL);
				}
				else
				{
					int xxx;
					wait(&xxx);
					parser(month1,date1,t1,month2,date2,t2);
					FILE* fp;
					fp = fopen("final","r");
					while(!feof(fp)) {
						char filebuffer[1024];
						int wasRead = fread (filebuffer, sizeof(char), 1024, fp);
						send (sock, filebuffer, (sizeof (char) * wasRead), 0);
					}
				}
				
			}
			else if(strcmp(str,"LongList")==0)
			{
				int pid;
				pid=fork();
				if(pid==0)
				{
					execlp("./1.sh","./1.sh",NULL);
				}
				else
				{
					int xxx;
					wait(&xxx);
					parser1();
					FILE* fp;
					fp = fopen("final","r");
					while(!feof(fp)) {
						char filebuffer[1024];
						int wasRead = fread (filebuffer, sizeof(char), 1024, fp);
					send (sock, filebuffer, (sizeof (char) * wasRead), 0);
				}
				}

			}
			else if(strcmp(str,"RegEx")==0)
			{
				char *a=strtok(NULL,"\n");
				int pid;
				pid=fork();
				if(pid==0)
				{
					puts("ENTERING");
//					b[0]="ls";
//					b[1]="|";
//					b[2]="grep";
//					b[3]=a;
//					b[4]=">";
//					b[5]="final";

					execlp("regex.sh","regex.sh",a,NULL);
				}
				else
				{
					int xxx;
					wait(&xxx);
					FILE* fp;
					fp = fopen("final","r");
					while(!feof(fp)) {
						char filebuffer[1024];
						int wasRead = fread (filebuffer, sizeof(char), 1024, fp);
						send (sock, filebuffer, (sizeof (char) * wasRead), 0);
					}


				}
			}


		}
		else if(strcmp(str,"FileHash")==0)
		{
			str=strtok(NULL," ");
			if(strcmp(str,"Verify")==0)
			{
				int pid;
				pid=fork();
				str=strtok(NULL,"\n");
				if(pid==0)
				{

					execlp("./211.sh","./211.sh",str,NULL);	
				}
				else
				{
					int xx;
					wait(&xx);

					parser2(str);
					puts("entered");
					FILE* fp;
					fp = fopen("final21","r");
					while(!feof(fp)) {
						char filebuffer[1024];
						int wasRead = fread (filebuffer, sizeof(char), 1024, fp);
						send (sock, filebuffer, (sizeof (char) * wasRead), 0);
					}

				}
			}
			else if(strcmp(str,"CheckAll")==0)
			{
				int pid;
				pid = fork();
				if(pid==0)
				{
					execlp("./212.sh","./212.sh",NULL);
				}
				else
				{
					int xx;
					wait(&xx);
					FILE* fp;
					fp = fopen("file","r");
					char line[200];
					while(fgets (line, sizeof line, fp)!=NULL)
					{
						puts("check");

						int p;
						p = fork();
						if(p==0)
						{
							execlp("./211.sh","./211.sh",line,NULL);	
						}
						else
						{
							int x;
							wait(&x);
							line[strlen(line)-1]='\0';
							parser2(line);
							//puts("entered");
							FILE* fp;
							fp = fopen("final21","r");
							while(!feof(fp)) {
								char filebuffer[2056];
								int wasRead = fread (filebuffer, sizeof(char), 1024, fp);
								send (sock, filebuffer, (sizeof (char) * wasRead), 0);
							}

						}
					}

				}
			}
		}
		else if(strcmp(str,"FileDownload")==0)
		{
			str=strtok(NULL," ");
			upload(str);
		}

		else
			printf("\nRecieved data = %s " , recv_data);
		printf("\nSEND (q or Q to quit) : ");
		gets(send_data);

		if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
			send(sock,send_data,strlen(send_data), 0); 

		else
		{
			send(sock,send_data,strlen(send_data), 0);   
			close(sock);
			break;
		}

	}
	}
	return 0;
}
void upload(char * str)
{
	str[strlen(str)-1]='\0';
	int p;
	p=fork();
	if(p==0)
	{
		execlp("./upload.sh","./upload.sh",str,NULL);
	}
	else
	{
		int x;
		wait(&x);
	FILE *f;
	f=fopen("upload","r");
	while(!feof(f)) {
		char filebuffer[2056];
		int wasRead = fread (filebuffer, sizeof(char), 2056, f);
		send (sock, filebuffer, (sizeof (char) * wasRead), 0);
	}
	FILE* fp;
	fp = fopen(str,"r");
	while(!feof(fp)) {
		int wasRead = fread (filebuffer, sizeof(char), 2056, fp);
		send (sock, filebuffer, (sizeof (char) * wasRead), 0);
	}
	fclose(fp);
	}
}
void parser2(char *str)
{
	printf("str is %s\n",str);
	int pid;
	pid=fork();
	if(pid==0)
	{
	puts("now what");
		execlp("./21.sh","./21.sh",str,NULL);
	}
	else
	{
		int xx;
		wait(&xx);
		FILE *fp;
		FILE *fp1;
		fp=fopen("intermediate","r");
		fp1=fopen("final21","a");
		char line[200];
		//fgets (line, sizeof line, fp);
		char buf[200];
	while(fgets (line, sizeof line, fp)!=NULL)
	{
		if(line[0]-'t'==0&&line[1]-'o'==0&&line[2]-'t'==0&&line[3]-'a'==0&&line[4]-'l'==0)
		{
			continue;
		}
		puts("hey");
		char * temp=strtok(line," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		char* size =strtok(NULL," ");
		char * month=strtok(NULL," ");
		char * date=strtok(NULL," ");
		char * time=strtok(NULL," ");
		char* file = strtok(NULL," ");
				file[strlen(file)-1]='\0';

if(strcmp(file,str)==0)
{
printf("comparing %s, %s\n",str,file);
		fputs(date,fp1);
		fputs(" ",fp1);
		fputs(month,fp1);
		fputs(" ",fp1);
		fputs(time,fp1);
		fputs(" ",fp1);
	break;
}
}
		fclose(fp);
		fclose(fp1);
	}
}
void parser1()
{
	FILE *fp;
	FILE *fp1;
	fp=fopen("intermediate","r");
	fp1=fopen("final","w");
	char line[200];

	fgets (line, sizeof line, fp);
	char buf[200];

	while(fgets (line, sizeof line, fp)!=NULL)
	{
		//strcpy(line,buf);
		//printf("%s \n",line,buf);
		//puts("reading");
		char * temp=strtok(line," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		char* size =strtok(NULL," ");
		char * month=strtok(NULL," ");
		char * date=strtok(NULL," ");
		char * time=strtok(NULL," ");
		char* file = strtok(NULL," ");
		fputs(date,fp1);
		fputs(" ",fp1);
		fputs(month,fp1);
		fputs(" ",fp1);
		fputs(size,fp1);
		fputs(" ",fp1);
		fputs(time,fp1);
		fputs(" ",fp1);
		fputs(file,fp1);
	}
	fclose(fp);
	fclose(fp1);


}
void parser(char *month1,char *date1, char *t1,char *month2,char * date2, char* t2)
{
	FILE *fp;
	FILE *fp1;
	fp=fopen("intermediate","r");
	fp1=fopen("final","w");
	char line[200];

	fgets (line, sizeof line, fp);
	char buf[200];

	while(fgets (line, sizeof line, fp)!=NULL)
	{
		//strcpy(line,buf);
		//printf("%s \n",line,buf);
		//puts("reading");
		char * temp=strtok(line," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		temp=strtok(NULL," ");
		char* size =strtok(NULL," ");
		char * month=strtok(NULL," ");
		char * date=strtok(NULL," ");
		char * time=strtok(NULL," ");
		char* file = strtok(NULL," ");
		if((int)*month >= (int)*month1 && (int)*month <=(int)* month2)
		{
			//puts("1");
			if((int)*month == (int)*month1 && (int)*month ==(int)* month2)
			{
				//puts("2");

				if((int)*date >= (int)*date1 && (int)*date<=(int)*date2)
				{
					//puts("3");
					if((int)*date == (int)*date1 && (int)*date==(int)*date2)
					{

						if((int)t1[0]<(int)time[0] && (int)t2[0]> (int)time[0])
						{
							if((int)t1[1]<(int)time[1] && (int)t2[1]> (int)time[1]){
								if((int)t1[3]<(int)time[3] && (int)t2[3]> (int)time[3]){
									if((int)t1[4]<(int)time[4] && (int)t2[4]> (int)time[4]){
										fputs(date,fp1);
										fputs(" ",fp1);
										fputs(month,fp1);
										fputs(" ",fp1);
					fputs(size,fp1);
					fputs(" ",fp1);
					fputs(time,fp1);
					fputs(" ",fp1);
					fputs(file,fp1);
				}
				}
				}
				}
			}
		else
		{
					fputs(date,fp1);
					fputs(" ",fp1);
					fputs(month,fp1);
					fputs(" ",fp1);
					fputs(size,fp1);
					fputs(" ",fp1);
					fputs(time,fp1);
					fputs(" ",fp1);
					fputs(file,fp1);
		}
			}
		}
		else
		{
					fputs(date,fp1);
					fputs(" ",fp1);
					fputs(month,fp1);
					fputs(" ",fp1);
					fputs(size,fp1);
					fputs(" ",fp1);
					fputs(time,fp1);
					fputs(" ",fp1);
					fputs(file,fp1);
		}
		}

	}
	fclose(fp1);
	fclose(fp);
}
