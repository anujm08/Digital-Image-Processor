#include<cstdio>
#include<iostream>
#include<gtkmm.h>
#include"Image.h"
#include "Interface.h"

dialog::~dialog()
{
    //dtor
}

dialog::dialog()
{
    //ctor
}

void dialog::operate(Image* imag,int i,Gtk::Image *m_image1,int imagestatus1,Gtk::Label* pixlabel)
{
    label=pixlabel;
    im=imag;
    imagestatus=imagestatus1;
    if(imagestatus)
    {
        m_image=m_image1;
    }
    set_default_size(500, 50);
    set_border_width(10);
    set_position(Gtk::WIN_POS_CENTER);
    set_resizable(false);

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);

    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    vbox->add(*grid);

    Gtk::Label *label = Gtk::manage(new Gtk::Label());
    grid->attach(*label,0,0,2,1);
    Gtk::Entry *bvalue = Gtk::manage(new Gtk::Entry());
    grid->attach(*bvalue, 2, 0, 2, 1);

    Gtk::Button *rotatec = Gtk::manage(new Gtk::Button("Rotate Clockwise 90"));
    Gtk::Button *rotatea = Gtk::manage(new Gtk::Button("Rotate AntiClockwise 90"));
    rotatec->signal_clicked().connect(sigc::mem_fun(*this,&dialog::on_click_rotatec_button));
    rotatea->signal_clicked().connect(sigc::mem_fun(*this,&dialog::on_click_rotatea_button));

    switch(i)
    {
        case 1: set_title("Give Brightness Value between -255 and 255");
                label->set_text("Brightness Value Change By : ");
                bvalue->set_text("10");
                bvalue->set_max_length(4);
                break;
        case 2: set_title("Give index value for blur between 1 and 20");
                label->set_text("Index value of blur :                    ");
                bvalue->set_text("10");
                bvalue->set_max_length(2);
                break;
        case 3: set_title("Give Rotation Angle Value between -360 and 360");
                label->set_text("Rotate Image By (in degrees) :                                   ");
                bvalue->set_text("30");
                bvalue->set_max_length(9);
                grid->attach(*rotatec, 1 ,1, 1, 1);
                grid->attach(*rotatea, 0 ,1, 1, 1);
                break;
        case 4: set_title("Give number of colours between 4 and 16");
                label->set_text("No. of colours in poster :          ");
                bvalue->set_text("4");
                bvalue->set_max_length(2);
                break;
    }
    Gtk::Button *bok = Gtk::manage(new Gtk::Button("OK"));
    bok->signal_clicked().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &dialog::on_bok_click), bvalue,i));
    grid->attach(*bok, 2, 2, 1, 1);

    show_all_children();
}

