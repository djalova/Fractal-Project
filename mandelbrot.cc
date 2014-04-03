// mandelbrot.cc
// Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu
#include "mandelbrot.h"
#include "fractal.h"
#include <gtkmm.h>
#include <cmath>
// Default constants:
#define DEFAULT_L_X -2.2 // mathematical coordinates
#define DEFAULT_H_X 0.8 // first quadrant is bottom-right
#define DEFAULT_L_Y -1.0 // but since the mandlebrot set is symmetrical,
#define DEFAULT_H_Y 1.0 // there is no difference
#define DEFAULT_RESOLUTION 0 // initial number of iterations
#define DEFAULT_MAX_ITERATION 60 // initial max iteration
// Other constants:
#define ZOOM_FACTOR 0.7 // percentage used to scale current window
#define PAN_FACTOR 0.1 // percentage used to scale pan distance
#define COLOR_SCHEME_NUM 3 // number of color schemes to toggle
#define NUM_RES_STATE 4 // number of resolution states. e.g. 3 will yield 1-2-3
#define COLOR_VARIETY 10 // how often the color changes as a function of iter
#define TOP_ITERATION 8000 // maximum iterations toggled by the Quality button
#define BITS_PER_SAMPLE 8 // number of bits/sample for Pixbuf rendering, default 8

Mandelbrot::Mandelbrot():
  pan_factor(0), // calculated distance to move
  M_LOW_X(DEFAULT_L_X), // initialze coordinates
  M_HIGH_X(DEFAULT_H_X), 
  M_LOW_Y(DEFAULT_L_Y), 
  M_HIGH_Y(DEFAULT_H_Y),
  image(), // pixbuf
  colorScheme(1), // there are 3 color schemes
  RESOLUTION(DEFAULT_RESOLUTION), 
  MAX_ITERATION(DEFAULT_MAX_ITERATION),
  colorR(0), // 8-bit, 0~255
  colorG(0),
  colorB(0),
  fractal_mode(1) // toggled by change fractal Combo Box
{
	// Don't mind me, I'm just an empty constructor
}

Mandelbrot::~Mandelbrot()
{ 
}

bool Mandelbrot::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
/*
* Main drawing method. Contains the Mandelbrot algorithm.
*/
{	// Makes sure that the drawing scales with window resizes:
	Gtk::Allocation allocation = get_allocation();
	const int w_width = allocation.get_width();
	const int w_height = allocation.get_height();
	// mathematical variables:
	long double x, y, x_temp, x_coord_change, y_coord_change;
	int iteration;
	long double x0 = 0;
	long double y0 = 0;
	x_coord_change = (this->M_HIGH_X - this->M_LOW_X)/w_width;
	y_coord_change = (this->M_HIGH_Y - this->M_LOW_Y)/w_height;
	// RGB buffer. Just an array that's [R,G,B,...] each 8-bit
	guint8 *data = new guint8[w_height*w_width*3]; // 3 for the 3 colors RGB
	int data_i = 0; // index for th RGB buffer

	/* ------------   THE MAIN ALGORITHM   ----------- */
	// Mandelbrot Set algorithm. Builds an RGB for each pixel of the screen
	for (int row = 0; row < w_height; row+=1) {
		for (int col = 0; col < w_width; col+=1) {
			x0 = col * x_coord_change + this->M_LOW_X; // change of coord
			y0 = row * y_coord_change + this->M_LOW_Y; // change of coord
			x = 0;
			y = 0;
			iteration = 0;
			/*	Julia Set switch	*/
			switch (this->fractal_mode) 
			{
				case 1:
					break; // do nothing, fractal mode 1 is mandelbrot
				case 2:
					x = x0;
					y = y0;
					x0 = -0.4; // from the mandelbrot set
					y0 = 0.6; // one specific julia set
					break;
				case 3:
					x = x0;
					y = y0;
					x0 = -0.8; // from the mandelbrot set
					y0 = 0.156; // another specific julia set
					break;
			}
			/*	Back to the Mandelbrot algorithm	*/
			// The heart of the algorithm:
			while ((x*x + y*y < 4) && (iteration < MAX_ITERATION)) {
				x_temp = x*x - y*y + x0;
				y = 2*x*y + y0; 
				x = x_temp;
				iteration += 1;
			}
			// Add colors into the RGB buffer
			getColor(iteration, &colorR, &colorG, &colorB);
			data[data_i] = colorR;
			data[data_i+1] = colorG;
			data[data_i+2] = colorB;
			data_i += 3; // 3 for the 3 colors RGB
		} // end for col
	} // end for row

	// Rendering the RGB buffer:
	Gdk::Colorspace colorspace = Gdk::COLORSPACE_RGB; // creates the RGB colorspace
	this->image = Gdk::Pixbuf::create_from_data(data,colorspace,false,
		BITS_PER_SAMPLE,w_width,w_height,w_width*3);
	Gdk::Cairo::set_source_pixbuf(cr, this->image, 0, 0);
	cr->rectangle(0,0,this->image->get_width(),this->image->get_height());
	cr->fill();
	return true;
}

