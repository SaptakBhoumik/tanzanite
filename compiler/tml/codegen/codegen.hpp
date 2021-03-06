#ifndef __TML_CODEGEN_HPP__
#define __TML_CODEGEN_HPP__
#include <string>
#include <vector>
#include "tml/ast/ast.hpp"
#include "tml/vm_opt/opt_code.hpp"
namespace tml{
namespace codegen{
    class Codegen:public ast::AstVisitor{
        public:
        Codegen(ast::AstNodePtr ast);
        std::vector<tml_vm::opt> get_code();
        private:
        std::vector<tml_vm::opt> m_code;
        bool visit(const ast::Program& node); 
        tml_vm::opt section_gen(std::shared_ptr<ast::Section> node);
        tml_vm::code_elm arg_gen(std::shared_ptr<ast::Argument> node);
    };
}
}
#endif