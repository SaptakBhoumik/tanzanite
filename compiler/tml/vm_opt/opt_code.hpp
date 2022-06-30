#ifndef __TML_OPT_CODE_HPP__
#define __TML_OPT_CODE_HPP__
#include <string>
#include <vector>
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

    enum type_of_element{
        //embede type
        TYPE_IMG=0,
        TYPE_VIDEO=1,
        TYPE_AUDIO=2,

        //input type        
        TYPE_BUTTON=3,
        TYPE_CHECKBOX=4,
        TYPE_COLOR=5,
        TYPE_DATE=6,
        TYPE_DATETIME_LOCAL=7,
        TYPE_EMAIL=8,
        TYPE_FILE=9,
        TYPE_TIME=10,
        TYPE_TEL=11,
        TYPE_TEXT=12,
        TYPE_MONTH=13,
        TYPE_NUMBER=14,
        TYPE_PASSWORD=15,
        TYPE_RADIO=16,
        TYPE_RANGE=17,
        TYPE_RESET=18,
        TYPE_URL=19,
        TYPE_WEEK=20,
    };
}
namespace tml_vm{
    enum type_of_code_block{
        STRING,
        TYPE,
        NESTED
    };
    struct opt;
    struct code_elm{
        opt_code::type_of_element type;
        union{
            opt_code::type_of_element elm_type;
            std::string str;
            std::vector<opt> nested_elements;
        };
        ~code_elm();
        code_elm(const code_elm& other);
    };
    struct opt{
        opt_code::optcode opt_code;
        std::vector<code_elm> args;
    };
}
}
#endif