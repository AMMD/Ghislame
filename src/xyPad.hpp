/**/

#include <stdio.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <malloc.h>

// SENDER CLASS
class xyPad : public Fl_Box {
private:
	int xpos, ypos, width, height;
	char* label;
	int oldxpos, oldypos;
	double v, s; int t;
	double *rgb;
public:
    // xyPad Ctor
    xyPad(int x,int y,int w,int h, char* l) : Fl_Box(x,y,w,h,l) {
        box(FL_FLAT_BOX); color(9); //label("Drag from here");
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	label = l;
	rgb = (double *)malloc(3*sizeof(double));
    }

/*    void draw() {

	Fl_Box::draw();

	double *revebe; revebe = (double *)malloc(3*sizeof(double));
	uchar re, gr, bl;

	re = (uchar)(rgb[0]*255);
	gr = (uchar)(rgb[1]*255);
	bl = (uchar)(rgb[2]*255);

	revebe = rvbGen(1, 1, t);
	fl_color((uchar)(revebe[0]*255), (uchar)(revebe[1]*255), (uchar)(revebe[2]*255));
    }*/

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

//	fprintf(stderr,"x: %d / xpos: %d / l: %d / width: %d / scaled: %d \n", x, xpos, l,  width, (int)scale_x);
//	fprintf(stderr,"y: %d / ypos: %d / h: %d / height: %d / scaled: %d \n", y, ypos, h,  height, (int)scale_y);

	// déplacement curseur
	c->hide();
	c->position((int)scale_x, (int)scale_y);
	c->show();
//	fprintf(stderr,"position curseur : x=%d, y=%d\n",c->x(), c->y());
    }

    void getTSV(){
	double saturation, valeur; int teinte;
	Fl_Slider* faderT = (Fl_Slider*)this->parent()->child(1); // curseur de teinte

	valeur = 1.0*oldypos/height;
	saturation = 1.0*oldxpos/width;
	teinte = (int)(faderT->value()*359);
//	fprintf(stderr, "teinte : %d / saturation : %f / valeur : %f\n", teinte, saturation, valeur);

	t = teinte;
	s = saturation;
	v = valeur;
	fprintf(stderr, "teinte : %d / saturation : %f / valeur : %f\n", t, s, v);

    } 

/*    double *rvbGen(double valeur, double saturation, int teinte) {
	double *rvb;
	double le, me, ne;
	rvb = (double *)malloc(3*sizeof(double));
	double f; int ti;

	fprintf(stderr, "teinte : %d / saturation : %f / valeur : %f\n", teinte, saturation, valeur);
	
	ti=(teinte/60)%6;
	f=teinte/60.0-ti;
	
	le = valeur*(1-saturation);
	me = valeur*(1-f*saturation);
	ne = valeur*(1-(1-f)*saturation);

	fprintf(stderr, "f: %f, le: %f, me: %f, ne: %f\n\n", f, le, me, ne);

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
	fprintf(stderr, "r: %f / v: %f / b: %f\n\n", rvb[0], rvb[1], rvb[2]);

	return rvb;

    }
*/

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

	// On identifie le curseur
	cursor = (Fl_Box *)this->parent()->child(2);


        switch ( event ) {
            case FL_PUSH:
		// changement de repère
		newx = Fl::event_x()-x;
		newy = y+h-Fl::event_y();
		int u;
		if(newx != oldxpos) {
//			fprintf(stderr, "%s pushed - x position changed: %d\n", l, newx);
			moveCursor(cursor, Fl::event_x(),y+h-oldypos);
			oldxpos = newx;
			getTSV();
			rgb = rvbGen(v,s,t);
		}
		if(newy != oldypos) {
//			fprintf(stderr, "%s pushed - y position changed: %d\n", l, newy);
			moveCursor(cursor, oldxpos+x, Fl::event_y());
			oldypos = newy;
			getTSV();
			rgb = rvbGen(v,s,t);
		}


		ret=1;
                break;
	    case FL_DRAG:
		newx = Fl::event_x()-x;
		newy = y+h-Fl::event_y();

		if(newx != oldxpos && newx >= 0 && newx < w){
			//fprintf(stderr, "%s dragged - x position changed: %d\n",l, newx);
			moveCursor(cursor, Fl::event_x(), y+h-oldypos);
			oldxpos = newx;
			getTSV();
			rgb = rvbGen(v,s,t);
		}
		if(newy != oldypos && newy >= 0 && newy < h){
			//fprintf(stderr, "%s dragged - y position changed: %d\n",l, newy);
			moveCursor(cursor, oldxpos+x, Fl::event_y());
			oldypos = newy;
			getTSV();
			rgb = rvbGen(v,s,t);
		}
		ret=1;
                break;
        }
        return(ret);
    }
};
