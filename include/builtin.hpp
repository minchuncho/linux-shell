//
//  builtin.hpp
//  shell_2
//
//  Created by Cho MinChun on 2021/12/17.
//

#ifndef builtin_hpp
#define builtin_hpp

#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>

namespace builtin {
    bool isBuiltinCmd(char* const& args_0);
    bool execute(std::vector<char*> const& args);
}

#endif /* builtin_hpp */
