#ifndef FILEDIALOG_H
#define FILEDIALOG_H
#include <gtkmm.h>
#include<fstream>
#include <iostream>
#include"Image.h"

class FileDialog
{
public:
  FileDialog(std::ifstream& f,bool&inputstatus,char* c);
  FileDialog(Image imag);
  virtual ~FileDialog();
  void on_button_file_clicked();
  void on_button_folder_clicked();
};

#endif // FILEDIALOG_H
