#include "parser.hpp"

#include "parser.hpp"
#include <iostream>
#include <algorithm>

namespace tss{
namespace parser{
using namespace lexer;
using namespace ast;

void Parser::advance() {
    //go to next token
    m_tokIndex++;

    if (m_tokIndex < m_tokens.size()) {
        m_currentToken = m_tokens[m_tokIndex];
    }
}

void Parser::advanceOnNewLine() {
    //go to next line
    if (next().tkType == tk_new_line) {
        advance();
    }
}

Token Parser::next() {
    //check the next token
    Token token;

    if (m_tokIndex + 1 < m_tokens.size()) {
        token = m_tokens[m_tokIndex + 1];
    }

    return token;
}
void Parser::error(Token tok, std::string msg,std::string submsg,std::string hint,std::string ecode) {
    //display error
    PEError err = {{tok.line, tok.location,tok.location, m_filename, tok.statement},
                   std::string(msg),
                   submsg,
                   hint,
                   ecode};

    display(err);
    exit(1);
}

void Parser::expect(TokenType expectedType, std::string msg,std::string submsg,std::string hint,std::string ecode) {
    //check if the next toke is what we expect or else show error
    if (next().tkType != expectedType) {
        if(msg==""){
            msg="expected token of type " + std::to_string(expectedType) +", got " + std::to_string(next().tkType) + " instead";
        }
        if(next().tkType==tk_new_line){
            error(m_currentToken,msg,submsg,hint,ecode);
        }
        else{
            error(next(),msg,submsg,hint,ecode);
        }
    }
    advance();
}

Parser::~Parser() {}

Parser::Parser(const std::vector<Token>& tokens,std::string filename) : m_tokens(tokens) {
    //initializer of parser class
    m_currentToken = tokens[0];
    m_filename=filename;
}

AstNodePtr Parser::parse() {
    //start parsing
    std::vector<AstNodePtr> statements;
    while (m_currentToken.tkType != tk_eof) {
        statements.push_back(parseStatement());
        if(m_currentToken.tkType!=tk_new_line && m_currentToken.tkType!=tk_dedent){
            error(m_currentToken,"Expected newline after statement");
        }
        advance();
    }
    return std::make_shared<Program>(statements);
}

AstNodePtr Parser::parseStatement() {
    //statements
    AstNodePtr stmt;

    switch (m_currentToken.tkType) {
        case tk_id:{
            stmt = parseStyle();
            break;
        }      
        default:{
            error(m_currentToken,"Unexpected token "+m_currentToken.keyword);
        }
    }
    return stmt;
}
AstNodePtr Parser::parseArgument(){
    auto tok = m_currentToken;
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_string,"Expected a string but got "+next().keyword+" instead","Add a string here","","");//on `string`
    auto str=m_currentToken.keyword;
    expect(tk_new_line,"Expected a newline but got "+next().keyword+" instead","Add a newline here","","");//on `\n`
    return std::make_shared<Argument>(tok,tok.keyword,str);
}

AstNodePtr Parser::parseStyle() {
    //style
    auto tok = m_currentToken;//on `id`
    expect(tk_less,"Expected a < but got "+next().keyword+" instead","Add a < here","","");//on `<`
    expect(tk_name,"Expected a `name` but got "+next().keyword+" instead","Add a `name` here","","");//on `name`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_string,"Expected a field name but got "+next().keyword+" instead","Add a field name here","","");
    auto name= m_currentToken.keyword;
    expect(tk_comma,"Expected a `,` but got "+next().keyword+" instead","Add a `,` here","","");
    expect(tk_type,"Expected a `type` but got "+next().keyword+" instead","Add a `type` here","","");
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    advance();
    switch(m_currentToken.tkType){
        case tk_content:
        case tk_input:
        case tk_link:
        case tk_embed:{
            break;
        }
        default:{
            error(m_currentToken,"Expected  `content`, `input` or `embed` but got "+m_currentToken.keyword+" instead","Add a proper type","","");
        }
    }
    auto type= m_currentToken.tkType;
    expect(tk_greater,"Expected a > but got "+next().keyword+" instead","Add a > here","","");//on `>`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    std::vector<AstNodePtr> args;
    while(m_currentToken.tkType!=tk_dedent){
        advance();
        if(m_currentToken.tkType==tk_dedent){
            break;
        }
        switch (m_currentToken.tkType) {
            case tk_id:{
                args.push_back(parseStyle());
                m_currentToken.tkType=tk_new_line;
                break;
            }
            case tk_string:{
                args.push_back(parseArgument());
                break;
            }
            default:{
                error(m_currentToken,"Invalid argument "+m_currentToken.keyword);
            }
        }
    }
    return std::make_shared<Section>(tok,name,type,args);
}
}
}