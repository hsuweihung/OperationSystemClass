#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void show(int fork_id) {
    printf("Fork %d. I'm the child %d, my parent is %d.\n", fork_id, getpid(), getppid());
}

int main() {
    printf("Main Process ID: %d\n\n", getpid());
    
    pid_t pid;
    
    pid = fork(); // 2772
    if (pid == 0) {
        show(1);
        pid = fork(); //2773
        if (pid == 0) {
            show(2);
        }
        else {
            wait(NULL);//等2773結束
            pid=fork();//2772 -> 2774
            if(pid==0){
                show(3);
                pid=fork();//2775
                if(pid==0){
                    show(4);
                }
                else{
                    wait(NULL);//等2775結束
                }
            }
            else{
                wait(NULL);//等原先 2772 結束
                pid=fork();//2776
                if(pid==0){
                    show(4);
                }
                else{
                    wait(NULL);//等2772 -> 2776 結束
                }
            }
        }
    }
    
    else if(pid>0){
        wait(NULL);
        pid=fork();//2777
        if (pid==0){
            show(2);
        }
        else{
            wait(NULL);
            pid=fork();//2778
            if(pid==0){
                show(3);
                pid=fork();//2779
                if(pid==0){
                    show(4);
                }
                else{
                    wait(NULL);
                }
            }
            else{
                wait(NULL); //等 2778 -> 2779 結束
                pid=fork();
                if (pid==0){
                    show(4);
                }
                else{
                    wait(NULL);//等2780 結束
                }
            }
        }
    }
    return 0;
}
