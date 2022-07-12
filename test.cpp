#include "compiler/tml/parser/parser.hpp"
#include "compiler/tml/codegen/codegen.hpp"
#include "compiler/tml/disassembler/dis.hpp"
#include "compiler/tml/lexer/lexer.hpp"
#include "compiler/tss/parser/parser.hpp"
#include "compiler/tss/lexer/lexer.hpp"
#include "renderer/renderer.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
tml::renderer::Renderer render;
static void activate(GtkApplication *app, gpointer data){
    auto y=render.render(app, data);
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "style/default.css");
    gtk_style_context_add_provider_for_display(gtk_widget_get_display (GTK_WIDGET (y)),
                               GTK_STYLE_PROVIDER(cssProvider),
                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_widget_show(y);
        int width=0,height=0;
    gtk_window_get_default_size(GTK_WINDOW(y),&width,&height);
    std::cout<<height<<std::endl;
    std::cout<<width<<std::endl;
};
int main(int    argc, char **argv){
    {
        using namespace tml;
        using namespace lexer;
        using namespace ast;
        using namespace parser;
        using namespace codegen;
        using namespace dis;
        using namespace renderer;
        std::ifstream file("test.tml");
        std::stringstream buf;
        buf << file.rdbuf();
        auto lex=LEXER(buf.str(), "test");
        std::vector<Token> tokens = lex.result();
        
        Parser parser(tokens, "test");
        ast::AstNodePtr program = parser.parse();
        // std::cout << program->stringify() << "\n";
        auto x=Codegen(program);
        auto code=x.get_code();
        std::cout<<disassemble(code)<<"\n";
        GtkApplication *app;
        int status;
        app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        render=Renderer(code);
        g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
        status = g_application_run (G_APPLICATION (app), argc, argv);
        g_object_unref (app);
        clean_up(code);
    }
    {
        using namespace tss;
        using namespace lexer;
        using namespace ast;
        using namespace parser;
        std::ifstream file("test.tss");
        std::stringstream buf;
        buf << file.rdbuf();
        auto lex=LEXER(buf.str(), "test");
        std::vector<Token> tokens = lex.result();
        
        Parser parser(tokens, "test");
        ast::AstNodePtr program = parser.parse();
        // std::cout << program->stringify() << "\n";
    }
}