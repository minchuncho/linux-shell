//
//  pipe.cpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#include "pipe.hpp"

void Pipe::getPipeCmds(std::string const& cmd){
    std::string del="|";
    std::vector<int> del_pos;
    int start = 0, end = (int)cmd.find(del, start);

    while(end != -1){
        del_pos.push_back(end);
        start = end+1;
        end = (int)cmd.find(del, start);
    }
    del_pos.push_back(cmd.length());
    start = 0;

    for(int i=0; i<del_pos.size(); i++){
        std::string str = cmd.substr(start, del_pos[i]-start);
        char* c = new char(str.length()+1);
        strcpy(c, str.c_str());
        cmds_.push_back(c);
        start = del_pos[i]+1;
    }
}

bool Pipe::execute(){
    int n = (int)cmds_.size(), fd_num = (n-1)*2, fd[fd_num], status;
    pid_t pids[n], wpid;

    for(int i=0; i<n; i++){
        const int pipe_in = (i-1)*2, pipe_out = 2*i+1;
        
        if(i != n-1 && pipe(fd + 2*i) < 0){
            std::cout << "Pipe cannot be initialized.\n";
            return true;
        }
        
        pids[i] = fork();
        
        if(pids[i] == -1){ // fail to fork
            std::cout << "\nFail to fork.\n";
        }
        else if(pids[i] == 0){ // child process
            if(i != 0){
                if(dup2(fd[pipe_in], STDIN_FILENO) < 0){
                    exit(EXIT_FAILURE);
                }
            }
            if(i != n-1){
                if(dup2(fd[pipe_out], STDOUT_FILENO) < 0){
                    exit(EXIT_FAILURE);
                }
            }
            
            std::vector<char*> args = getArgs(cmds_[i]);
            if(execvp(*&args[0], &args[0]) < 0){
                std::cout << "\nFail to execute the command.\n";
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else{
            /*  NOTE:
                This way, the parent process's stdin and stdout is unchanged.
                Tho processes share filedescriptors thru fork(), if you change stdin and stdout before entering a child process,  enevtually the stdout would be trapped in fd[2n-3].
                Hence, the last command's output would point to fd[2n-3] instead of stdout. That's why it gets stuck.
             */
//            wpid = waitpid(pids[i], &status, 0);
//            if(i != 0){
//                close(fd[pipe_in]);
//            }
//            if(i != n-1){
//                close(fd[pipe_out]);
//            }
            if(i != 0){
                close(fd[pipe_in]);
            }
            if(i != n-1){
                close(fd[pipe_out]);
            }
        }
    }
    
    /*  NOTE:
        Do not wait for a specific process to complete, instead, the parent should skip it and carry on to the next child process.
        The reason os that while the previous child's output are large data and pipe cannot buffer them all at once, if the next process has not started, the pipe can explode.
     */
    for(int i=0; i<n; i++){
        wpid = waitpid(pids[i], &status, 0);
    }

    return true;
}

std::vector<char*> Pipe::getArgs(char*& cmd){
    std::vector<char*> args;
    char *token = strtok(cmd, " ");
    while (token != NULL)
    {
        args.push_back(token);
        token = strtok(NULL, " ");
    }
    args.push_back(nullptr);
    return args;
}
