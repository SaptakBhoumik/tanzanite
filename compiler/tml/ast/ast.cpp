#include "tml/ast/ast.hpp"

#include <iostream>
#include <map>

namespace tml{
namespace ast{
Program::Program(std::vector<AstNodePtr> statements){
    m_statements=statements;
}
std::vector<AstNodePtr> Program::statements() const{
    return m_statements;
}
lexer::Token Program::token() const{
    return lexer::Token{};
}
AstKind Program::type() const{
    return KAstProgram;
}
std::string Program::stringify() const{
    std::string res = "";

    for (auto& stmt : m_statements) {
        res += stmt->stringify();
        res += "\n";
    }

    return res;
}
void Program::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}

Section::Section(lexer::Token token,std::vector<AstNodePtr> args){
    m_args=args;
    m_token=token;
}
std::vector<AstNodePtr> Section::args() const{
    return m_args;
}
lexer::Token Section::token() const{
    return m_token;
}
AstKind Section::type() const{
    return KAstSection;
}
std::string Section::stringify() const{
    std::string res = m_token.keyword+":\n";
    for (auto& arg : m_args) {
        res += "\t"+arg->stringify()+"\n";
    }
    return res;
}
void Section::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}


Argument::Argument(lexer::Token token,std::string args){
    m_args.first=token.tkType;
    m_args.second=args;
    m_token=token;
}
std::pair<lexer::TokenType,std::string> Argument::args() const{
    return m_args;
}
lexer::Token Argument::token() const{
    return m_token;
}
AstKind Argument::type() const{
    return KAstArguement;
}
std::string Argument::stringify() const{
    using namespace lexer;
    std::map<TokenType,std::string> lex_name={
        {tk_id,"id"},
        {tk_text,"text"},
        {tk_path,"path"},
        {tk_type,"type"},
    };
    return lex_name[m_args.first]+":"+m_args.second;
}
void Argument::accept(AstVisitor& visitor) const {
    visitor.visit(*this);
}
}
}