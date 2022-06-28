#ifndef __tss_AST_HPP__
#define __tss_AST_HPP__
#include "tss/lexer/tokens.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
namespace tss{
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
    std::string m_name;
    lexer::TokenType m_id_type;
    std::vector<AstNodePtr> m_args;
  public:
    Section(lexer::Token token,std::string name,lexer::TokenType id_type,std::vector<AstNodePtr> args);
    std::vector<AstNodePtr> args() const;
    lexer::TokenType id_type() const;
    std::string name() const;

    lexer::Token token() const;
    AstKind type() const;
    std::string stringify() const;
    void accept(AstVisitor& visitor) const;
}; 
class Argument: public AstNode {
    lexer::Token m_token;
    std::pair<std::string,std::string> m_args;
  public:
    Argument(lexer::Token token,std::string field,std::string args);
    std::pair<std::string,std::string> args() const;
    
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
} // namespace tss
#endif