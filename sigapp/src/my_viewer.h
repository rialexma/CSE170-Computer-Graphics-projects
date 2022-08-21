# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
# include <sig/sn_model.h>
# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	SnGroup* _gLight;
	bool _animating;
	SnTransform* _tShadow1;
	GsVec lightPos = GsVec(75, 75, 75);
	GsModel* torus;
   public :
	MyViewer ( int x, int y, int w, int h, const char* l );
	void build_ui ();
	void add_model ( SnShape* s, GsVec p );
	void build_scene ();
	void show_normals ( bool view );
	void run_animation ();
	void translate();
	void camera_move();
	void rotation();
	void moveObj();
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
	void MyViewer::collisions();
	void trail();
};