// public functions:
void Mandelbrot::reset()
/* This function resets variables, triggered by the Reset button */
{
	this->RESOLUTION = DEFAULT_RESOLUTION;
	this->MAX_ITERATION = DEFAULT_MAX_ITERATION;
	this->M_LOW_X = DEFAULT_L_X; 
	this->M_HIGH_X = DEFAULT_H_X; 
	this->M_LOW_Y = DEFAULT_L_Y;
	this->M_HIGH_Y = DEFAULT_H_Y; 
}

void Mandelbrot::set_quality()
/* This function sets the number of iterations, triggered by Quality button */
{
	this->RESOLUTION = this->RESOLUTION % NUM_RES_STATE + 1;
	switch (RESOLUTION) 
	{
		case 1:
			this->MAX_ITERATION = 200;
			break;
		case 2:
			this->MAX_ITERATION = 1000;
			break;
		case 3:
			this->MAX_ITERATION = 3000;
			break;
		case 4:
			this->MAX_ITERATION = TOP_ITERATION;
			break;
	}
}

void Mandelbrot::getColor(int iter, long double *R, long double *G, long double *B)
/* This function calculates an RGB (8bit) color as a function of iterations */
{
	switch (colorScheme)
	{
		case 1: // default rainbow scheme, uses a sine function
			*R = 127.5*sin(((TOP_ITERATION/600)*M_PI/
				this->MAX_ITERATION)*iter)+127.5;
			*G = 127.5*sin(((TOP_ITERATION/600)*M_PI/
				this->MAX_ITERATION)*iter+1)+127.5;
			*B = 127.5*sin(((TOP_ITERATION/600)*M_PI/
				this->MAX_ITERATION)*iter+2)+127.5;
			break;
		case 2: // clear scheme, is a linear function
			*R = 255.0*iter/(1.0*(this->MAX_ITERATION));
			*G = 255.0*iter/(1.6*(this->MAX_ITERATION));
			*B = 255.0*iter/(6*(this->MAX_ITERATION));
			break;
		case 3: // valentime scheme, is a sinusoidal logarithmic function
			*R = 127.5*sin(0.15*(log(iter)+0.5)) + 127.5;
			*G = 127.5*sin(0.15*iter+0.8) + 127.5;
			*B = 127.5*sin(0.15*iter+1.0) + 127.5;
			break;
		default: // optional default case
			break;
	}
	if ((iter==MAX_ITERATION)&&(colorScheme==1)) { // default
		*R = 0; // as a tag-along, sets the negative space to black
		*G = 0;
		*B = 0;
	}
	if ((iter==MAX_ITERATION)&&(colorScheme==3)) { // valentine
		*R = 255; // sets negative space to white, how romantic
		*G = 255;
		*B = 255;
	}
	// the clear scheme doesn't need any negative space setting 
	// because it was designed to run quickly and be clean
}

