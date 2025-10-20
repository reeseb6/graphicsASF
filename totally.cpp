// g++ -o totalMidpoint.exe total_midpoint.cpp -O2 -lgdi32
// ./totalMidpoint.exe

#include <iostream>
#include "CImg.h"
#include <vector>

using namespace cimg_library;
using namespace std;


float f(float x, float y, int x0, int y0, int x1, int y1) {
    return (y1 - y0) * x + (x0 - x1) * y + (x1 * y0 - x0 * y1);
}

int main() {
	CImg<unsigned char> image(500, 500, 1, 3, 5);
	
	vector<int> coords_x;
	vector<int> coords_y;
	
	
	CImgDisplay disp1(image, "Click control points");
	unsigned char color[] = { 128,120,250 };
	while (!disp1.is_closed()){
			disp1.wait();
			
			if (disp1.button()){
			
				int x = disp1.mouse_x();
				double y = disp1.mouse_y();
				//vector package
				coords_x.push_back(x);
				coords_y.push_back(y);
				
				cout << x << ", " << y << endl;
				image.draw_circle(x, y, 5, color); 
				
				disp1.display(image);
			}
	}
	
	if (coords_x.size() < 2) return 1; 

	
	int img_height = image.height();
	
	if (coords_x[0] > coords_x[1]){
		int swap_x = coords_x[0];
		coords_x[0] = coords_x[1];
		coords_x[1] = swap_x;
		
		double swap_y = coords_y[0];
		coords_y[0] = coords_y[1];
		coords_y[1] = swap_y;
	}
	
	int x0 = coords_x[0];
    int y0 = coords_y[0];
    int x1 = coords_x[1];
    int y1 = coords_y[1];
	
	int x_0 = coords_x[0];
	int x_1 = coords_x[1];
	int y_0 = (img_height-1) - coords_y[0];
	int y_1 = (img_height-1) - coords_y[1];
	
	double slope = (y_1 - y_0) / (x_1 - x_0);
	
	if (slope >= 0 && slope <= 1){
		int y = y_0;

		for (int x=x_0; x <= x_1; x++){	// go through all x vals
			// when drawing the points, invert the y back to accurately draw line 
			int y_draw = (img_height - 1) - y;
			image.draw_point(x, y_draw, color);
		
			float midpoint = f(x+1, y+0.5, x_0, y_0, x_1, y_1);
		
			// call the midpoint function, f, passing x+1 and y+0.5
			if ( midpoint > 0){
				y++;
			}
		
		}
	}
	
	else if (slope >= -1 && slope < 0){
		int domain = coords_x[1] - coords_x[0]; //used for slope and algorithm
		int range = coords_y[1] - coords_y[0]; //used for slope and algorithm
		double slope = range / domain; //slope, should be between (-1, 0)
	
		double pos_y = coords_y[0]; //tracks current Y position
		int pos_x = coords_x[0]; //tracks current x position
		
		for (int i=0; i < domain-1;i++){
		
			pos_x += 1; //iterate through X values
			image(pos_x, pos_y, 0, 0) = 255; //red 
			image(pos_x, pos_y, 0, 1) = 128; //green
			image(pos_x, pos_y, 0, 2) = 64; //blue

			//algorithm f(x+1,y+0.5)
			double f_xy = ((pos_x + 1) * (range)) - ((pos_y + 0.5) * (domain)) + (coords_y[0] * coords_x[1]) - (coords_y[1] * coords_x[0]);
			if (f_xy > 0){ //if f(x+1,y+0.5) > 0, iterate y
				pos_y = pos_y + 1;
			}
		}
	}
	
	else if( slope > 1){
		if (y0 < y1) {
			int swap_x = x0;
			x0 = x1;
			x1 = swap_x;
		
			int swap_y = y0;
			y0 = y1;
			y1 = swap_y;
		}
	
	
		//slope and other equations used in MidPt formula
		int run = x1 - x0;
		int rise = y0 - y1;
		double slope = rise / run;
	
		//set to first x vlalue
		int x = x0;
	
	
		//loop through the y values
		for(int y = y0; y >= y1; y--){
		
			//algorithm f(x+.5,y+1)
			double MidPt = (rise) * (x + .5) + (run) * y + (x0 * y1 - x1 * y0);
			if(MidPt < 0){
				x = x + 1;
			}
		
			image.draw_point(x, y, color);
		}
	}
	
	else if(slope < -1){
		if (y0 > y1) {
        swap(x0, x1);
        swap(y0, y1);
		}

		//init x
		int x = x0;

		//loop over all y values
		for (int y = y0; y <= y1; y++) {
			image.draw_point(x, y, color);
        
			//calculate midpoint, if about the line move right
			float midpoint = f(x + 0.5, y - 1, x0, y0, x1, y1);
			if (midpoint > 0) {
				x++;
			}
		}
	}
	
	//display
    CImgDisplay disp(image); 
    while (!disp.is_closed())
        disp.wait(); 
	
	return 0;
}
	
