#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

#include "my_scanf.h"

#include "Face.h"
#include "Start.h"

Face * Face::Face_ = NULL;

Face * Face::GetFace()
{
	if(Face_ == NULL)
	{
		Face_ = new Face();
	}

	return Face_;
}
void Face::FreeFace()
{
	if(Face_ != NULL)
	{
		delete Face_;
		Face_ = NULL;
	}
}


Face::Face()
{
	scan = MyScanf::GetScanf();
}

int Face::Function(int client_sock)
{
    char etc[BUFF_SIZE];
	char *msg;
	time_t timep;
	char *time_save;
	char buffer[BUFF_SIZE];
	char name[ACC_SIZE];
	sqlite3 *db;

	memset(etc,0,sizeof(etc));
	sprintf(etc,"etc/_%s.db",id);

	int flag = 0;
	Node user;
	read(fd[0],&user,sizeof(user));
	if(user.chat_flag == 0)
	{
	    printf("		您已被禁言!\n");
		return 0;
	}

    printf("请输入对方昵称:");
	strcpy(name,user.name);
	scanf("%s",user.name);
	write(client_sock,&user,sizeof(user));
	read(fd[0],&user,sizeof(user));
	if(user.sock == -1)
	{
	    printf("		无联系人!\n");
		return 0;
	}
	else
	{
		while(flag == 0)
		{
			printf("		1.(^_^) 	2.。(*_*)。\n");
	    	printf("需要发送的表情编号:");
			scanf("%d",&flag);
			switch(flag)
			{
			    case 1:
					strcpy(user.buffer,"(^_^)");
					break;

				case 2:
					strcpy(user.buffer,"。(*_*)。");
					break;

				default:
					printf("		没有该表情!\n");
					flag = 0;
					break;
				
			}
		}
		user.action = 0;
		write(client_sock,&user,sizeof(user));


		time(&timep);
		time_save = ctime(&timep);
		memset(buffer,0,sizeof(buffer));
		sqlite3_open(etc,&db);
		sprintf(buffer,"create table _%s(time text, name text, buffer text, flag text)",id);
		sqlite3_exec(db,buffer,NULL,NULL,&msg);
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"insert into _%s(time,name,buffer,flag) values ('%s','%s','%s','1')",id, time_save,name,user.buffer);
		sqlite3_exec(db,buffer,NULL,NULL,&msg);
		sqlite3_close(db);


		return 0;
	}
}

int Face::Function() {}
