/* message2.c */

msg_T *msg2_create(msgPriority_T priority);
char_u *msg2_get_contents(msg_T *msg);
void msg2_set_title(char_u *title, msg_T *msg);
void msg2_put(char_u *s, msg_T *msg);
void msg2_send(msg_T *msg);
void msg2_source(msg_T *msg);
void msg2_free(msg_T *msg);

/* vim: set ft=c : */