void Mandelbrot::set_colorScheme()
/* Function to set the color scheme, triggered by Color button */
{
	this->colorScheme = this->colorScheme % COLOR_SCHEME_NUM + 1;
}

void Mandelbrot::save_fractal(std::string filepath)
/* This function saves the current pixbuf stored in "image", triggered by Save button */
{
	std::string filetype = "png"; // default type is .png
	std::string tempType = filepath.substr(filepath.find_last_of(".")+1);
	if (tempType == "png" || tempType == "jpeg" || tempType == "bmp") {
		filetype = tempType;
	}
	this->image->save(filepath,filetype);
}

void Mandelbrot::set_fractal_mode(int id)
/* This function changes to the specified fractal mode, triggered by drop-down box */
{
	this->reset();
	this->fractal_mode = id;
	if (id >= 2) { // Julia sets are symemtrical about the origin, so the coordinates are reset
		this->M_LOW_X = (-1)*(0.5)*(this->M_HIGH_X - this->M_LOW_X);
		this->M_HIGH_X = (1)*(0.5)*(this->M_HIGH_X - this->M_LOW_X);
	}
}

// Basic navigation functions:
void Mandelbrot::zoom_in()
/* This functions zooms into the fractal by calculating new bounds, triggered by Zoom In button */
{
	this->M_LOW_X += (this->M_HIGH_X - this->M_LOW_X)*(1-ZOOM_FACTOR)*(0.5);
	this->M_HIGH_X -= (this->M_HIGH_X - this->M_LOW_X)*(1-ZOOM_FACTOR)*(0.5);
	this->M_LOW_Y += (this->M_HIGH_Y - this->M_LOW_Y)*(1-ZOOM_FACTOR)*(0.5);
	this->M_HIGH_Y -= (this->M_HIGH_Y - this->M_LOW_Y)*(1-ZOOM_FACTOR)*(0.5);
}

void Mandelbrot::zoom_out()
/* This functions zooms into the fractal by calculating new bounds, triggered by Zoom Out button */
{
	this->M_LOW_X -= (this->M_HIGH_X - this->M_LOW_X)*(1-ZOOM_FACTOR)*(0.5);
	this->M_HIGH_X += (this->M_HIGH_X - this->M_LOW_X)*(1-ZOOM_FACTOR)*(0.5);
	this->M_LOW_Y -= (this->M_HIGH_Y - this->M_LOW_Y)*(1-ZOOM_FACTOR)*(0.5);
	this->M_HIGH_Y += (this->M_HIGH_Y - this->M_LOW_Y)*(1-ZOOM_FACTOR)*(0.5);
}

void Mandelbrot::calc_pan_factor()
/* This function calculates the pan factor as a function of the current coordinates */
{
	this->pan_factor = PAN_FACTOR*(this->M_HIGH_X - this->M_LOW_X);
}

void Mandelbrot::pan_left()
/* This function pans left, triggered by 'A' key */
{
	this->M_LOW_X -= pan_factor;
	this->M_HIGH_X -= pan_factor;
}

void Mandelbrot::pan_up()
/* This function pans up, triggered by 'W' key */
{
	this->M_LOW_Y -= pan_factor; // remember the 'initial y' of the drawingArea is "y min"
	this->M_HIGH_Y -= pan_factor;
}

void Mandelbrot::pan_right()
/* This function pans right, triggered by 'D' key */
{
	this->M_LOW_X += pan_factor;
	this->M_HIGH_X += pan_factor;
}

void Mandelbrot::pan_down()
/* This function pans down, triggered by 'S' key */
{
	this->M_LOW_Y += pan_factor;
	this->M_HIGH_Y += pan_factor;
}
