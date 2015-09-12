#ifndef DIALOG_H
#define DIALOG_H
#include<gtkmm.h>
#include "Image.h"
#include "Interface.h"
#include<fstream>

class dialog : public Gtk::Window
{
    public:
        dialog();
        virtual ~dialog();
        void operate(Image* im,int i,Gtk::Image *m_image1,int imagestatus1,Gtk::Label* pixlabel);
        void operate3(Image* imag,Gtk::Image *m_image1,int imagestatus1,Gtk::Label* pixlabel);
        void operate4(Image imag,std::string filename);
        void on_bok_click(Gtk::Entry *bvalue,int j);
        void on_bok2_click(Gtk::Entry *x1,Gtk::Entry *x2,Gtk::Entry *y1,Gtk::Entry *y2);
        void on_bok3_click(Gtk::Entry *bx1,Gtk::Entry *bx2,Gtk::Entry *by1,Gtk::Entry *by2);
        void on_bok4_click(Gtk::Entry *bvalue,Image imag,std::string filename);
        void on_click_rotatea_button();
        void on_click_rotatec_button();
        void set_label();
    protected:
    private:
    Image* im;
    Interface* I;
    Gtk::Label* label;
    Gtk::Image* m_image;
    int imagestatus;
};

#endif // DIALOG_H
