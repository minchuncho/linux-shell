//
//  pipe.cpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#include "pipe.hpp"

void Pipe::getPipeCmds(std::string const& cmd){
    std::string del="|";
    int start = 0, end = (int)cmd.find(del, start);
    while(end != -1){
        std::string str = cmd.substr(start, end-start);
        char* c = new char(str.length()+1);
        strcpy(c, str.c_str());
        cmds_.push_back(c);
        start = end+1;
        end = (int)cmd.find(del, start);
    }
    std::string str = cmd.substr(start, end-start);
    char* c = new char(str.length()+1);
    strcpy(c, str.c_str());
    cmds_.push_back(c);
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
                close(fd[pipe_in]);
            }
            if(i != n-1){
                if(dup2(fd[pipe_out], STDOUT_FILENO) < 0){
                    exit(EXIT_FAILURE);
                }
                close(fd[pipe_out]);
            }
            
            std::vector<char*> args = getArgs(cmds_[i]);
            if(execvp(*&args[0], &args[0]) < 0){
                std::cout << "\nFail to execute the command.\n";
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
        else{
            wpid = waitpid(pids[i], &status, 0);
            if(i != 0){
                close(fd[pipe_in]);
            }
            if(i != n-1){
                close(fd[pipe_out]);
            }
        }
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

void Pipe::test(){
    int fd[2] = {0}, status;
    pid_t pid = fork();
    
    if(pid == -1){ // fail to fork
        std::cout << "\nFail to fork.\n";
    }
    else if(pid == 0){ // child process
        fd[0] = 10;
    }
    else{
        waitpid(pid, &status, 0);
        std::cout << fd[0] << std::endl;
    }
}
