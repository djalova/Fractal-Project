// fractal.cc
// Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu
#include <gtkmm.h>
//#include <iostream> // see on_key_pressed()
#include "fractal.h"

// default size for window:
#define W_WIDTH 800
#define W_HEIGHT 600
// default size for drawing area:
#define H1_WIDTH 800
#define H1_HEIGHT 550
// default size for buttons bar:
#define H2_WIDTH 800
#define H2_HEIGHT 50

Fractal::Fractal():
  drawingFrame( "== Use WASD to move == Please send all charitable donations to the World Hunger Fund",Gtk::ALIGN_CENTER,Gtk::ALIGN_CENTER,W_WIDTH/W_HEIGHT,true),
  vbox(Gtk::ORIENTATION_VERTICAL), // holds the two hboxes
  hbox_top(Gtk::ORIENTATION_HORIZONTAL), // holds the drawing area, mandelbrot
  hbox_bot(Gtk::ORIENTATION_HORIZONTAL), // holds all the buttons at the bottom
  button_zoom_in("Zoom In(Z)"), // zooms in a fixed amount
  button_zoom_out("Zoom Out(X)"), // zooms out a fixed amount
  button_save("Save(E)"), // opens a dialog window where the user will be able to save the current screen.
  button_color("Color(C)"), // toggles between color schemes
  button_reset("Reset(R)"), // resets default bounds and values
  button_quality("Quality(Q)"), // toggles the number of iterations
  button_exit("Exit(Esc)"), // exits the program
  mandelbrot() // object of Mandelbrot, which is a subclass of Gtk::DrawingArea
{
	// Main window:
	set_title("Fractal Viewer 20,000 - GNU GPL v3");
	set_default_size(W_WIDTH, W_HEIGHT);
	mandelbrot.set_size_request(H1_WIDTH, H1_HEIGHT);
	// Aspec frame:
	drawingFrame.set_shadow_type(Gtk::ShadowType::SHADOW_NONE); 
	drawingFrame.set_size_request(H1_WIDTH, H1_HEIGHT);
	drawingFrame.add(mandelbrot);
	hbox_top.pack_start(drawingFrame);
	hbox_bot.set_size_request(H2_WIDTH, H2_HEIGHT);
	// Adding buttons to the bottom box (hbox_bot) in this order
	// Button 1:
	hbox_bot.pack_start(button_zoom_in);
	button_zoom_in.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_zoom_in_clicked) );
	// Button 2:
	hbox_bot.pack_start(button_zoom_out);
	button_zoom_out.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_zoom_out_clicked) );
	// Button 3:
	hbox_bot.pack_start(button_save);
	button_save.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_save_clicked) );
	// Button 4:
	hbox_bot.pack_start(button_color);
	button_color.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_color_clicked) );
	// Button 5:
	hbox_bot.pack_start(button_reset);
	button_reset.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_reset_clicked) );
	// Button 6:
	hbox_bot.pack_start(button_quality);
	button_quality.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_quality_clicked) );

	//Fractal selector drop-down box:
	m_refTreeModel = Gtk::ListStore::create(m_Columns);
	m_Combo.set_model(m_refTreeModel);

	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	row[m_Columns.m_col_id] = 1;
	row[m_Columns.m_col_name] = "Mandelbrot";

	row = *(m_refTreeModel->append());
	row[m_Columns.m_col_id] = 2;
	row[m_Columns.m_col_name] = "Julia - 1";
	
	row = *(m_refTreeModel->append());
	row[m_Columns.m_col_id] = 3;
	row[m_Columns.m_col_name] = "Julia - 2";
	
	m_Combo.pack_start(m_Columns.m_col_id);
	m_Combo.pack_start(m_Columns.m_col_name);
	m_Combo.set_active(0); // first item is index 0
	m_Combo.signal_changed().connect( sigc::mem_fun(*this, &Fractal::on_combo_changed) );
	hbox_bot.pack_start(m_Combo);

	// Button 7:
	hbox_bot.pack_start(button_exit);
	button_exit.signal_clicked().connect( sigc::mem_fun(*this,
		&Fractal::on_button_exit_clicked) );

	// Pack hbox_top and hbox_bot into vbox:
	vbox.pack_start(hbox_top);
 	vbox.pack_end(hbox_bot,false,W_WIDTH);
	// Putting everything together into the window:
	add(vbox);
	vbox.signal_key_press_event().connect( sigc::mem_fun(*this, &Fractal::on_key_pressed) );
	show_all_children();
}

