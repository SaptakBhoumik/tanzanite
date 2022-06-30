#include "tss/lexer/lexer.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#define not_tab()   m_is_tab=false;

namespace tss{
namespace lexer{
LEXER::LEXER(std::string input, std::string filename){
    m_input = input;
    m_filename = filename;
    if(m_input.size()>0){
        m_statments=split_ln(input);
        m_curr_item=m_input[0];
        m_curr_line=m_statments[0];
        lex();
        complete_it();
    }
    else {
        m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "<tk_eof>",
            m_curr_index,
            m_curr_index+1,
            m_line,
            tk_eof,
            m_tab_count
        });
    }
}

void LEXER::add_unknown(){
    TokenType type;
    std::map<std::string,TokenType> key_map={
        //sections
        {"content",tk_content},
        {"input",tk_input},
        {"embed",tk_embed},
        {"link",tk_link},
        {"id",tk_id},
        {"type",tk_type},//type of id
        {"name",tk_name},//name of id
    };
    if (key_map.count(m_keyword) > 0) {
        type = key_map[m_keyword];
    }
    else if(m_keyword!=""){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unknown keyword: "+m_keyword,
                 .ecode = ""})
        ));
    } 
    if(m_keyword!=""){
        auto res = Token{
                        m_loc,
                        m_curr_line,
                        m_keyword,
                        m_curr_index-m_keyword.length(),
                        m_curr_index+1,
                        m_line,
                        type,
                        m_tab_count
        };   
        m_result.push_back(res);
    }
    m_keyword = "";
}
void LEXER::complete_it(){
    auto result=m_result;
    m_result.clear();
    size_t previous_ident=0;
    size_t current_ident=0;
    for(auto item:result){
        current_ident=item.tab;
        if(current_ident>previous_ident){
            m_result.push_back(Token{
                item.location,
                item.statement,
                "<ident>",
                item.start,
                item.end,
                item.line,
                tk_ident,
            });
            m_tabs.push_back(item.tab);
        }
        else if(current_ident<previous_ident){
            while (current_ident < previous_ident) {
                m_tabs.pop_back();
                m_result.push_back(Token{
                    item.location,
                    item.statement,
                    "<dedent>",
                    item.start,
                    item.end,
                    item.line,
                    tk_dedent,
                });
                if (m_tabs.size() != 0) {
                    if (current_ident >=m_tabs.back()) {
                        break;
                    }
                    previous_ident = m_tabs.back();
                } else {
                    previous_ident = 0;
                }
            }    
        }
        m_result.push_back(item);
        previous_ident=current_ident;
    }
    if(m_keyword!=""){
        add_unknown();
    }
    if(m_first_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a ')'",
                 .ecode = "e1"})
        ));
    }
    else if(m_second_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a '}'",
                 .ecode = "e1"})
        ));
    }
    else if(m_third_bracket_count!=0){
        m_error.push_back(PEError(
            PEError({.loc = Location({.line = m_line,
                                  .col = m_loc,
                                  .loc=m_loc,
                                  .file = m_filename,
                                  .code = m_curr_line}),
                 .msg = "Unexpected end of file",
                 .submsg = "Expecting a ']'",
                 .ecode = "e1"})
        ));
    }
    if(m_error.size()>0){
        for (auto& x: m_error) {
            display(x);
        }
        exit(1);
    }
    if(m_result.size()>0){
        if(m_result.back().tkType!=tk_new_line
            && m_result.back().tkType!=tk_dedent
            ){
            m_result.push_back(Token{
                m_loc,
                m_curr_line,
                "<tk_new_line>",
                m_curr_index,
                m_curr_index+1,
                m_line,
                tk_new_line,
                m_tab_count
            });
        }
    }
    auto item=m_result.back();
    for(size_t i=0;i<m_tabs.size();++i){
        m_result.push_back(Token{
                item.location,
                item.statement,
                "<dedent>",
                item.start,
                item.end,
                item.line,
                tk_dedent,
        });
    }
    m_result.push_back(Token{
            m_loc,
            m_curr_line,
            "<tk_eof>",
            m_curr_index,
            m_curr_index+1,
            m_line,
            tk_eof,
            m_tab_count
    });
    
}

