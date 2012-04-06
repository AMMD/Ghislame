/**/

//#include <stdio.h>
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

static int light_xypad_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);


// SENDER CLASS
class light_xyPad : public Fl_Group {
private:
  double v, s; int t;
  double *rgb;
  const char *host, *udp_port;
  lo_address osco;
public:
  // light_xyPad Ctor
  light_xyPad(int x,int y,int w,int h, char* l) : Fl_Group(x,y,w,h,l) {
    box(FL_FLAT_BOX); color(9); //label("Drag from here");
    rgb = (double *)malloc(3*sizeof(double));

    host = "SC-bassControl";
    udp_port = "7700";

    osco = lo_address_new( host, udp_port );
    add_method();
  }

  const char* _host(){ return host; };
  const char* _port(){ return udp_port; };

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
    lo_server_thread_add_method(((OSCWindow *)wid)->st, inpath, "iff", light_xypad_handler_wrapper, this);
  };

  int light_xypad_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data){
//	((xyPad *)this)->value(argv[0]->i);
//	vx = argv[0]->f;
//	vy = argv[0]->f;
	t = argv[0]->i;
	((Fl_Slider *)(this->child(1)))->value(t);
	s = argv[1]->f;
	v = argv[2]->f;

	

	//	((xyPad *) (this->child(0)))->oldxpos = (int) (width * s);
	//	((xyPad *)(this->child(0)))->oldypos = (int) (height * v);
	this->child(0)->damage(1);
	//	moveCursor((Fl_Box *)(this->child(2)), oldxpos, oldypos);
	this->redraw();
	Fl::flush();
  };

  void configOSC(const char* h, const char* p){
    host = h;
    udp_port = p;
    //    delete osco;
    osco = lo_address_new(host, udp_port);
  };


  lo_address osco_(){ return osco; };

  double v_() {
    return v;
  }

  int v_set(double vset){
    v = vset;
    //    std::cout << "TSV: " << t << " / " << s << " / " << v << std::endl;
  }

  double s_() {
    return s;
  }

  int s_set(double sset){
    s = sset;
    //    std::cout << "TSV: " << t << " / " << s << " / " << v << std::endl;
  }

  int t_() {
    return t;
  }

  int t_set(double tset){
    t = tset;
    //    std::cout << "TSV: " << t << " / " << s << " / " << v << std::endl;
  }

  double *rgb_() {
    return rgb;
  }

  int rgb_set(double *revebe) {
    rgb = revebe;
  }

  double *rvbGen(){
    double valeur = v;
    double saturation = s;
    int teinte = t;
    

    for(int i = 0; i<2; i++) {
      double *rvb;
      double le, me, ne;
      rvb = (double *)malloc(3*sizeof(double));
      double f; int ti;

      ti=(teinte/60)%6;
      f=teinte/60.0-ti;
      
      if(i){
	valeur = 1;
	saturation = 1;
      }


      le = valeur*(1-saturation);
      me = valeur*(1-f*saturation);
      ne = valeur*(1-(1-f)*saturation);
      
      switch(ti){
      case 0:
	rvb[0] = valeur;
	rvb[1] = ne;
	rvb[2] = le;
	break;
      case 1:
	rvb[0] = me;
	rvb[1] = valeur;
	rvb[2] = le;
	break;
      case 2:
	rvb[0] = le;
	rvb[1] = valeur;
	rvb[2] = ne;
      break;
      case 3:
	rvb[0] = le;
	rvb[1] = me;
	rvb[2] = valeur;
	break;
      case 4:
	rvb[0] = ne;
	rvb[1] = le;
	rvb[2] = valeur;
	break;
      case 5:
	rvb[0] = valeur;
	rvb[1] = le;
	rvb[2] = me;
	break;
      }
    
      if(!i)
	rgb = rvb;	
      else
	return rvb;

      free(rvb);
    }
  };


  void sendOSCrgb(){
    char path[1024], tmpath[1024], rpath[1024], gpath[1024], bpath[1024];

    strcpy(path,"/");
    strcpy(tmpath,"/");
    //    strcat(path, this->label());
    
    Fl_Widget* wid=(Fl_Widget *)this;
    while(wid->parent()){
      if(wid->parent()->label()){
	strcat(tmpath, wid->parent()->label());
	strcat(tmpath,path);
	strcpy(path,tmpath);
	strcpy(tmpath,"/");
      }
      wid = wid->parent();
    }

    strcat(path, "light_xyPad");
    lo_send(osco, path, "sfff", this->label(), rgb[0], rgb[1], rgb[2]);
    // strcpy(rpath,path); strcat(rpath, "/red");
    // lo_send(osco, rpath, "f", rgb[0]);

    // strcpy(gpath,path); strcat(gpath,"/green");
    // lo_send(osco, gpath, "f", rgb[1]);

    // strcpy(bpath,path); strcat(bpath,"/blue");
    // lo_send(osco, bpath, "f", rgb[2]);
  };

};

static int light_xypad_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	((light_xyPad *)user_data)->light_xypad_handler(path, types, argv, argc, data);
}


static int xypad_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data);



// SENDER CLASS
class xyPad : public Fl_Box {
private:
	int xpos, ypos, width, height;
	char* label;
	int oldxpos, oldypos;
	double vx, vy;
  OSCoutput *osco;
  const char *host, *port;

