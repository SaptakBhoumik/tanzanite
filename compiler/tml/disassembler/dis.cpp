#include "dis.hpp"
#include <iostream>
#include <string.h>
#define add_str(x) res+=padding+std::string(x)+"\n";
namespace tml{
namespace dis{
    std::string disassemble(const std::vector<tml_vm::opt>& code,std::string padding){
        std::string res;
        for(auto& opt : code){
            switch(opt.opt_code){
                case opt_code::OPT_LUA:{
                    add_str("lua:");
                    break;
                }
                case opt_code::OPT_CONTENT:{
                    add_str("content:");
                    break;
                }
                case opt_code::OPT_STYLE:{
                    add_str("style:");
                    break;
                }
                case opt_code::OPT_TITLE:
                    add_str("title:");
                    break;
                case opt_code::OPT_INPUT:{
                    add_str("input:");
                    break;
                }
                case opt_code::OPT_EMBED:{
                    add_str("embed\n");
                    break;
                }
                case opt_code::OPT_LINK:{
                    add_str("link:");
                    break;
                }
            }
            for(auto& arg : opt.args){
                if(arg.is_arg){
                    switch(arg.arg_type){
                        case opt_code::ARG_ID:{
                            std::string str="";
                            auto size=arg.len;
                            for(size_t i=0;i<size;i++){
                                str+=arg.str[i];
                            }
                            add_str("\tid:"+str);
                            break;
                        }
                        case opt_code::ARG_TYPE:{
                            add_str("\ttype:"+opt_code::type_of_element_to_string[arg.elm_type]);
                            break;
                        }
                        case opt_code::ARG_TEXT:{
                            std::string str="";
                            auto size=arg.len;
                            for(size_t i=0;i<size;i++){
                                str+=arg.str[i];
                            }
                            add_str("\ttext:"+str);
                            break;
                        }
                        case opt_code::ARG_PATH:{
                            std::string str="";
                            auto size=arg.len;
                            for(size_t i=0;i<size;i++){
                                str+=arg.str[i];
                            }
                            add_str("\tpath:"+str);
                            break;
                        }
                    }
                }
                else{
                    add_str("\t"+disassemble(arg.nested_elements,padding+"\t"));
                }
            }
        }
        return res;
    }
}
}