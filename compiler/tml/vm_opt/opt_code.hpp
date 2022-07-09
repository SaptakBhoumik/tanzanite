#ifndef __TML_OPT_CODE_HPP__
#define __TML_OPT_CODE_HPP__
#include <string>
#include <vector>
#include <map>
namespace tml{
namespace opt_code{
    enum optcode{
        OPT_EMBED=0,//embed <type> <path> <id> <text>
        OPT_INPUT=1,//input <type> <id> <text>
        OPT_STYLE=3,//style <path>
        OPT_LUA=4,//lua <path>
        OPT_TITLE=5,//title <text>
        OPT_CONTENT=6,//content <id> <text> : ...rest of the nested elements
        OPT_LINK=7,//link <id> : ...rest of the nested elements
    };
    enum argtype{
        ARG_ID=0,
        ARG_TYPE=1,
        ARG_TEXT=2,
        ARG_PATH=3,
    };
    enum type_of_element{
        //embed type
        TYPE_IMG=0,
        TYPE_VIDEO=1,
        TYPE_AUDIO=2,

        //input type        
        TYPE_BUTTON=3,//
        TYPE_CHECKBOX=4,//
        TYPE_COLOR=5,
        TYPE_DATE=6,
        TYPE_DATETIME_LOCAL=7,
        TYPE_FILE=9,
        TYPE_TIME=10,
        TYPE_TEXT=12,//
        TYPE_MONTH=13,
        TYPE_PASSWORD=15,//
        TYPE_RADIO=16,
        TYPE_RANGE=17,//
        TYPE_WEEK=20,
    };
    extern std::map<std::string, type_of_element> type_of_element_map;
    extern std::map<type_of_element, std::string> type_of_element_to_string;
}
namespace tml_vm{
    enum type_of_code_block{
        STRING=0,
        TYPE=1,
        NESTED=2,
    };
    struct opt;
    struct code_elm;
    struct code_elm{
        type_of_code_block type;
        opt_code::argtype arg_type;//if any
        bool is_arg=false;
        union{
            opt_code::type_of_element elm_type;
            struct{
                char* str;
                size_t str_len;
            };
            struct{
                opt* nested_elements;
                size_t list_size;
            };
        };
        ~code_elm();
        code_elm(const code_elm& other);
        code_elm(opt_code::type_of_element value);
        code_elm(std::string value);
        code_elm(std::vector<opt> value);
        code_elm(opt_code::argtype arg, code_elm value);
        void operator=(const code_elm& other);
    };
    struct opt{
        opt_code::optcode opt_code;
        code_elm* args;
        size_t list_size;
        opt(opt_code::optcode opt_code,std::vector<code_elm> args);
        opt(const opt& other);
        void operator=(const opt& other);
    };
    void clean_up(opt);
    void clean_up(std::vector<opt>);
}
}
#endif