void LEXER::lex(){
    while (true){
        switch(m_curr_item){
            case '#':{
                add_unknown();
                while(next()!='\n' && next()!='\0' && next()!='\r'){
                    if(!advance()){
                        break;
                    }
                }
                break;
            }
            case '\"':
            case '\'':{
                not_tab();
                add_unknown();
                lex_string();
                break;
            }
            
            case ':':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_colon,
                            m_tab_count
                });
                break;
            }
            case ',':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_comma,
                            m_tab_count
                });
                break;
            }
            case '<':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_less,
                            m_tab_count
                });
                break;
            }
            case '>':{
                not_tab();
                add_unknown();
                m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            std::string(1,m_curr_item),
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_greater,
                            m_tab_count
                });
                break;
            }
            case ' ':{
                add_unknown();
                if(m_is_tab&&
                    m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_tab_count++;
                }
                break;
            }
            case '\t':{
                if(m_is_tab&&
                    m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_tab_count+=8;
                }
                add_unknown();
                break;
            }
            case '\n':{
                add_unknown();
                if(m_result.size()>0){
                    if (m_result.back().tkType!=tk_new_line && m_result.back().tkType!=tk_colon
                        && m_result.back().tkType!=tk_dedent
                        && m_first_bracket_count==0 
                        && m_second_bracket_count==0
                        && m_third_bracket_count==0){
                        m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            "<tk_new_line>",
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_new_line,
                            m_tab_count
                        });
                    }
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-1];
                if(m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_is_tab=true;
                    m_tab_count=0;
                }
                break;
            }
            case '\r':{
                add_unknown();
                if (next()=='\n'){// \r\n
                    advance();
                }
                if(m_result.size()>0){
                    if (m_result.back().tkType!=tk_new_line && m_result.back().tkType!=tk_colon
                        && m_result.back().tkType!=tk_dedent
                        && m_first_bracket_count==0 
                        && m_second_bracket_count==0
                        && m_third_bracket_count==0){
                        m_result.push_back(Token{
                            m_loc,
                            m_curr_line,
                            "<tk_new_line>",
                            m_curr_index,
                            m_curr_index+1,
                            m_line,
                            tk_new_line,
                            m_tab_count
                        });
                    }
                }
                if(m_first_bracket_count==0&&
                    m_second_bracket_count==0&&
                    m_third_bracket_count==0){
                    m_is_tab=true;
                    m_tab_count=0;
                }
                m_line++;
                m_loc=0;
                m_curr_line=m_statments[m_line-1];
                break;
            }
            default:{
                not_tab();
                m_keyword+=m_curr_item;
                break;
            }
        }
        if(!advance()){
            break;
        }
    }
}

void LEXER::lex_string(){
    char quote=m_curr_item;
    auto loc=m_loc;
    size_t start_index=m_curr_index+1;
    std::string temp="Expecting a ";
    temp.push_back(quote);
    std::string str="" ;
    if(!advance()){
        m_error.push_back(PEError({.loc = Location({.line = m_line,
                                          .col = m_loc,
                                          .loc=m_loc,
                                          .file = m_filename,
                                          .code = m_curr_line}),
                         .msg = "Unexpected end of file",
                         .submsg = temp,
                         .ecode = "e1"}));
    }
    while(m_curr_item!=quote){
        if(loc>=m_loc && m_is_tab){
            if(m_curr_item!=' '&&m_curr_item!='\t'){
                str.push_back(m_curr_item);
                m_is_tab=false;
            }
        }
        else{
            str.push_back(m_curr_item);
        }
        if(m_curr_item=='\n'){
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        else if(m_curr_item=='\r'){
            if (next()=='\n'){// \r\n
                advance();
                str.push_back(m_curr_item);
            }
            m_line++;
            m_loc=0;
            m_curr_line=m_statments[m_line-1];
            m_is_tab=true;
        }
        redo:{}
        if(!advance()){
            m_error.push_back(PEError({.loc = Location({.line = m_line,
                                            .col = m_loc,
                                            .loc=m_loc,
                                            .file = m_filename,
                                            .code = m_curr_line}),
                            .msg = "Unexpected end of file",
                            .submsg = temp,
                            .ecode = "e1"}));
            break;
        }
        if(m_curr_item==quote && str.size()>0){
            if(str.back()=='\\'){
                str.push_back(m_curr_item);
                goto redo;
            }
        }
    }
    m_result.push_back(Token{
                m_loc,
                m_curr_line,
                str,
                start_index,
                m_curr_index+1,
                m_line,
                tk_string,
                m_tab_count
            });
}

}
}
