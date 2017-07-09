#include<stdio.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<errno.h>
#include<time.h>

void msg_stat(int msgid)
{
	struct msqid_ds msg_info;
	int reval;
	sleep(1);
	reval = msgctl(msgid, IPC_STAT, &msg_info);
	if(reval == -1)
	{
		printf("get msg info error\n");
		return;
	}
	printf("\n");
	printf("msg_cbytes = %d\n", msg_info.msg_cbytes);
	printf("msg_qnum = %d\n", msg_info.msg_qnum);
	printf("msg_qbytes = %d\n", msg_info.msg_qbytes);
	printf("msg_lspid = %d\n", msg_info.msg_lspid);
	printf("msg_lrpid = %d\n", msg_info.msg_lrpid);
	printf("msg_stime = %s\n", ctime(&(msg_info.msg_stime)));
	printf("msg_rtime = %s\n", ctime(&(msg_info.msg_rtime)));
	printf("msg_ctime = %s\n", ctime(&(msg_info.msg_ctime)));
	printf("msg_uid = %d\n", msg_info.msg_perm.uid);
	printf("msg_gid = %d\n", msg_info.msg_perm.gid);
	printf("msg_key = %d\n", msg_info.msg_perm.__key);
}

int main(void)
{
	int gflags, sflags, rflags;
	key_t key;
	int msgid;
	int reval;
	struct msgsbuf
	{
		long mtype;
		char mtext[1];
	} msg_sbuf;
	
	struct msgrbuf
	{
		long mtype;
		char mtext[10];
	} msg_rbuf = {'\0'};
	
	struct msqid_ds msg_ginfo, msg_sinfo;
	char *msgpath="msgqueue";
	key=ftok(msgpath, 'a');
	printf("key = %d\n", key);
	gflags=IPC_CREAT;
	msgid = msgget(key, gflags|0666);
	if (msgid == -1)
	{
		perror("msg create error");
		return;
	}
	msg_stat(msgid);
	
	sflags=IPC_NOWAIT;
	msg_sbuf.mtype=10;
	msg_sbuf.mtext[0] = 'a';
	reval = msgsnd(msgid, &msg_sbuf, sizeof(msg_sbuf.mtext), sflags);
	if(reval == -1)
	{
		perror("message send error");
	}
	msg_stat(msgid);

	rflags=IPC_NOWAIT|MSG_NOERROR;
	reval = msgrcv(msgid, &msg_rbuf, 4, 10, rflags);
	
	if(reval == -1)
	{
		perror("message recv error");
	}
	else
	{
		printf("read from msg queue %d bytes\n value = %s type = %d", reval, msg_rbuf.mtext, msg_rbuf.mtype);
	}
	msg_stat(msgid);
	return 0;
}
