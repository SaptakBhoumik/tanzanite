#include "opt_code.hpp"
namespace tml{
namespace tml_vm{
    code_elm::~code_elm(){}
    code_elm::code_elm(const code_elm& other){
        this->type=other.type;
        switch(other.type){
            case STRING:{
                this->str=other.str;
                break;
            }
            case TYPE:{
                this->elm_type=other.elm_type;
                break;
            }
            case NESTED:{
                this->nested_elements=other.nested_elements;
                break;
            }
        }
    };
}
}