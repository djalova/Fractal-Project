// mandelbrot.h
// Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu

#ifndef GTKMM_MANDELBROT_H
#define GTKMM_MANDELBROT_H
#include <gtkmm/drawingarea.h>

class Mandelbrot : public Gtk::DrawingArea
{
public:
	Mandelbrot();
	virtual ~Mandelbrot();
	virtual void reset();
	void set_quality();
	void set_colorScheme();
	void save_fractal(std::string filepath);
	void set_fractal_mode(int id);
	void zoom_in();
	void zoom_out();
	void calc_pan_factor();
	void pan_left();
	void pan_up();
	void pan_right();
	void pan_down();

protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:
/* Many variables here are 'long double' type in order to be
 able to zoom in farther than most readily-available program.
 Switching to 'double' marginally increases performance. */
	long double pan_factor;
	long double M_LOW_X;
	long double M_HIGH_X; 
	long double M_LOW_Y; 
	long double M_HIGH_Y; 
	Glib::RefPtr<Gdk::Pixbuf> image;
	int colorScheme;
	int RESOLUTION; 
	long double MAX_ITERATION; 
	void getColor(int iter, long double *R, long double *G, long double *B);
	long double colorR;
	long double colorG;
	long double colorB;
	int fractal_mode;
};

#endif
