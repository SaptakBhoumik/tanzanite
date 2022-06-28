#include "parser.hpp"

#include "parser.hpp"
#include <iostream>
#include <algorithm>

namespace tml{
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
        case tk_lua:{
            stmt = parseLua();
            break;
        }
        case tk_content:{
            stmt = parseContent();
            break;
        }
        case tk_style:{
            stmt = parseStyle();
            break;
        }
        case tk_title:{
            stmt = parseTitle();
            break;
        }
        case tk_input:{
            stmt = parseInput();
            break;
        }
        case tk_embed:{
            stmt = parseEmbed();
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
    return std::make_shared<Argument>(tok,str);
}

AstNodePtr Parser::parseLua() {
    //lua
    auto tok = m_currentToken;//on `lua`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    expect(tk_path,"Expected path to lua file but got "+next().keyword+" instead");
    std::vector<AstNodePtr> args;
    args.push_back(parseArgument());
    expect(tk_dedent,"Expected dedent but got "+next().keyword+"Add a dedent here","","");
    return std::make_shared<Section>(tok,args);
}
AstNodePtr Parser::parseStyle() {
    //style
    auto tok = m_currentToken;//on `style`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    expect(tk_path,"Expected path to style file but got "+next().keyword+" instead");
    std::vector<AstNodePtr> args;
    args.push_back(parseArgument());
    expect(tk_dedent,"Expected dedent but got "+next().keyword+"Add a dedent here","","");
    return std::make_shared<Section>(tok,args);
}
AstNodePtr Parser::parseTitle() {
    //title
    auto tok = m_currentToken;//on `title`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    expect(tk_text,"Expected title but got "+next().keyword+" instead");
    std::vector<AstNodePtr> args;
    args.push_back(parseArgument());
    expect(tk_dedent,"Expected dedent but got "+next().keyword+"Add a dedent here","","");
    return std::make_shared<Section>(tok,args);
}
AstNodePtr Parser::parseContent() {
    //content
    std::vector<AstNodePtr> args;
    auto tok = m_currentToken;//on `content`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    while(m_currentToken.tkType!=tk_dedent){
        advance();
        if(m_currentToken.tkType==tk_dedent){
            break;
        }
        switch (m_currentToken.tkType) {
            case tk_text:
            case tk_id:{
                args.push_back(parseArgument());
                break;
            }
            case tk_content:{
                args.push_back(parseContent());
                m_currentToken.tkType=tk_new_line;
                break;
            }
            case tk_input:{
                args.push_back(parseInput());
                m_currentToken.tkType=tk_new_line;
                break;
            }
            case tk_embed:{
                args.push_back(parseEmbed());
                m_currentToken.tkType=tk_new_line;
                break;
            }
            default:{
                error(m_currentToken,"Invalid argument "+m_currentToken.keyword);
            }
        }
    }
    return std::make_shared<Section>(tok,args);
}
AstNodePtr Parser::parseInput() {
    //input
    std::vector<AstNodePtr> args;
    auto tok = m_currentToken;//on `input`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    while(m_currentToken.tkType!=tk_dedent){
        advance();
        if(m_currentToken.tkType==tk_dedent){
            break;
        }
        switch (m_currentToken.tkType) {
            case tk_text:
            case tk_type:
            case tk_id:{
                args.push_back(parseArgument());
                break;
            }
            default:{
                error(m_currentToken,"Invalid argument "+m_currentToken.keyword);
            }
        }
    }
    return std::make_shared<Section>(tok,args);
}
AstNodePtr Parser::parseEmbed() {
    //embed
    std::vector<AstNodePtr> args;
    auto tok = m_currentToken;//on `embed`
    expect(tk_colon,"Expected a : but got "+next().keyword+" instead","Add a : here","","");//on `:`
    expect(tk_ident,"Expected an idented block but got "+next().keyword+" instead");//on `ident`
    while(m_currentToken.tkType!=tk_dedent){
        advance();
        if(m_currentToken.tkType==tk_dedent){
            break;
        }
        switch (m_currentToken.tkType) {
            case tk_text:
            case tk_path:
            case tk_type:
            case tk_id:{
                args.push_back(parseArgument());
                break;
            }
            default:{
                error(m_currentToken,"Invalid argument "+m_currentToken.keyword);
            }
        }
    }
    return std::make_shared<Section>(tok,args);
}
}
}