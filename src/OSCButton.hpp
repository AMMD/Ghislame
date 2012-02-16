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
    add_method();
  }

  ~OSCButton(){};


  void add_method(){
    char inpath[1024], tmpath[1024];
    
        //std::cout << "Envoi OSC, udp_port: " << udp_port << " / wType: " << wType << std::endl;
    
    strcpy(inpath,"/");
    strcpy(tmpath,"/");
    
    Fl_Widget* wid=(Fl_Widget *)this;
    while(wid->parent()){
      if(wid->parent()->label()){
	strcat(tmpath, wid->parent()->label());
	strcat(tmpath,inpath);
	strcpy(inpath,tmpath);
	strcpy(tmpath,"/");
      }
      wid = wid->parent();
    }
    strcat(inpath, ((Fl_Widget *)this)->label());
    lo_server_thread_add_method(((OSCWindow *)wid)->st, inpath, "i", button_handler_wrapper, this);
  };

  OSCoutput *osco_(){ return osco; };

  int button_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data){
	((Fl_Button *)this)->value(argv[0]->i);
	Fl::flush();
  };

  void configOSC(const char* h, const char* p){
    host = h;
    port = p;
    delete osco;
    osco = new OSCoutput(host, port);
  };
};

static int button_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	((OSCButton *)user_data)->button_handler(path, types, argv, argc, data);
}
