#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>

#define PATH1 "/msg_exchange_12"
#define PATH2 "/msg_exchange_21"
#define MSG_MAX 256
#define FINISH_PRIO 15

void ParticipantFirst(struct mq_attr config)
{
    mqd_t send_fd, recv_fd;
    char text_buffer[MSG_MAX];
    unsigned int priority_val;

    mq_unlink(PATH1);
    mq_unlink(PATH2);

    send_fd = mq_open(PATH1, O_CREAT | O_WRONLY, 0666, &config);
    if (send_fd == (mqd_t)-1) 
    { 
        perror("mq_open send"); 
        exit(1); 
    }

    recv_fd = mq_open(PATH2, O_CREAT | O_RDONLY, 0666, &config);
    if (recv_fd == (mqd_t)-1) 
    { 
        perror("mq_open recv"); 
        exit(1); 
    }

    while (1) 
    {
        printf("First: ");
        if (!fgets(text_buffer, MSG_MAX, stdin)) 
        {
            break;
        }

        priority_val = FINISH_PRIO;
        if (strcmp(text_buffer, "quit\n") == 0) 
        {
            priority_val = FINISH_PRIO;
        } 
        else 
        {
            priority_val = 8;
        }

        if (mq_send(send_fd, text_buffer, strlen(text_buffer) + 1, priority_val) == -1) 
        {
            perror("mq_send");
            break;
        }
        
        if (priority_val == FINISH_PRIO) 
        {
            break;
        }
        
        if (mq_receive(recv_fd, text_buffer, MSG_MAX, &priority_val) == -1) 
        {
            perror("mq_receive");
            break;
        }

        printf("Second: %s\n", text_buffer);
        if (priority_val == FINISH_PRIO) 
        {
            break;
        }
    }

    mq_close(send_fd);
    mq_close(recv_fd);
    mq_unlink(PATH1);
    mq_unlink(PATH2);
}

int main() 
{
    struct mq_attr config = {0, 10, MSG_MAX, 0};
    printf("Chat participant 1 started\n"); 
    ParticipantFirst(config);
    printf("Chat ended\n");
    return 0;
}