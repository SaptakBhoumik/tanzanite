#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__
#include <gtk/gtk.h>
#include <map>
#include "tml/vm_opt/opt_code.hpp"
namespace tml{
namespace renderer{
class Renderer {
    std::vector<tml::tml_vm::opt> m_code;
    int m_pos=0;
    GtkWidget *m_window;
    GtkWidget *m_grid;
    GtkWidget *m_box;
    void render_input(opt_code::type_of_element, char*, size_t,char*, size_t,GtkWidget*,int*);
    //Various input type
    void render_button(char*, size_t,char*, size_t,GtkWidget*,int*);
    void render_checkbox(char*, size_t,char*, size_t,GtkWidget*,int*);
    void render_date(char*, size_t,GtkWidget*,int*);
    void render_text_input(char*, size_t,char*, size_t,GtkWidget*,int*);
    void render_password(char*, size_t,char*, size_t,GtkWidget*,int*);
    void render_range(char*, size_t,GtkWidget*,int*);
    //render Embeded
    GtkWidget* renderEmbeded(char*,char*,size_t,opt_code::type_of_element);
    void render(std::vector<tml::tml_vm::opt>&,GtkWidget*,int*);
    public:
    Renderer()=default;
    Renderer(std::vector<tml::tml_vm::opt> code);
    GtkWidget* render(GtkApplication *app, gpointer data);
    void set_window(GtkWidget *window);
};
}
}
#endif
