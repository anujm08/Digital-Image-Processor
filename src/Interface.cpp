#include "Interface.h"
#include<gtkmm.h>
#include<iostream>
#include<cstdio>
#include <unistd.h>
#include"Image.h"
#include "FileDialog.h"

Interface::Interface()
{
    running=0;
    set_default_size(100,100);
    set_title("Image Editor");
    set_position(Gtk::WIN_POS_CENTER);
    set_border_width(10);
    vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);
    grid = Gtk::manage(new Gtk::Grid);
    grid->set_border_width(10);
    vbox->add(*grid);
    imageb = Gtk::manage(new Gtk::Button("Image"));
    m_image=new Gtk::Image();
    label = Gtk::manage(new Gtk::Label());
//ctor
}

Interface::~Interface()
{
    //dtor
}

void Interface::initial()
{
    filechoose();

    grid->attach(*imageb, 1, 1, 1000, 1000);
    grid->attach(*label, 1, 1001, 8,1);

    Gtk::Button *openb = Gtk::manage(new Gtk::Button("Open"));
    grid->attach(*openb, 1, 0, 1, 1);

    Gtk::Button *saveb = Gtk::manage(new Gtk::Button("Save As"));
    grid->attach(*saveb, 2, 0, 1, 1);

    Gtk::Button *undob = Gtk::manage(new Gtk::Button("Undo"));
    grid->attach(*undob, 3, 0, 1, 1);

    Gtk::Button *redob = Gtk::manage(new Gtk::Button("Redo"));
    grid->attach(*redob, 4, 0, 1, 1);
    //redob->set_relief(Gtk::RELIEF_NONE );

    Gtk::Button *quitb = Gtk::manage(new Gtk::Button("Quit"));
    grid->attach(*quitb, 5, 0, 1, 1);

    Gtk::Button *grayscaleb = Gtk::manage(new Gtk::Button("Grayscale"));
    grid->attach(*grayscaleb, 0 , 1, 1, 1);

    Gtk::Button *sepiab = Gtk::manage(new Gtk::Button("Sepia"));
    grid->attach(*sepiab, 0, 2, 1, 1);

    Gtk::Button *negativeb = Gtk::manage(new Gtk::Button("Negative"));
    grid->attach(*negativeb, 0, 3, 1, 1);

    Gtk::Button *blurb = Gtk::manage(new Gtk::Button("Blur"));
    grid->attach(*blurb, 0, 4, 1, 1);

    Gtk::Button *brightnessb = Gtk::manage(new Gtk::Button("Brightness"));
    grid->attach(*brightnessb, 0, 5, 1, 1);

    Gtk::Button *contrastb = Gtk::manage(new Gtk::Button("Contrast"));
    grid->attach(*contrastb, 0, 6, 1, 1);

    Gtk::Button *rotateb = Gtk::manage(new Gtk::Button("Rotate"));
    grid->attach(*rotateb, 0, 7, 1, 1);

    Gtk::Button *flip_horizontalb = Gtk::manage(new Gtk::Button("Horizontal Flip"));
    grid->attach(*flip_horizontalb, 0, 8, 1, 1);

    Gtk::Button *flip_verticalb = Gtk::manage(new Gtk::Button("Vertical Flip"));
    grid->attach(*flip_verticalb, 0,9, 1, 1);

    Gtk::Button *halftoneb = Gtk::manage(new Gtk::Button("Halftone"));
    grid->attach(*halftoneb, 0, 10, 1, 1);

    Gtk::Button *ditherb = Gtk::manage(new Gtk::Button("Dither"));
    grid->attach(*ditherb, 0,11, 1, 1);

    Gtk::Button *edge_detectb = Gtk::manage(new Gtk::Button("Edge Detection"));
    grid->attach(*edge_detectb, 0,12, 1, 1);

    Gtk::Button *salt_pepperb = Gtk::manage(new Gtk::Button("Salt and Pepper"));
    grid->attach(*salt_pepperb, 0,13, 1, 1);

    Gtk::Button *posterizeb = Gtk::manage(new Gtk::Button("Posterize"));
    grid->attach(*posterizeb, 0,14, 1, 1);

    Gtk::Button *cropb = Gtk::manage(new Gtk::Button("Crop"));
    grid->attach(*cropb, 0,15, 1, 1);

    vbox->show_all();

    imageb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_image_button));
    openb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_open_button));
    saveb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::save));
    undob->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_undo_button));
    redob->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_redo_button));
    quitb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_quit_button));
    grayscaleb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_grayscale_button));
    brightnessb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_brightness_button));
    sepiab->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_sepia_button));
    negativeb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_negative_button));
    blurb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_blur_button));
    contrastb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_contrast_button));
    rotateb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_rotate_button));
    flip_horizontalb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_flip_horizontal_button));
    flip_verticalb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_flip_vertical_button));
    ditherb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_dither_button));
    edge_detectb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_edge_detect_button));
    salt_pepperb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_salt_pepper_button));
    posterizeb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_posterize_button));
    halftoneb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_halftone_button));
    cropb->signal_clicked().connect(sigc::mem_fun(*this,&Interface::on_click_crop_button));
}

