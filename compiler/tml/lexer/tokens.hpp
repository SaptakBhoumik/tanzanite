#ifndef __TML_TOKENS_HPP__
#define __TML_TOKENS_HPP__

#include <string>
namespace tml{
namespace lexer{
// Create a string variable
enum TokenType {
    tk_eof, // end of file
    // Some sections
    tk_lua,
    tk_content,
    tk_style,
    tk_title,
    tk_input,
    //arguments
    tk_id,//id name
    tk_text,//text
    tk_path,//path to a file
    tk_type,//input type
    //symbol
    tk_colon,      // :
    tk_new_line,   // \n
    //keyword
    tk_pass,

    tk_ident,  // beginning of identation
    tk_dedent, // end of identation

    tk_string, // string
    tk_raw,    // raw string i.e. r
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