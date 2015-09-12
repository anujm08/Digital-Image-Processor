#include "FileDialog.h"
#include <iostream>
#include<fstream>
#include "dialog.h"

FileDialog::FileDialog(std::ifstream& f,bool&inputstatus,char* c)
{
  Gtk::FileChooserDialog dlg("Please choose a image",Gtk::FILE_CHOOSER_ACTION_OPEN);
  dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dlg.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  //BMP Filter:To select only bmp files
  Glib::RefPtr<Gtk::FileFilter> filter_bmp = Gtk::FileFilter::create();
  filter_bmp->set_name("BMP Images");
  filter_bmp->add_mime_type("image/x-windows-bmp");
  filter_bmp->add_mime_type("image/bmp");
  dlg.add_filter(filter_bmp);

  //Show the dialog and wait for a user response:
  int result = dlg.run();
    if(result==Gtk::RESPONSE_OK)
    {
      int i,j;
      std::string filename = dlg.get_filename();
      const char *cstr = filename.c_str();
      for(i=strlen(cstr);i>0;i--)
      {
        if(cstr[i]=='/')
        {
            i++;
            break;
        }
      }
      for(j=0;i<strlen(cstr);i++,j++)
        c[j]=cstr[i];
      c[j]='\0';
      f.open(cstr);
      if(!f)
      {
        Gtk::MessageDialog dlg("Can't open image", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
        dlg.set_title("Image Opening  Error");
        dlg.run();
        exit(-1);
      }
      inputstatus= true;
    }
  }

FileDialog::FileDialog(Image imag)
{
  Gtk::FileChooserDialog dlg("Please choose a folder",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  dlg.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dlg.add_button("Select", Gtk::RESPONSE_OK);

  int result = dlg.run();
  switch(result)
  {
    case(Gtk::RESPONSE_OK): std::string filename = dlg.get_filename();
                            dialog *popup;
                            popup=new dialog;
                            popup->operate4(imag,filename);
                            popup->show();
                            break;
    }
  }

FileDialog::~FileDialog()
{
    //dtor
}
