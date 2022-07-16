#include "../renderer.hpp"
namespace tml{
namespace renderer{
void Renderer::render_button(char* text, size_t text_len,char* id, size_t id_len,GtkWidget* grid,int* pos){
    GtkWidget *button=gtk_button_new();
            if(text_len!=0){
                gtk_button_set_label(GTK_BUTTON(button), text);
            }
            if(id_len!=0){
                gtk_widget_set_name(button, id);
            }
            else{
                gtk_widget_set_name(button, "tml_button");
            }
            gtk_flow_box_insert(GTK_FLOW_BOX(grid), button,*pos);
}
void Renderer::render_checkbox(char* text, size_t text_len,char* id, size_t id_len,GtkWidget* grid,int* pos){
    GtkWidget *check=gtk_check_button_new();
    if(text_len!=0){
        gtk_check_button_set_label(GTK_CHECK_BUTTON(check), text);
    }
    if(id_len!=0){
        gtk_widget_set_name(check, id);
    }
    else{
        gtk_widget_set_name(check, "tml_checkbox");
    }
    gtk_flow_box_insert(GTK_FLOW_BOX(grid), check,*pos);
}
void Renderer::render_date(char* id, size_t id_len,GtkWidget* grid,int* pos){
    //TODO: implement date
    auto cal=gtk_calendar_new();
    if(id_len!=0){
        gtk_widget_set_name(cal, id);
    }
    else{
        gtk_widget_set_name(cal, "tml_date");
    }
    gtk_flow_box_insert(GTK_FLOW_BOX(grid), cal,*pos);
}
void Renderer::render_text_input(char* text, size_t text_len,char* id, size_t id_len,GtkWidget* grid,int* pos){
    auto text_enter=gtk_entry_new();
    if(text_len!=0){
        gtk_entry_set_placeholder_text(GTK_ENTRY(text_enter), text);
    }
    if(id_len!=0){
        gtk_widget_set_name(text_enter, id);
    }
    else{
        gtk_widget_set_name(text_enter, "tml_text");
    }
    gtk_flow_box_insert(GTK_FLOW_BOX(grid), text_enter,*pos);
}
void Renderer::render_password(char* text, size_t text_len,char* id, size_t id_len,GtkWidget* grid,int* pos){
    auto pass_enter=gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(pass_enter), FALSE);
    if(text_len!=0){
        gtk_entry_set_placeholder_text(GTK_ENTRY(pass_enter), text);
    }
    if(id_len!=0){
        gtk_widget_set_name(pass_enter, id);
    }
    else{
        gtk_widget_set_name(pass_enter, "tml_password");
    }
    gtk_flow_box_insert(GTK_FLOW_BOX(grid), pass_enter,*pos);
}
void Renderer::render_range(char* id, size_t id_len,GtkWidget* grid,int* pos){
    auto range=gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL,0,1,0.001);
    if(id_len!=0){
        gtk_widget_set_name(range, id);
    }
    else{
        gtk_widget_set_name(range, "tml_range");
    }
    gtk_widget_set_size_request(range, 200, -1);
    gtk_flow_box_insert(GTK_FLOW_BOX(grid), range,*pos);
}
}
}