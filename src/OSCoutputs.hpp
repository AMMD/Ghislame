#include <iostream>
#include <string.h>
#include <lo/lo.h>

class OSCoutput{
private:
  lo_address t;
  const char* udp_port;
public:
  OSCoutput(const char* h, const char* p){
    udp_port = p;
    t = lo_address_new(h,p);
  };
  ~OSCoutput(){};

  void sendOSC(Fl_Widget* w, int wType);
}