void dialog::on_bok_click(Gtk::Entry *bvalue,int j)
{
    const char* c;
    unsigned int i;
    int t=0;
    bool correctco=true;
    Glib::ustring s;
    s=bvalue->get_text();
    c=s.data();
    if(strlen(c)==0)
        {
            Gtk::MessageDialog dlg("You cannot leave values empty", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg.set_title("Empty Box");
            dlg.run();
            correctco=false;
        }
    if(correctco==true)
    {
            for(i=0;i<strlen(c);i++)
            {
                if(!(((c[i]>='0')&&(c[i]<='9'))||((c[i]=='-')&&(i==0)&&((j==1)||(j==3)))))
                {
                    Gtk::MessageDialog dlg("Please enter a integer value", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                    dlg.set_title("Non Integer Value");
                    dlg.run();
                    correctco=false;
                    break;
                }
            }
    }
    if(correctco==true)
    {
        sscanf(c,"%d",&t);
        switch(j)
        {
            case 1: if(t<-255||t>255)
                    {
                        Gtk::MessageDialog dlg("Please enter a integer value between -255 and 255", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                        dlg.set_title("Value not in limits");
                        dlg.run();
                        correctco=false;
                    }
                    if(correctco==true)
                    im->brightness(t);
                    im->addprocess(5,t);
                    break;
            case 2: if(t<1||t>20)
                    {
                        Gtk::MessageDialog dlg("Please enter a integer value between 1 and 20", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                        dlg.set_title("Value not in limits");
                        dlg.run();
                        correctco=false;
                    }
                    if(correctco==true)
                    im->blur(t);
                    im->addprocess(4,t);
                    break;
            case 3: if(t<-360||t>360)
                    {
                        Gtk::MessageDialog dlg("Please enter a integer value between -360 and 360", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                        dlg.set_title("Value not in limits");
                        dlg.run();
                        correctco=false;
                    }
                    if(correctco==true)
                    im->rotate(t);
                    im->addprocess(7,t);
                    break;
            case 4: if(t<4||t>16)
                    {
                        Gtk::MessageDialog dlg("Please enter a integer value between 4 and 16", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                        dlg.set_title("Value not in limits");
                        dlg.run();
                        correctco=false;
                    }
                    if(correctco==true)
                    im->posterize(t);
                    im->addprocess(15,t);
                    break;
        }
    }
    if(correctco==true)
    {
        im->no_undo=0;
        im->is_save=false;
        im->createim();
        if(imagestatus)
        {
            m_image->set(".image_temp.bmp");
        }
        hide();
    }
}

void dialog::operate3(Image* imag,Gtk::Image *m_image1,int imagestatus1,Gtk::Label* pixlabel)
{
    label =pixlabel;
    im=imag;
    imagestatus=imagestatus1;
    if(imagestatus)
    {
        m_image=m_image1;
    }
    set_default_size(300, 50);
    set_border_width(10);
    set_position(Gtk::WIN_POS_CENTER);
    set_resizable(false);

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);

    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    vbox->add(*grid);

    set_title("Coordinates of bounding rectangle");
    Gtk::Label *label1 = Gtk::manage(new Gtk::Label("Enter coordinates of initial point  "));
    grid->attach(*label1,0,0,1,1);
    Gtk::Label *label2 = Gtk::manage(new Gtk::Label("Enter coordinates of final point  "));
    grid->attach(*label2,0,1,1,1);
    Gtk::Label *lx1 = Gtk::manage(new Gtk::Label("x:"));
    grid->attach(*lx1,1,0,1,1);
    Gtk::Label *lx2 = Gtk::manage(new Gtk::Label("x:"));
    grid->attach(*lx2,1,1,1,1);
    Gtk::Label *ly1 = Gtk::manage(new Gtk::Label("y:"));
    grid->attach(*ly1,3,0,1,1);
    Gtk::Label *ly2 = Gtk::manage(new Gtk::Label("y:"));
    grid->attach(*ly2,3,1,1,1);

    Gtk::Entry *bx1 = Gtk::manage(new Gtk::Entry());
    grid->attach(*bx1, 2, 0, 1, 1);
    bx1->set_max_length(5);
    bx1->set_text("0");
    Gtk::Entry *bx2 = Gtk::manage(new Gtk::Entry());
    grid->attach(*bx2, 2, 1, 1, 1);
    bx2->set_max_length(5);
    Gtk::Entry *by1 = Gtk::manage(new Gtk::Entry());
    grid->attach(*by1, 4, 0, 1, 1);
    by1->set_max_length(5);
    by1->set_text("0");
    Gtk::Entry *by2 = Gtk::manage(new Gtk::Entry());
    grid->attach(*by2, 4, 1, 1, 1);
    by2->set_max_length(5);

    char c[10];
    sprintf(c,"%d",im->width);
    Glib::ustring s(c);
    bx2->set_text(s);
    sprintf(c,"%d",im->height);
    s=c;
    by2->set_text(s);

    Gtk::Button *bok = Gtk::manage(new Gtk::Button("OK"));
    bok->signal_clicked().connect(sigc::bind<Gtk::Entry*, Gtk::Entry*,Gtk::Entry*,Gtk::Entry*>(sigc::mem_fun(*this, &dialog::on_bok3_click), bx1,bx2,by1,by2));
    grid->attach(*bok, 4, 2, 1, 1);

    show_all_children();
}

void dialog::on_bok3_click(Gtk::Entry *bx1,Gtk::Entry *bx2,Gtk::Entry *by1,Gtk::Entry *by2)
{
    const char* c[4];
    unsigned int i,j;
    int t[4];
    bool correctco=true;
    Glib::ustring s[4];
    s[0]=bx1->get_text();
    s[1]=bx2->get_text();
    s[2]=by1->get_text();
    s[3]=by2->get_text();

    for(i=0;i<4;i++)
    {
        t[i]=0;
        c[i]=s[i].data();
    }
    for(i=0;i<4;i++)
    {
        if(strlen(c[i])==0)
        {
            Gtk::MessageDialog dlg("You cannot leave coordinates empty", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg.set_title("Empty Box");
            dlg.run();
            correctco=false;
            break;
        }

    }
    if(correctco==true)
    {
        for(i=0;i<4;i++)
            for(j=0;j<strlen(c[i]);j++)
            {
                if(!(c[i][j]>='0'&&c[i][j]<='9'))
                {
                    Gtk::MessageDialog dlg("Please enter positive integer values", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
                    dlg.set_title("Non Integer Value");
                    dlg.run();
                    correctco=false;
                    break;
                }
            }
    }
    if(correctco==true)
    {
        for(i=0;i<4;i++)
            sscanf(c[i],"%d",&t[i]);

        if(t[0]<0||t[0]>im->width||t[1]<0||t[1]>im->width||t[2]<0||t[2]>im->height||t[3]<0||t[3]>im->height)
        {
            Gtk::MessageDialog dlg("Please enter a integer value within limits", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK, true);
            dlg.set_title("Value not in limits");
            dlg.run();
            correctco=false;
        }
    }
    if(correctco==true)
    {
        im->no_undo=0;
        im->is_save=false;
        im->addprocess(16,t);
        im->crop(t[0],t[1],t[2],t[3]);
        set_label();
        im->createim();
        if(imagestatus)
        {
            m_image->set(".image_temp.bmp");
        }
        hide();
    }
}

void dialog::operate4(Image imag,std::string filename)
{
    set_default_size(300, 50);
    set_border_width(10);
    set_position(Gtk::WIN_POS_CENTER);
    set_resizable(false);

    Gtk::Box *vbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    add(*vbox);

    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);
    vbox->add(*grid);

    Gtk::Label *label = Gtk::manage(new Gtk::Label("File Name :"));
    grid->attach(*label,0,0,1,1);
    Gtk::Entry *bvalue = Gtk::manage(new Gtk::Entry());
    grid->attach(*bvalue, 1, 0, 1, 1);
    Gtk::Label *label2 = Gtk::manage(new Gtk::Label(".bmp"));
    grid->attach(*label2,2,0,1,1);

    set_title("Save As");
    bvalue->set_text("Untitled");
    bvalue->set_max_length(50);

    Gtk::Button *bok = Gtk::manage(new Gtk::Button("OK"));
    bok->signal_clicked().connect(sigc::bind<Gtk::Entry*>(sigc::mem_fun(*this, &dialog::on_bok4_click), bvalue,imag,filename));
    grid->attach(*bok, 2, 2, 1, 1);
    show_all_children();
}

void dialog::on_bok4_click(Gtk::Entry *bvalue,Image imag,std::string filename)
{
    std::ofstream f;
    Glib::ustring s=bvalue->get_text();
    char c[60];
    sprintf(c,"/%s.bmp",s.data());
    filename.append(c);
    const char *name = filename.c_str();
    f.open(name,std::ios::app);
    if(f.tellp()!=0)
    {
        Gtk::MessageDialog dlg("File with name already present.Do you want to replace?", false,Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
        dlg.set_title("Replace");
        gint result = dlg.run();
        switch(result)
        {
            case GTK_RESPONSE_YES: imag.createim(f);
                                    hide();
                                    break;
            case GTK_RESPONSE_NO:   break;
        }
    }
    else
    {
        imag.createim(f);
        hide();
    }
}

void dialog::on_click_rotatea_button()
{
    im->addprocess(8,1);
    im->rotate90(1);//1 represents anticlockwise
    im->createim();
    im->no_undo=0;
    im->is_save=false;
    set_label();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    hide();
}

void dialog::on_click_rotatec_button()
{
    im->addprocess(8,-1);
    im->rotate90(-1);//-1 represents clockwise
    im->createim();
    im->no_undo=0;
    im->is_save=false;
    set_label();
    if(imagestatus)
    {
        m_image->set(".image_temp.bmp");
    }
    hide();
}

void dialog::set_label()
{
    char c[200];
    sprintf(c,"Size : %d bytes            %dX%d pixels",im->size,im->width,im->height);
    Glib::ustring s(c);
    label->set_text(s);
}
