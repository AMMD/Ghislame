#include <iostream>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <malloc.h>
//#include "OSCoutputs.hpp"
#include <lo/lo.h>
void error(int num, const char *msg, const char *path);
// SENDER CLASS
class OSCWindow : public Fl_Window {
private:
public:
  lo_server_thread st;
  // light_xyPad Ctor
  OSCWindow(int w,int h, char* l) : Fl_Window(w,h,l) {
    st = lo_server_thread_new("6666", error);
    lo_server_thread_start(st);
  };
  ~OSCWindow(){};


};
void error(int num, const char *msg, const char *path)
{
	std::cout << "liblo server error " << num << " in path " << path << ": " << msg << std::endl;
}


