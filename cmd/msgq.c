//
// Created by millefeuille on 9/18/23.
//

#include "lemipc.h"

struct msg_buf {
	long mtype;
	char mtext[sizeof(t_vec) + 1];
};

int send_message_to_team(int qid, const t_vec *const message, unsigned int team) {
	errno = 0;
	struct msg_buf buf = {0};
	buf.mtype = team;
	char *message_as_string = (char *)message;
	for (size_t i = 0; i < sizeof(t_vec); i++) {
		buf.mtext[i] = message_as_string[i];
	}
	return msgsnd(qid, &buf, sizeof(buf), IPC_NOWAIT);
}

int read_message_for_team(int qid, const t_vec *message, unsigned int team) {
	errno = 0;
	struct msg_buf buf = {0};
	msgrcv(qid, &buf, sizeof(buf), team, IPC_NOWAIT);
	if (errno) return errno;
	char *message_as_string = (char *)message;
	for (size_t i = 0; i < sizeof(t_vec); i++) {
		message_as_string[i] = buf.mtext[i];
	}
	return 0;
}

void delete_msgq(int qid) {
	errno = 0;
	msgctl(qid, IPC_RMID, NULL);
}

int get_msgq(int key) {
	errno = 0;
	return msgget(key, IPC_CREAT | 0666);
}
