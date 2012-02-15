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

static int button_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);



// SENDER CLASS
class OSCButton : public Fl_Button {
private:
  OSCoutput *osco;
  const char *host, *port;
public:
  // light_xyPad Ctor
  OSCButton(int x,int y,int w,int h, char* l) : Fl_Button(x,y,w,h,l) {
    host = "SC-bassControl";
    port = "7700";
    osco = new OSCoutput( host, port );

/*    Fl_Widget *wid = this;
    while(wid->parent()){
	wid = wid->parent();	
    }
    lo_server_thread bserver = ((OSCWindow *)w)->oscs_()->server_();
    lo_server_thread_stop(bserver);
    lo_server_thread_add_method(bserver, "/test/machin", "si", button_handler_wrapper, this);
    lo_server_thread_start(bserver);*/
  }
  ~OSCButton(){};

  OSCoutput *osco_(){ return osco; };

  int button_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data){
	std::cout << "dans le handler du bouton" << std::endl;
  };

  void configOSC(const char* h, const char* p){
    host = h;
    port = p;
    delete osco;
    osco = new OSCoutput(host, port);
  };
};

static int button_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	std::cout << ((Fl_Widget*)user_data)->label() << std::endl;
	((OSCButton *)user_data)->button_handler(path, types, argv, argc, data);
}
