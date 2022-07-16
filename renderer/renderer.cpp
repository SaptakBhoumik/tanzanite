#include "renderer.hpp"
#include <iostream>
//TODO: handle id and responsiveness
namespace tml{
namespace renderer{
Renderer::Renderer(std::vector<tml::tml_vm::opt> code){
    m_code=code;
}

GtkWidget* Renderer::render(GtkApplication *app, gpointer data){
    m_window = gtk_application_window_new(app);
    gtk_widget_set_name(m_window, "tml_renderer");
    gtk_window_maximize (GTK_WINDOW(m_window));
    m_grid=gtk_flow_box_new();
    gtk_widget_set_valign (m_grid, GTK_ALIGN_START);
    gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(m_grid), 2);
    auto scroll=gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), m_grid);
    gtk_window_set_child (GTK_WINDOW (m_window), scroll);
    render(m_code,m_grid,&m_pos);
    return m_window;
}

void Renderer::render(std::vector<tml::tml_vm::opt>& code,GtkWidget* grid,int* pos){
    using namespace opt_code;

    for(auto& x:m_code){
        switch(x.opt_code){
            case OPT_TITLE:{
                gtk_window_set_title(GTK_WINDOW (m_window), x.args[0].str);
                break;
            }
            case OPT_CONTENT:{
                auto arg=x.args;
                auto sire=x.list_size;
                char* text;
                size_t len=0;
                char* id;
                size_t id_len=0;
                std::vector<tml::tml_vm::opt> nested_code;
                for(size_t i=0;i<sire;i++){
                    if (!arg[i].is_arg){
                        auto elm=arg[i].nested_elements;
                        auto sire_elm=arg[i].list_size;
                        for (size_t j=0;j<sire_elm;j++){
                            nested_code.push_back(elm[j]);
                        }
                    }
                    else if(arg[i].arg_type==ARG_TEXT){
                        text=arg[i].str;
                        len=arg[i].str_len;
                    }
                    else if(arg[i].arg_type==ARG_ID){
                        id=arg[i].str;
                        id_len=arg[i].str_len;
                    }

                }
                auto label=gtk_label_new(text);
                gtk_widget_set_can_focus(GTK_WIDGET(label),FALSE);
                if(id_len>0){
                    gtk_widget_set_name(label, id);
                }
                else{
                    gtk_widget_set_name(label, "tml_label");
                }
                gtk_label_set_selectable(GTK_LABEL(label), TRUE);
                gtk_flow_box_insert(GTK_FLOW_BOX(grid), label,*pos);
                (*pos)++;
                if(nested_code.size()>0){
                    auto new_grid=gtk_flow_box_new();
                    gtk_widget_set_valign(new_grid, GTK_ALIGN_START);
                    gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(new_grid), 2);
                    int new_pos=0;
                    render(nested_code,new_grid,&new_pos);
                    gtk_flow_box_insert(GTK_FLOW_BOX(grid), new_grid,*pos);
                    (*pos)++;
                }
                break;
            }
            case OPT_INPUT:{
                auto arg=x.args;
                auto sire=x.list_size;
                opt_code::type_of_element type;
                char* text;
                size_t len=0;
                char* id;
                size_t id_len=0;
                for(size_t i=0;i<sire;i++){
                    if(arg[i].arg_type==ARG_TYPE){
                        type=arg[i].elm_type;
                    }
                    else if(arg[i].arg_type==ARG_TEXT){
                        text=arg[i].str;
                        len=arg[i].str_len;
                    }
                    else if(arg[i].arg_type==ARG_ID){
                        id=arg[i].str;
                        id_len=arg[i].str_len;
                    }
                }
                (*pos)++;
                render_input(type, text, len,id,id_len,grid,pos);
                break;
            }
            default:{}
        }
    }
}

void Renderer::render_input(opt_code::type_of_element type,char* text, 
                            size_t text_len,char* id, size_t id_len,
                            GtkWidget* grid,int* pos){
    using namespace opt_code;
    switch(type){
        case TYPE_BUTTON:{
            render_button(text,text_len,id,id_len,grid,pos);
            break;
        }
        case TYPE_CHECKBOX:{
            render_checkbox(text,text_len,id,id_len,grid,pos);
            break;
        }
        case TYPE_DATE:{
            render_date(id,id_len,grid,pos);
            break;
        }
        case TYPE_TEXT:{
            render_text_input(text,text_len,id,id_len,grid,pos);
            break;
        };
        case TYPE_PASSWORD:{
            render_password(text,text_len,id,id_len,grid,pos);
            break;
        };
        case TYPE_RANGE:{
            render_range(id,id_len,grid,pos);
            break;
        }
        default:{}
    }
}
void Renderer::set_window(GtkWidget *window){
    m_window=window;
}
}
}