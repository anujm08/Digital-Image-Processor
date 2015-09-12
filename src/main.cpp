#include<iostream>
#include<gtkmm.h>
#include"Interface.h"
int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app =Gtk::Application::create(argc, argv,"org.gtkmm.example");
    Interface I;
    I.initial();
    return app->run(I);
}