Fractal::~Fractal()
{
	// Oh hello there, don't mind me I'm just an empty destructor
}

void Fractal::on_combo_changed()
/* This function detects which fractal mode was selected and switches to it */
{
	//int index = 0; // assuming combo box packs with index of 0 as default
	Gtk::TreeModel::iterator iter = m_Combo.get_active();
	Gtk::TreeModel::Row row = *iter;
	int id = row[m_Columns.m_col_id];
	mandelbrot.set_fractal_mode(id);
	mandelbrot.queue_draw();
}

bool Fractal::on_key_pressed(GdkEventKey* event)
/*
* Main key press handler. Every number for "keyval" was determined by printing "keyval" using the
* commented-out code. 
* These numbers are cross-platform because these are the values that Gtk assigns to the actual keys.
*/
{
	//std::cout << "Key pressed:\t" << event->keyval << std::endl;
	mandelbrot.calc_pan_factor();
	switch(event->keyval)
	{
		case 97: // left (A)
			mandelbrot.pan_left();
			break;
		case 119: // up (W)
			mandelbrot.pan_up();
			break;
		case 100: // right (D)
			mandelbrot.pan_right();
			break;
		case 115: // down (S)
			mandelbrot.pan_down();
			break;
		case 122: // zoom in "Z"
			on_button_zoom_in_clicked();
			break;
		case 120: // zoom out "X"
			on_button_zoom_out_clicked();
			break;
		case 101: // save "E"
			on_button_save_clicked();
			break;
		case 114: // reset "R"
			on_button_reset_clicked();
			break;
		case 113: // quality "Q"
			on_button_quality_clicked();
			break;
		case 99: // color "C"
			on_button_color_clicked();
			break;
		case 65307: // exit "Esc"
			on_button_exit_clicked();
			break;
		default: // optional default case:
			break;
	}
	mandelbrot.queue_draw();
	return false;
}

void Fractal::on_button_zoom_in_clicked()
/* This function zooms in */
{
	mandelbrot.zoom_in();
	mandelbrot.queue_draw();
}

void Fractal::on_button_zoom_out_clicked()
/* This function zooms out */
{
	mandelbrot.zoom_out();
	mandelbrot.queue_draw();
}

void Fractal::on_button_save_clicked()
/* This function brings up the save dialogue */
{
	Gtk::FileChooserDialog dialog("Please choose a file",
		Gtk::FILE_CHOOSER_ACTION_SAVE); 
	dialog.set_transient_for(*this);
	// add response buttons:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK); 
	// add filters so that only  certain file types are shown:
	Glib::RefPtr<Gtk::FileFilter> filter_png = Gtk::FileFilter::create();
	filter_png->set_name(".png files");
	filter_png->add_mime_type("image/png"); // PNG
	dialog.add_filter(filter_png);
	Glib::RefPtr<Gtk::FileFilter> filter_jpeg = Gtk::FileFilter::create();
	filter_jpeg->set_name(".jpeg files");
	filter_jpeg->add_mime_type("image/jpeg"); // JPEG
	dialog.add_filter(filter_jpeg);
	Glib::RefPtr<Gtk::FileFilter> filter_bmp = Gtk::FileFilter::create();
	filter_bmp->set_name(".bmp files");
	filter_bmp->add_mime_type("image/bmp"); // BMP
	dialog.add_filter(filter_bmp);
	Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
	filter_any->set_name("Any files");
	filter_any->add_pattern("*");
	dialog.add_filter(filter_any);
	// show the dialog and wait for input:
	int result = dialog.run();
	// execute the input:
	switch(result)
	{
		case (Gtk::RESPONSE_OK): 
		{
			std::string filepath = dialog.get_filename();
			mandelbrot.save_fractal(filepath);
			break;
		}
		case (Gtk::RESPONSE_CANCEL):
		{
			break;
		}
		default:
			break;
	}
}

void Fractal::on_button_color_clicked()
/* This function changes the color scheme */
{
	mandelbrot.set_colorScheme();
	mandelbrot.queue_draw();
}

void Fractal::on_button_reset_clicked()
/* This function resets important constants */
{
	mandelbrot.reset();
	mandelbrot.queue_draw();
}

void Fractal::on_button_quality_clicked()
/* This function changes the number of iterations */
{
	mandelbrot.set_quality();
	mandelbrot.queue_draw();
}

void Fractal::on_button_exit_clicked()
/* This function quits the program. Adios! */
{
	exit(EXIT_SUCCESS);
}
