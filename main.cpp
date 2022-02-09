//
//  main.cpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#include "builtin.hpp"
#include "pipe.hpp"

#include <iostream>
#include <sstream>
#include <filesystem>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

void printCurDir();
void readLine(std::string& cmd);
void getArgs(std::string& cmd, std::vector<char*>& args);
int cmdFlag(std::string& cmd, std::vector<char*>& args);
bool execute(std::vector<char*> args);
bool execute2();
bool execute3();

int main(){
    bool status = true;

    while(status){
        std::string cmd;
        std::vector<char*> args;

        printCurDir();
        readLine(cmd);
        switch (cmdFlag(cmd, args)){
            case 0:
                break;
            case 1:
                status = builtin::execute(args);
                break;
            case 2:
                status = execute(args);
                break;
            case 3: {
                Pipe po;
                po.getPipeCmds(cmd);
                status = po.execute();
                break;
            }
            default:
                break;
        }
    }
        
//    Pipe p;
//    p.test();
    
    return 0;
}

void printCurDir(){
    std::cout << std::filesystem::current_path() << " % ";
}

void readLine(std::string& cmd){
    std::getline(std::cin, cmd);
}

void getArgs(std::string& cmd, std::vector<char*>& args){
    if(!cmd.empty()){
        std::istringstream ss(cmd);
        std::string str;
        while(ss >> str){
            char* c = new char(str.length()+1);
            strcpy(c, str.c_str());
            args.push_back(c);
        }
        args.push_back(nullptr);
    }
}

int cmdFlag(std::string& cmd, std::vector<char*>& args){
    if(cmd.empty()) return 0;
    if(cmd.find("|") == -1){
        getArgs(cmd, args);
        if(builtin::isBuiltinCmd(args[0])) return 1; // builtin cmd
        else return 2; // regular cmd
    }
    else return 3; // pipe commands
}

bool execute(std::vector<char*> args){
    int status;
    pid_t pid = fork(), wpid;
    
    if(pid == -1){ // fail to fork
        std::cout << "\nFail to fork.\n";
    }
    else if(pid == 0){ // child process
        char** argArr = &args[0];
        if(execvp(argArr[0], argArr) < 0){ // let the child process do the command
            std::cout << "\nFail to execute the command.\n";
            exit(EXIT_FAILURE); // kill this child process
        }
    }
    else{ // parent process
        //  [NOTE]
        //  Technically, the last term has 3 options: wait until the child is returned, return no matter what, return at once if the child process is paused for whichever reason.
        wpid = waitpid(pid, &status, 0); // basically the same as wait
    }
    
    return true;
}

//bool execute2(){
//    int status;
//
//    pid_t pid = fork(), wpid;
//
//    if(pid == -1){ // fail to fork
//        std::cout << "\nFail to fork.\n";
//    }
//    else if(pid == 0){ // child process
//        system ("/bin/stty raw");
//
//        char c;
////        do{
//            c = getchar();
////        } while(c != 'c');
//
//        system ("/bin/stty cooked");
//
//        std::cout << "XXXX" << std::endl;
//        exit(0);
//    }
//    else{ // parent process
//        do{
//            std::cout << "wait " << std::endl;
//            wpid = waitpid(pid, &status, 0);
//            std::cout << "end " << std::endl;
//        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
//
////        std::cout << "x = " << *x_ptr << std::endl;
//    }
//
//    return true;
//}
//
//bool execute3(){
//    int keyboard;
//    int ret;
//    char c;
//    fd_set readfd;
//    struct timeval timeout;
//    keyboard = open("/dev/tty", O_RDONLY | O_NONBLOCK);
//    assert(keyboard > 0);
//    while (1)
//    {
//        timeout.tv_sec = 1;
//        timeout.tv_usec = 0;
//        FD_ZERO(&readfd);
//        FD_SET(keyboard, &readfd);
//        ret = select(keyboard + 1, &readfd, NULL, NULL, &timeout);
//        if (FD_ISSET(keyboard, &readfd))
//        {
//            read(keyboard, &c, 1);
//            std::cout << "Input is " << c << "\n";
//            if (c == 'c')
//                break;
//        }
//    }
//
//    return true;
//}
