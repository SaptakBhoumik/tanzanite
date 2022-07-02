#include "codegen.hpp"
#include <iostream>

#define get_instruction(case_key, ins)  case lexer::tk_##case_key:{\
                                            instruction=opt_code::OPT_##ins;\
                                            break;\
                                        }



#define get_arg(case_key, arg)  case lexer::tk_##case_key:{\
                                    arg_type=opt_code::ARG_##arg;\
                                    break;\
                                }\

namespace tml{
namespace codegen{
Codegen::Codegen(ast::AstNodePtr ast){
    ast->accept(*this);
}
bool Codegen::visit(const ast::Program& node){
    for (auto& stmt : node.statements()) {
        m_code.push_back(section_gen(std::dynamic_pointer_cast<ast::Section>(stmt)));
    }
    return true;
}
std::vector<tml_vm::opt> Codegen::get_code(){
    return m_code;
}
tml_vm::opt Codegen::section_gen(std::shared_ptr<ast::Section> node){
    auto token=node->token();
    opt_code::optcode instruction;
    switch (token.tkType){
        get_instruction(lua, LUA);
        get_instruction(content, CONTENT);
        get_instruction(style, STYLE);
        get_instruction(title, TITLE);
        get_instruction(input, INPUT);
        get_instruction(embed, EMBED);
        get_instruction(link, LINK);
        default:{}
    };
    std::vector<tml_vm::code_elm> args;
    std::vector<tml_vm::opt> nested_elm;
    for (auto arg : node->args()) {
        if(arg==NULL) continue;
        else if(arg->type() == ast::KAstArguement){
            auto argument=std::dynamic_pointer_cast<ast::Argument>(arg);
            args.push_back(arg_gen(argument));
        }
        else{
            auto section=std::dynamic_pointer_cast<ast::Section>(arg);
            nested_elm.push_back(section_gen(section));
        }
    }
    if(nested_elm.size()>0){
        std::cout<<nested_elm.size()<<std::endl;
        args.push_back(tml_vm::code_elm(nested_elm));
    }
    return tml_vm::opt(instruction, args);
}
tml_vm::code_elm Codegen::arg_gen(std::shared_ptr<ast::Argument> node){
    opt_code::argtype arg_type;
    switch(node->token().tkType) {
        get_arg(id, ID);
        get_arg(text, TEXT);
        get_arg(type, TYPE);
        get_arg(path, PATH);
        default:{}
    }
    auto arg=node->args().second;
    if(arg_type==opt_code::ARG_TYPE){
        return tml_vm::code_elm(arg_type, tml_vm::code_elm(opt_code::type_of_element_map[arg]));
    }
    return tml_vm::code_elm(arg_type, tml_vm::code_elm(arg));
}
}
}