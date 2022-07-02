#include "opt_code.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
namespace tml{
namespace opt_code{
        std::map<std::string, type_of_element> type_of_element_map= {
            {"img",TYPE_IMG},
            {"video",TYPE_VIDEO},
            {"audio",TYPE_AUDIO},
            {"button",TYPE_BUTTON},
            {"checkbox",TYPE_CHECKBOX},
            {"color",TYPE_COLOR},
            {"date",TYPE_DATE},
            {"datetime_local",TYPE_DATETIME_LOCAL},
            {"email",TYPE_EMAIL},
            {"file",TYPE_FILE},
            {"time",TYPE_TIME},
            {"tel",TYPE_TEL},
            {"text",TYPE_TEXT},
            {"month",TYPE_MONTH},
            {"number",TYPE_NUMBER},
            {"password",TYPE_PASSWORD},
            {"radio",TYPE_RADIO},
            {"range",TYPE_RANGE},
            {"reset",TYPE_RESET},
            {"url",TYPE_URL},
            {"week",TYPE_WEEK},
            };
        std::map<type_of_element, std::string> type_of_element_to_string= {
            {TYPE_IMG,"img"},
            {TYPE_VIDEO,"video"},
            {TYPE_AUDIO,"audio"},
            {TYPE_BUTTON,"button"},
            {TYPE_CHECKBOX,"checkbox"},
            {TYPE_COLOR,"color"},
            {TYPE_DATE,"date"},
            {TYPE_DATETIME_LOCAL,"datetime_local"},
            {TYPE_EMAIL,"email"},
            {TYPE_FILE,"file"},
            {TYPE_TIME,"time"},
            {TYPE_TEL,"tel"},
            {TYPE_TEXT,"text"},
            {TYPE_MONTH,"month"},
            {TYPE_NUMBER,"number"},
            {TYPE_PASSWORD,"password"},
            {TYPE_RADIO,"radio"},
            {TYPE_RANGE,"range"},
            {TYPE_RESET,"reset"},
            {TYPE_URL,"url"},
            {TYPE_WEEK,"week"},
            };
}
namespace tml_vm{
    code_elm::~code_elm(){}
    code_elm::code_elm(const code_elm& other){
        operator=(other);
    }
    void code_elm::operator=(const code_elm& other){
        this->type=other.type;
        this->is_arg=other.is_arg;
        this->arg_type=other.arg_type;
        switch(other.type){
            case STRING:{
                auto size=strlen(other.str);
                this->str=new char[size+1];
                for(size_t i=0;i<size;++i){
                    this->str[i]=other.str[i];
                }
                this->str[size]='\0';
                this->len=size;
                break;
            }
            case TYPE:{
                this->elm_type=other.elm_type;
                break;
            }
            case NESTED:{
                if(other.nested_elements.size()<=0){
                    this->nested_elements=std::vector<opt>();
                }
                else{
                    this->nested_elements=other.nested_elements;
                }
                break;
            }
        }
    };
    code_elm::code_elm(opt_code::type_of_element value){
        this->type=TYPE;
        this->elm_type=value;
    }
    code_elm::code_elm(std::string value){
        this->type=STRING;
        auto size=value.size();
        this->str=new char[size+1];
        for(size_t i=0;i<size;++i){
            this->str[i]=value[i];
        }
        this->str[size]='\0';
        this->len=size;
    }
    code_elm::code_elm(std::vector<opt> value){
        this->type=NESTED;
        if(value.size()<=0){
            this->nested_elements=std::vector<opt>();
        }
        else{
            this->nested_elements=value;
        }
    }
    code_elm::code_elm(opt_code::argtype arg, code_elm value){
        *this=value;
        this->arg_type=arg;
        is_arg=true;
    }


    opt::opt(opt_code::optcode opt_code,std::vector<code_elm> args){
        this->opt_code=opt_code;
        this->args=args;
    }
    void opt::operator=(const opt& other){
        this->opt_code=other.opt_code;
        this->args=other.args;
    }
    opt::opt(const opt& other){
        operator=(other);
    }
}
}