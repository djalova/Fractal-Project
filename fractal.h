// fractal.h
// Jiarui Wang, jwang158@jhu.edu ; Daniel Jalova, djalova1@jhu.edu
#ifndef _FRACTAL_H
#define _FRACTAL_H

#define H1_WIDTH 800
#define H1_HEIGHT 550
#include "mandelbrot.h"
#include <gtkmm.h>

class Fractal: public Gtk::Window 
{
public:
	Fractal();
	virtual ~Fractal();

protected:
	Gtk::AspectFrame drawingFrame;
	Gtk::Box vbox;
	Gtk::Box hbox_top;
	Gtk::Box hbox_bot;
	Gtk::Button button_zoom_in;
	Gtk::Button button_zoom_out;
	Gtk::Button button_save;
	Gtk::Button button_color;
	Gtk::Button button_reset;
	Gtk::Button button_quality;
	Gtk::Button button_exit;
	Mandelbrot mandelbrot; // "mandelbrot.h"
	bool on_key_pressed(GdkEventKey* event);
	void on_button_zoom_in_clicked();
	void on_button_zoom_out_clicked();
	void on_button_save_clicked();
	void on_button_color_clicked();
	void on_button_reset_clicked();
	void on_button_quality_clicked();
	void on_button_exit_clicked();
	void on_combo_changed();

	class ModelColumns: public Gtk::TreeModel::ColumnRecord
	{
	public:
		ModelColumns()
		{
			add(m_col_id);
			add(m_col_name);
		}
		Gtk::TreeModelColumn<int> m_col_id;
		Gtk::TreeModelColumn<Glib::ustring> m_col_name;
	};

	ModelColumns m_Columns;
	Gtk::ComboBox m_Combo;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
};

#endif
