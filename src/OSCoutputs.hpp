#include <iostream>
#include <string.h>
#include <lo/lo.h>
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Dial.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Roller.H>


#define SLIDER_INT 1
#define BUTTON_INT 2
#define DIAL_INT 3
#define ROLLER_INT 4


class OSCoutput{
private:
  lo_address t;
  const char* udp_port;
public:
  OSCoutput(const char* h, const char* p){
    udp_port = p;
    t = lo_address_new(h,p);
    std::cout << "Création d'une adresse OSC d'envoi" << std::endl;
    std::cout << "Host: " << h << std::endl;
    std::cout << "Port: " << p << std::endl;
  };
  ~OSCoutput(){};

  void sendOSC(Fl_Widget* widget, int wType){
    char path[1024], tmpath[1024];
    
    std::cout << "Envoi OSC, udp_port: " << udp_port << " / wType: " << wType << std::endl;
    
    strcpy(path,"/");
    strcpy(tmpath,"/");
    strcat(path, widget->label());
    
    Fl_Widget* wid=(Fl_Widget *)widget;
    while(wid->parent()){
      strcat(tmpath, wid->parent()->label());
      strcat(tmpath,path);
      strcpy(path,tmpath);
      strcpy(tmpath,"/");
      wid = wid->parent();
    }
    
    
    
    switch(wType){
    case SLIDER_INT:
      std::cout << path << " ,f " << ((Fl_Slider *)widget)->value() << std::endl;
      lo_send(t, path, "f", ((Fl_Slider *)widget)->value());
      break;
    case BUTTON_INT:
      lo_send(t, path, "i", ((Fl_Button *)widget)->value());
      break;
    case DIAL_INT:
      lo_send(t, path, "f", ((Fl_Dial *)widget)->value());
	break;
    case ROLLER_INT:
      lo_send(t, path, "f", ((Fl_Roller *)widget)->value());
      break;
    }
  };


};
