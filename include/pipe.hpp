//
//  pipe.hpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#ifndef pipe_hpp
#define pipe_hpp

#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <iostream>

class Pipe {
public:
    Pipe(){}
    ~Pipe(){}
    void getPipeCmds(std::string const& cmd);
    bool execute();
private:
    std::vector<char*> getArgs(char*& cmd);
    
    std::vector<char*> cmds_;
};

#endif /* pipe_hpp */
