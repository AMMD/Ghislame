#include <stdio.h>
#include <iostream>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <malloc.h>
#include <typeinfo>

// SENDER CLASS
class light_xyPad : public Fl_Group {
private:
	int xpos, ypos, width, height;
	char* label;
	double v, s; int t;
	double *rgb;
public:
    // light_xyPad Ctor
    light_xyPad(int x,int y,int w,int h, char* l) : Fl_Group(x,y,w,h,l) {
        box(FL_FLAT_BOX); color(9); //label("Drag from here");
	xpos = x;
	ypos = y;
	width = w;
	height = h;
	label = l;
	rgb = (double *)malloc(3*sizeof(double));
    }
};