void Interface::filechoose()
{
    FileDialog* filec;
    std::ifstream f;
    char imagename[50];
    bool inputstatus=false;
    filec= new FileDialog(f,inputstatus,imagename);
    if(inputstatus==false&&running==0)
        exit(-1);
    if(inputstatus==true)
    {
        Glib::ustring s(imagename);
        imageb->set_label(s);
        running++;
        bool correctimage;
        correctimage=imagorg.readim(f);
        if(correctimage==false)
        {
            exit(-1);
        }
        imag.copyImage(imagorg);
        imag.index=0;
        imag.no_undo=0;
        imag.is_save=true;
        imag.createim();
        set_label();
        imagestatus=1;
        if(imag.height>700||imag.width>1200)
        {
            bigimage=true;
            Gtk::MessageDialog dlg("Only some part of image will be displayed.\n Do you still want to display image or continue without displaying? ", false,Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
            dlg.set_title("Image too large to be completely displayed on screen");
            gint result = dlg.run();
            switch(result)
            {
                case GTK_RESPONSE_YES: imagestatus=1;
                                        break;
                case GTK_RESPONSE_NO:  imagestatus=0;
            }
        }

        else
            bigimage=false;

        if(imagestatus)
       {
            m_image->set(".image_temp.bmp");
            imageb->set_image_position(Gtk::POS_BOTTOM);
            imageb->set_image(*m_image);
            m_image->show();
        }
        else
            m_image->hide();
    }
    resize(100,100);
}

void Interface::on_click_open_button()
{
    if(imag.is_save==false)
    {
        Gtk::MessageDialog dlg("Do you want to save the image before opening other image? ", false,Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
        dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dlg.set_title("Save?");
        gint result = dlg.run();
        switch(result)
        {
            case GTK_RESPONSE_YES:  dlg.hide();
                                    save();
                                    break;
            case GTK_RESPONSE_NO:   filechoose();
                                    dlg.hide();
                                    break;
            case GTK_RESPONSE_CANCEL: break;
        }
    }
    else
    {
        filechoose();
    }
}

void Interface::save()
{
    imag.is_save=true;
    FileDialog* filec;
    filec= new FileDialog(imag);
}

void Interface::on_click_image_button()
{
        if(!imagestatus)
        {
            m_image->set(".image_temp.bmp");
            imageb->set_image_position(Gtk::POS_BOTTOM);
            imageb->set_image(*m_image);
            imagestatus=1;
            m_image->show();

        }
        else
        {
            m_image->hide();
            imagestatus=0;
        }

}

void Interface::on_click_grayscale_button()
{
    imag.addprocess(1);
    imag.grayscale();
    imag.createim(); //creating image so that it can be displayed
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");//changing image this is temp image we are saving after every action
    }
    imag.no_undo=0; //because last step is now not undo necessary for implementaton of redo indicates redo not possible
    imag.is_save=false; //image is not saved
}

void Interface::on_click_brightness_button()
{
    dialog *popup;
    popup=new dialog;
    popup->operate(&imag,1,m_image,imagestatus,label);
    popup->show();
}
void Interface::on_click_sepia_button()
{
    imag.addprocess(2);
    imag.sepia();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}
void Interface::on_click_negative_button()
{
    imag.negative();
    imag.addprocess(3);
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_blur_button()
{
    dialog *popup;
    popup=new dialog;
    popup->operate(&imag,2,m_image,imagestatus,label);
    popup->show();
}
void Interface::on_click_contrast_button()
{
    imag.addprocess(6);
    imag.contrast();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_rotate_button()
{
    dialog *popup;
    popup=new dialog;
    popup->operate(&imag,3,m_image,imagestatus,label);
    popup->show();
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_flip_horizontal_button()
{
    imag.addprocess(9);
    imag.flip_horizontal();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_flip_vertical_button()
{
    imag.addprocess(10);
    imag.flip_vertical();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_dither_button()
{
    imag.addprocess(11);
    imag.dither();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
    //imag.createim_3bit();
}

void Interface::on_click_edge_detect_button()
{
    imag.addprocess(14);
    imag.edge_detect();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}
void Interface::on_click_posterize_button()
{
    dialog *popup;
    popup=new dialog;
    popup->operate(&imag,4,m_image,imagestatus,label);
    popup->show();
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_salt_pepper_button()
{
    imag.addprocess(13);
    imag.salt_pepper();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}

void Interface::on_click_halftone_button()
{
    imag.addprocess(12);
    imag.halftone();
    imag.createim();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    imag.no_undo=0;
    imag.is_save=false;
}
void Interface::on_click_crop_button()
{
    dialog *popup;
    popup=new dialog;
    popup->operate3(&imag,m_image,imagestatus,label);
    popup->show();
}

void Interface::on_click_undo_button()
{
    if(imag.index==0)
    {
        Gtk::MessageDialog dlg("Already at initial stage", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("Undo Not Possible");
        dlg.run();
    }
    else
    {
        imag.undo(imagorg);
        imag.is_save=false;
        imag.createim();
        if(imagestatus)
        {
            m_image->set(".image_temp.bmp");
        }
        set_label();
    }

}

void Interface::on_click_redo_button()
{
    if(imag.no_undo==0)
    {
        Gtk::MessageDialog dlg("Redo can be used only after using undo", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        dlg.set_title("Redo not possible");
        dlg.run();
    }
    else
    {
        imag.redo();
        imag.is_save=false;
        imag.createim();
        if(imagestatus)
        {
            m_image->set(".image_temp.bmp");
        }
        set_label();
    }

}

void Interface::on_click_quit_button()
{
    if(imag.is_save==true)
    {
        hide();
    }
    else
    {
        Gtk::MessageDialog dlg("Do you want to save the image before closing? ", false,Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
        dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dlg.set_title("Save?");
        gint result = dlg.run();
        switch(result)
        {
            case GTK_RESPONSE_YES:  dlg.hide();
                                    save();
                                    break;
                                    //Not written break as want to execute filechoose() for GTK_RESPONSE yes also
            case GTK_RESPONSE_NO:  hide();
                                    break;

            case GTK_RESPONSE_CANCEL: break;
        }
    }
}

void Interface::set_label() //to change label showing height width and size if anyone is changed
{
    char c[200];
    sprintf(c,"Size : %d bytes            %dX%d pixels",imag.size,imag.width,imag.height);
    Glib::ustring s(c);
    label->set_text(s);
}
