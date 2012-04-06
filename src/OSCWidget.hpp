#include <iostream>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <malloc.h>
//#include "OSCoutputs.hpp"
#include <lo/lo.h>

// SENDER CLASS
class OSCWidget : public Fl_Widget {
private:
  OSCoutput *osco;
  const char *host, *port;
public:
  // light_xyPad Ctor
  OSCWidget(int x,int y,int w,int h, char* l) : Fl_Widget(x,y,w,h,l) {
    host = "SC-bassControl";
    port = "7700";
    osco = new OSCoutput( host, port );
  }
  ~OSCWidget(){};

  OSCoutput *osco_(){ return osco; };

  const char* _host(){ return host; };
  const char* _port(){ return port; };

};


