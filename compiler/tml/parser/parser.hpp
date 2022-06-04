#ifndef __TML_PARSER_HPP__
#define __TML_PARSER_HPP__

#include "tml/ast/ast.hpp"
#include "errors/error.hpp"
#include "tml/lexer/lexer.hpp"
#include <map>
#include <string>
#include <vector>
namespace tml{
namespace parser{

class Parser {
  private:
    size_t m_tokIndex{0};
    lexer::Token m_currentToken;
    std::vector<lexer::Token> m_tokens;
    std::string m_filename;
    void advance();
    void advanceOnNewLine();
    void expect(lexer::TokenType expectedType, std::string msg="",std::string submsg="",std::string hint="",std::string ecode="");
    lexer::Token next();
    void error(lexer::Token tok, std::string msg,std::string submsg="",std::string hint="",std::string ecode="");

    ast::AstNodePtr parseStatement();
    ast::AstNodePtr parseLua();
    ast::AstNodePtr parseContent();
    ast::AstNodePtr parseStyle();
    ast::AstNodePtr parseTitle();
    ast::AstNodePtr parseInput();

    ast::AstNodePtr parseArgument();
  public:
    Parser(const std::vector<lexer::Token>& tokens,std::string filename);
    ~Parser();
    ast::AstNodePtr parse();
};
}
}
#endif