  Fl_Box * cursor;

public:
    // xyPad Ctor
    xyPad(int x,int y,int w,int h, char* l) : Fl_Box(x,y,w,h,l) {
        box(FL_FLAT_BOX); color(9); //label("Drag from here");
	oldxpos = 0;
	oldypos = 0;
	vx = 0;
	vy = 0;
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	label = l;
	host = "SC-bassControl";
	port = "7700";
	osco = new OSCoutput( host, port );

	cursor = new Fl_Box(xpos,ypos+height-10,10,10,"cursor");
	cursor->box(FL_DIAMOND_UP_BOX);
	cursor->color((Fl_Color)35);
	cursor->labeltype(FL_NO_LABEL);


	add_method();
    };

  void draw() {
    // cas light_xyPad
    Fl_Widget * papa = this->parent();
    if( dynamic_cast< light_xyPad* >(papa) ){
      light_xyPad * father = (light_xyPad *)papa;
      oldxpos = (int)(width * father->s_());
      oldypos = (int)(height * father->v_());
    }
    Fl_Box::draw();
    cursor->redraw();
  }


  const char* _host(){ return host; };
  const char* _port(){ return port; };

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
    lo_server_thread_add_method(((OSCWindow *)wid)->st, inpath, "ff", xypad_handler_wrapper, this);
  };

  int xypad_handler(const char *path, const char *types, lo_arg **argv, int argc, void *data){
//	((xyPad *)this)->value(argv[0]->i);
	vx = argv[0]->f;
	vy = argv[1]->f;
	oldxpos = (int) (width * vx);
	oldypos = (int) (height * vy);
	//	std::cout << "vx : " << vx << " / vy : " << vy << " / oldxpos : " << oldxpos << " / oldypos : " << oldypos << " / width : "<< width << " / height : "<< height << std::endl;
	moveCursor(xpos+oldxpos, ypos+height-oldypos);
	//	std::cout << "Le curseur a bougé" << std::endl;
  };

	double vx_(){ return vx; };
	double vy_(){ return vy; };

	void v_compute(){
		vx = 1.0 * oldxpos / width;
		vy = 1.0 * oldypos / height;
	};

    OSCoutput *osco_(){ return osco; };

  void configOSC(const char* h, const char* p){
    host = h;
    port = p;
    delete osco;
    osco = new OSCoutput(host, port);
  };



    void moveCursor(int x, int y) {
      	float scale_x, scale_y;
	float coef_x, coef_y;
	int l = cursor->w();
	int h = cursor->h();

	// échelle
	coef_x = 1.0*l/width;
	coef_y = 1.0*h/height;
	scale_x = (1.0-coef_x)*(x-xpos)+xpos;
	scale_y = (1.0-coef_y)*(y-ypos)+ypos;

	if((int)scale_x < xpos)
		scale_x = xpos;
	if((int)scale_y < ypos)
		scale_y = ypos;

	// déplacement curseur
	cursor->hide();
	cursor->position((int)scale_x, (int)scale_y);
	cursor->damage(1);
	cursor->show();
    }

  void sendOSC(){
    char path[1024], tmpath[1024];
    
   //     std::cout << "Envoi OSC, udp_port: " << udp_port << " / wType: " << wType << std::endl;
    
    strcpy(path,"/");
    strcpy(tmpath,"/");
    //    strcat(path, widget->label());
    
    Fl_Widget* wid=(Fl_Widget *)this;
    while(wid->parent()){
      if(wid->parent()->label()){
	strcat(tmpath, wid->parent()->label());
	strcat(tmpath,path);
	strcpy(path,tmpath);
	strcpy(tmpath,"/");
      }
      wid = wid->parent();
    }

    strcat(path, "xyPad");
    lo_send(osco->t_(), path, "sff", label, vx, vy);
  };


    // xyPad event handler
    int handle(int event) {
        int ret = Fl_Box::handle(event);
	int x = xpos;
	int y = ypos;
	int w = width;
	int h = height;
	char* l = label;
	int newx, newy;
	Fl_Box* cursor;

	// On stocke le parent
	Fl_Widget* papa = this->parent();


	// On identifie le curseur
	cursor = (Fl_Box *)this->parent()->child(2);


        switch ( event ) {
            case FL_PUSH:
		// changement de repère
		newx = Fl::event_x()-xpos;
		newy = ypos+h-Fl::event_y();

		int u;
		if(newx != oldxpos) {
			moveCursor(Fl::event_x(),ypos+h-oldypos);
			oldxpos = newx;
			v_compute();

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;
			  
			  father->s_set(1.0*newx/width);

			  father->rvbGen();
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;

			  father->sendOSCrgb();
			} else {
			  this->sendOSC();
			}
		}
		if(newy != oldypos) {
			moveCursor(oldxpos+x, Fl::event_y());
			oldypos = newy;
			v_compute();

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;

			  father->v_set(1.0*newy/height);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;

			  father->sendOSCrgb();

			} else {
			  this->sendOSC();

			}
		}
		ret=1;
                break;
	    case FL_DRAG:
		newx = Fl::event_x()-x;
		newy = y+h-Fl::event_y();

		if(newx != oldxpos && newx >= 0 && newx < w){
			moveCursor(Fl::event_x(), y+h-oldypos);
			oldxpos = newx;
			v_compute();

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;
			  
			  father->s_set(1.0*newx/width);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;

			  father->sendOSCrgb();

			} else {
			  this->sendOSC();

			}
		}
		if(newy != oldypos && newy >= 0 && newy < h){
			moveCursor(oldxpos+x, Fl::event_y());
			oldypos = newy;
			v_compute();

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;

			  father->v_set(1.0*newy/height);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;

			  father->sendOSCrgb();

			} else {
			  this->sendOSC();

			}

		}
		ret=1;
                break;
        }
        return(ret);
    }
};

static int xypad_handler_wrapper(const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data){
	((xyPad *)user_data)->xypad_handler(path, types, argv, argc, data);
}
