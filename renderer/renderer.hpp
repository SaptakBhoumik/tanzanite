#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__
#include <gtk/gtk.h>
#include "tml/vm_opt/opt_code.hpp"
namespace tml{
namespace renderer{
class Renderer {
    std::vector<tml::tml_vm::opt> m_code;
    GtkWidget *m_window;
    public:
    Renderer()=default;
    Renderer(std::vector<tml::tml_vm::opt> code);
    GtkWidget* render(GtkApplication *app, gpointer data);
    void set_window(GtkWidget *window);
};
}
}
#endif
