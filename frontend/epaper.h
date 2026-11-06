#ifndef EPAPER_H
#define EPAPER_H

struct Employee {
  String fileId;
  String dogName;
  int age;
  String department;
  String hint;
};

void epaper_init();
void epaper_task();   
void show_code(String code);
void draw_file_text(int y, String text, bool large = false);
void draw_profile_icon();
void show_employee_file(const Employee& employee);
void reset_show_code();

#endif