#include "codegen.hpp"

namespace tml{
namespace Codegen{
Codegen::Codegen(ast::AstNodePtr ast){
    ast->accept(*this);
}
bool Codegen::visit(const ast::Program& node){
    for (auto& stmt : node.statements()) {
        stmt->accept(*this);
    }
    return true;
}
tml_vm::opt Codegen::section_gen(std::shared_ptr<ast::Section> node){
    auto token=node->token();
    tml_vm::opt instruction;
    switch (token.tkType){
        case lexer::tk_lua:{
            instruction=tml_vm::opt{opt_code::OPT_LUA};
            break;
        }
        case lexer::tk_content:{
            instruction=tml_vm::opt{opt_code::OPT_CONTENT};
            break;
        }
        case lexer::tk_style:{
            instruction=tml_vm::opt{opt_code::OPT_STYLE};
            break;
        }
        case lexer::tk_title:{
            instruction=tml_vm::opt{opt_code::OPT_TITLE};
            break;
        }
        case lexer::tk_input:{
            instruction=tml_vm::opt{opt_code::OPT_INPUT};
            break;
        }
        case lexer::tk_embed:{
            instruction=tml_vm::opt{opt_code::OPT_EMBED};
            break;
        }
        case lexer::tk_link:{
            instruction=tml_vm::opt{opt_code::OPT_LINK};
            break;
        }
        default:{}
    };
    std::vector<tml_vm::code_elm> args;
    std::vector<tml_vm::opt> nested_elm;
    for (auto &arg : node->args()) {
        if(arg->type() == ast::KAstArguement){
            auto argument=std::dynamic_pointer_cast<ast::Argument>(arg);
            args.push_back(arg_gen(argument));
        }
        else{
            auto section=std::dynamic_pointer_cast<ast::Section>(arg);
            nested_elm.push_back(section_gen(section));
        }
    }
}
}
}