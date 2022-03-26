//
//  builtin.cpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#include "builtin.hpp"

namespace builtin{
    static std::vector<std::string> builtinCmds {"cd", "exit", "help"};

    static bool exeCd(char* const& args_1);
    static bool exeExit();
    static bool exeHelp();

    bool isBuiltinCmd(char* const& args_0){
        int n = (int)builtinCmds.size();
        for (int i=0; i<n; i++) {
            if(strcmp(args_0, builtinCmds[i].c_str()) == 0){
                return true;
            }
        }
        return false;
    }

    bool execute(std::vector<char*> const& args){
        int n = (int)builtinCmds.size();
        for (int i=0; i<n; i++) {
            if(strcmp(args[0], builtinCmds[i].c_str()) == 0){
                switch (i) {
                    case 0:
                        return exeCd(args[1]);
                    case 1:
                        return exeExit();
                    case 2:
                        return exeHelp();
                    default:
                        break;
                }
            }
        }
        return true;
    }

    static bool exeCd(char* const& args_1){
        if(args_1 == nullptr){
            std::cout << "cd requires arguments.\n";
        }
        else{
            if(chdir(args_1) != 0){
                std::cout << "change directory error.\n";
            }
        }

        return true;
    }

    static bool exeExit(){
        return false;
    }

    static bool exeHelp(){
        std::cout << "The following commands are builtins:\n";
        for(auto const &bc : builtinCmds){
            std::cout << "* " << bc << std::endl;
        }

        return true;
    }
}
