#ifndef INTERFACE_H
#define INTERFACE_H
#include "Image.h"
#include<gtkmm.h>
#include "dialog.h"
#include<stdlib.h>

class Interface : public Gtk::Window
{
    public:
        Interface();
        virtual ~Interface();
        void initial();
        void on_click_open_button();
        void on_click_undo_button();
        void on_click_redo_button();
        void filechoose();
        void save();
        void on_click_image_button();
        void on_click_quit_button();
        void on_click_grayscale_button();
        void on_click_brightness_button();
        void on_click_sepia_button();
        void on_click_negative_button();
        void on_click_blur_button();
        void on_click_contrast_button();
        void on_click_rotate_button();
        void on_click_flip_horizontal_button();
        void on_click_flip_vertical_button();
        void on_click_dither_button();
        void on_click_edge_detect_button();
        void on_click_posterize_button();
        void on_click_halftone_button();
        void on_click_salt_pepper_button();
        void on_click_crop_button();
        void set_label();
    protected:
    private:
        Image imagorg,imag; //iamgorg is original image imag is current image
        int running;   //keep track of no of images opened
        int imagestatus; //image is shown or hidden
        bool bigimage;  //whether image is big to display on scrren
        Gtk::Image *m_image;
        Gtk::Button *imageb;
        Gtk::Label *label;
        Gtk::Grid *grid;
        Gtk::Box *vbox;
};

#endif // INTERFACE_H
