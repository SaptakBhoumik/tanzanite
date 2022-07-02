#ifndef __TML_DIS_HPP__
#define __TML_DIS_HPP__
#include "tml/vm_opt/opt_code.hpp"
namespace tml{
namespace dis{
    std::string disassemble(const std::vector<tml_vm::opt>& code,std::string padding="");
}
}
#endif