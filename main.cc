// main.cc
// Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu
#include "fractal.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "gtkmm.fractalViewer");
	Fractal fractal;
	return app->run(fractal);
}
