#ifndef __TML_AST_HPP__
#define __TML_AST_HPP__
#include "tml/lexer/tokens.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
namespace tml{
namespace ast{
class AstVisitor;

//Defines the type of ast;
enum AstKind {
    KAstProgram,
    KAstSection,
    KAstArguement,
};
//base class
class AstNode {
  public:
    virtual ~AstNode() = default;

    virtual lexer::Token token() const = 0;
    virtual AstKind type() const = 0;
    virtual std::string stringify() const = 0;
    virtual void accept(AstVisitor& visitor) const = 0;
};

using AstNodePtr = std::shared_ptr<AstNode>;

class Program : public AstNode {
    std::vector<AstNodePtr> m_statements;
  public:
    Program(std::vector<AstNodePtr> statements);
    std::vector<AstNodePtr> statements() const;

    lexer::Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};
class Section : public AstNode {
    lexer::Token m_token;
    std::vector<AstNodePtr> m_args;
  public:
    Section(lexer::Token token,std::vector<AstNodePtr> args);
    std::vector<AstNodePtr> args() const;
    
    lexer::Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
}; 
class Argument: public AstNode {
    lexer::Token m_token;
    std::pair<lexer::TokenType,std::string> m_args;
  public:
    Argument(lexer::Token token,std::string args);
    std::pair<lexer::TokenType,std::string> args() const;
    
    lexer::Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
};

class AstVisitor {
  public:
    virtual ~AstVisitor() = default;

    virtual bool visit(const Program& node) { return false; };
    virtual bool visit(const Section& node) { return false; };
    virtual bool visit(const Argument& node) { return false; };
};
} // namespace ast
} // namespace tml
#endif