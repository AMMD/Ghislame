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
//#include <typeinfo>

// SENDER CLASS
class light_xyPad : public Fl_Group {
private:
	double v, s; int t;
	double *rgb;
public:
  // light_xyPad Ctor
  light_xyPad(int x,int y,int w,int h, char* l) : Fl_Group(x,y,w,h,l) {
    box(FL_FLAT_BOX); color(9); //label("Drag from here");
    rgb = (double *)malloc(3*sizeof(double));
  }
  
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

  void *rvbGen(){
    double valeur = v;
    double saturation = s;
    int teinte = t;
    double *rvb;
    double le, me, ne;
    rvb = (double *)malloc(3*sizeof(double));
    double f; int ti;
    
    
    ti=(teinte/60)%6;
    f=teinte/60.0-ti;
    
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
    
    rgb = rvb;
  }
};

// SENDER CLASS
class xyPad : public Fl_Box {
private:
	int xpos, ypos, width, height;
	char* label;
	int oldxpos, oldypos;
public:
    // xyPad Ctor
    xyPad(int x,int y,int w,int h, char* l) : Fl_Box(x,y,w,h,l) {
        box(FL_FLAT_BOX); color(9); //label("Drag from here");
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	label = l;
    }

    void moveCursor(Fl_Box* c, int x, int y) {
      	float scale_x, scale_y;
	float coef_x, coef_y;
	int l = c->w();
	int h = c->h();

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
	c->hide();
	c->position((int)scale_x, (int)scale_y);
	c->show();
    }

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
		newx = Fl::event_x()-x;
		newy = y+h-Fl::event_y();
		int u;
		if(newx != oldxpos) {
			moveCursor(cursor, Fl::event_x(),y+h-oldypos);
			oldxpos = newx;

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;
			  
			  father->s_set(1.0*newx/width);

			  father->rvbGen();
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;
			} else {
			}
		}
		if(newy != oldypos) {
			moveCursor(cursor, oldxpos+x, Fl::event_y());
			oldypos = newy;

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;

			  father->v_set(1.0*newy/height);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;
			} else {
			}
		}
		ret=1;
                break;
	    case FL_DRAG:
		newx = Fl::event_x()-x;
		newy = y+h-Fl::event_y();

		if(newx != oldxpos && newx >= 0 && newx < w){
			moveCursor(cursor, Fl::event_x(), y+h-oldypos);
			oldxpos = newx;
			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;
			  
			  father->s_set(1.0*newx/width);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;
			} else {
			}
		}
		if(newy != oldypos && newy >= 0 && newy < h){
			moveCursor(cursor, oldxpos+x, Fl::event_y());
			oldypos = newy;

			// Identification de la classe du parent
			if( dynamic_cast< light_xyPad* >(papa) ){
			  light_xyPad* father = (light_xyPad*)papa;

			  father->v_set(1.0*newy/height);

			  father->rvbGen();			  
			  //			  std::cout << "RVB: " << father->rgb_()[0] << " " << father->rgb_()[1] << " " << father->rgb_()[2] << std::endl;
			} else {
			}

		}
		ret=1;
                break;
        }
        return(ret);
    }
};
