#ifndef __tss_TOKENS_HPP__
#define __tss_TOKENS_HPP__

#include <string>
namespace tss{
namespace lexer{
// Create a string variable
enum TokenType {
    tk_eof, // end of file
    // Some sections
    tk_content,
    tk_input,
    tk_embed,
    tk_link,
    tk_id,//id name
    tk_type,//type of id
    tk_name,//name of id
    
    //symbol
    tk_colon,      // :
    tk_new_line,   // \n
    tk_less,       // <
    tk_greater, // >
    tk_comma, // ,

    tk_ident,  // beginning of identation
    tk_dedent, // end of identation

    tk_string, // string
};

struct Token {
    size_t location;
    std::string statement;
    std::string keyword;
    size_t start;
    size_t end;
    size_t line;
    TokenType tkType;
    size_t tab=0;
};
}
}
#endif