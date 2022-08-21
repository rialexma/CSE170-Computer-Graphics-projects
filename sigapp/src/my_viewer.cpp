
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>
# include <sig/sn_material.h>
# include <stdlib.h>

# include <sigogl/ws_run.h>

float trailX = 0, trailY = 0, trailZ = 0;

int rotate = 0;
float theta = 0;
int cameraMode = 1;
float up = 0;
float left = 0;
float down = 0;
float right = 0;
float actUp = 0, actDown = 0;
float x = 0, y = 0, z = 0;
bool Lturn;
bool Rturn;
bool Uturn;
bool Dturn;
bool collide = false;
bool aUTurn;
bool aDTurn;
int rot = 0;
GsVec p, p1p, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
int SCORE = 0;

MyViewer::MyViewer(int x, int y, int w, int h, const char* l) : WsViewer(x, y, w, h, l)
{
	_nbut = 0;
	_animating = false;
	build_ui();
	build_scene();
}

 void MyViewer::trail() {
	 SnPrimitive* p;
	 p = new SnPrimitive(GsPrimitive::Box, 1, 1, 1); //9+ trail
	 p->prim().material.diffuse = GsColor::gray;
	 add_model(p, GsVec(trailX, trailY, trailZ));
	 
	 

}
void MyViewer::build_ui()
{
	UiPanel* p, * sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel("", UiPanel::HorizLeft);
	p->add(new UiButton("View", sp = new UiPanel()));
	{	UiPanel* p = sp;
	p->add(_nbut = new UiCheckButton("Normals", EvNormals));
	}
	p->add(new UiButton("Animate", EvAnimate));
	p->add(new UiButton("Exit", EvExit)); p->top()->separate();
}



void MyViewer::add_model(SnShape* s, GsVec p)
{
	// This method demonstrates how to add some elements to our scene graph: lines,
	// and a shape, and all in a group under a SnManipulator.
	// Therefore we are also demonstrating the use of a manipulator to allow the user to
	// change the position of the object with the mouse. If you do not need mouse interaction,
	// you can just use a SnTransform to apply a transformation instead of a SnManipulator.
	// You would then add the transform as 1st element of the group, and set g->separator(true).
	// Your scene graph should always be carefully designed according to your application needs.


	SnTransform* manip = new SnTransform;
	GsMat m, m1, res;
	m.translation(p);
	if (rootg()->size() == 3 || rootg()->size() == 4) {
		m1.roty(1, 0);
		res.mult(m, m1);
		manip->set(res);
	}
	else {
		manip->set(m);
	}

	SnGroup* g = new SnGroup;
	SnLines* l = new SnLines;
	l->color(GsColor::orange);
	g->add(manip);
	g->separator(true);
	g->add(s);
	g->add(l);
	// manip->visible(false); // call this to turn off mouse interaction

	rootg()->add(g);
	//gsout << rootg()->size() << gsnl;
}

void MyViewer::build_scene()
{


	SnPrimitive* p;

	p = new SnPrimitive(GsPrimitive::Box, 100, 1, 100); //0) floor (0) red
	p->prim().material.diffuse = GsColor::darkred;
	add_model(p, GsVec(0, -100, 0));

	p = new SnPrimitive(GsPrimitive::Box, 100, 100, 1); //1) wall (1)
	p->prim().material.diffuse = GsColor::gray;
	add_model(p, GsVec(0, 0, -100));

	p = new SnPrimitive(GsPrimitive::Box, 100, 100, 1); //2) wall (2)
	p->prim().material.diffuse = GsColor::gray;
	add_model(p, GsVec(0, 0, 100));

	p = new SnPrimitive(GsPrimitive::Box, 100, 100, 1); //3) wall (3)
	p->prim().material.diffuse = GsColor::gray;
	add_model(p, GsVec(100, 0, 0));

	p = new SnPrimitive(GsPrimitive::Box, 100, 100, 1); //4) wall (4)
	p->prim().material.diffuse = GsColor::gray;
	add_model(p, GsVec(-100, 0, 0));

	p = new SnPrimitive(GsPrimitive::Box, 100, 1, 100); //5) ceiling (5)
	p->prim().material.diffuse = GsColor::green;
	add_model(p, GsVec(0, 100, 0));


	/*p = new SnPrimitive(GsPrimitive::Sphere, 3, 1, 3); //6) food (6)
	p->prim().material.diffuse = GsColor::yellow;
	add_model(p, GsVec(50, 50, 50));*/


	SnModel* k = new SnModel;
	GsModel& torus = *k->model();


	float N = 0.14f; // resolution
	float r = 1; // position range
	float R = 3; // radius of torus
	double x, y, z;
	int i = 0;
	GsModel::Face face;
	GsModel::Face face1;


	// this embedded loop is connecting pts 1,2,3,4 in counter colockwise in order to connect 1,2,3 to make a triangle, 
	//and connecting 1,3,4 in counter clockwise order to make another triangle and then forming a rectangle
	for (float theta = 0; theta < gs2pi; theta = theta + N) {//this goes 360 degrees as if it was a circle
		for (float phi = 0; phi < gs2pi; phi = phi + N) {// this goes 360 degrees asif it was going around a sphere


			// parametric equations for the torus
			x = (R + r * cos(theta)) * (cos(phi));
			y = (R + r * cos(theta)) * (sin(phi));
			z = r * sin(theta);

			torus.V.push() = GsPnt(x, y, z);//pt 1

			face.b = i;
			i++;

			// parametric equations adding deltato theta for the torus 
			x = (R + r * cos(theta + N)) * (cos(phi));
			y = (R + r * cos(theta + N)) * (sin(phi));
			z = r * sin(theta + N);

			torus.V.push() = GsPnt(x, y, z);//pt2

			face.a = i;
			i++;

			// parametric equations adding delta for both theta and phi for the torus
			x = (R + r * cos(theta + N)) * (cos(phi + N));
			y = (R + r * cos(theta + N)) * (sin(phi + N));
			z = r * sin(theta + N);

			torus.V.push() = GsPnt(x, y, z);//pt3

			face.c = i;
			i++;
			torus.F.push() = face;



			// parametric equations adding delta for both theta and phi for the torus
			x = (R + r * cos(theta + N)) * (cos(phi + N));
			y = (R + r * cos(theta + N)) * (sin(phi + N));
			z = r * sin(theta + N);

			torus.V.push() = GsPnt(x, y, z);//pt3

			face1.b = i;
			i++;

			// parametric equations adding delta  phi for the torus
			x = (R + r * cos(theta)) * (cos(phi + N));
			y = (R + r * cos(theta)) * (sin(phi + N));
			z = r * sin(theta);

			torus.V.push() = GsPnt(x, y, z);//pt4

			face1.a = i;
			i++;

			// parametric equations for going back to pt1 for the torus
			x = (R + r * cos(theta)) * (cos(phi));
			y = (R + r * cos(theta)) * (sin(phi));
			z = r * sin(theta);

			torus.V.push() = GsPnt(x, y, z);//pt1

			face1.c = i;
			i++;
			torus.F.push() = face1;

			//smooth shading
			for (int k = i - 6; k < torus.V.size(); k++) {
				torus.N.push() = torus.V[k] / 2;
			}
			torus.set_mode(GsModel::Smooth, GsModel::NoMtl);
		}

	}
	//part2
	GsModel::Group& g = *torus.G.push();
	g.fi = 0;
	g.fn = torus.F.size();
	g.dmap = new GsModel::Texture;

	g.dmap->fname.set("../../kawaii_donut_texture.jpg"); //donut


	//part3
	torus.M.push().init();

	//part4



	//float inc = gs2pi / N;
	for (float theta = 0; theta < gs2pi; theta = theta + N) {//this goes 360 degrees as if it was a circle
		for (float phi = 0; phi < gs2pi; phi = phi + N) {// this goes 360 degrees asif it was going around a sphere

			/*torus.T.push().set(phi/gs2pi, theta/gs2pi);		//this is texture coordinates (phi, theta)
			torus.T.push().set((phi + N) / gs2pi, (theta ) / gs2pi);
			torus.T.push().set((phi + N)/ gs2pi, (theta + N) / gs2pi);
			torus.T.push().set((phi + N) / gs2pi, (theta + N)/ gs2pi);
			torus.T.push().set((phi )/ gs2pi, (theta + N) / gs2pi);
			torus.T.push().set(phi / gs2pi, theta / gs2pi;*/

			torus.T.push().set(theta / gs2pi, phi / gs2pi);		//this is texture coordinates (theta, phi)
			torus.T.push().set((theta + N) / gs2pi, (phi) / gs2pi);
			torus.T.push().set((theta + N) / gs2pi, (phi + N) / gs2pi);
			torus.T.push().set((theta + N) / gs2pi, (phi + N) / gs2pi);
			torus.T.push().set((theta) / gs2pi, (phi + N) / gs2pi);
			torus.T.push().set(theta / gs2pi, phi / gs2pi);


		}

	}


	//gsout << torus.T.size() << " texture coord " << gsnl;
	//gsout << torus.V.size() << " vertices " << gsnl;


	//part5
	torus.set_mode(GsModel::Smooth, GsModel::PerGroupMtl);
	torus.textured = true;
	add_model(k, GsVec(50, 50, 50)); // donut 6

	// Light pos: 7
	SnPrimitive* s = new SnPrimitive;
	s->prim().sphere(5);
	s->prim().nfaces = 20;
	s->prim().material.diffuse = GsColor::yellow;
	_gLight = new SnGroup(new SnTransform, s, true);
	_gLight->get<SnTransform>(0)->get().setrans(lightPos);
	rootg()->add(_gLight);
	//shadow 9
	SnMaterial* snm = new SnMaterial;
	GsMaterial mtl;
	mtl.diffuse = GsColor::black; // shadow color
	snm->material(mtl, 4); // will override the used color for 4 shapes (primitives and lines) that come next
	snm->restore(true);
	SnGroup* dshadowg = new SnGroup; // we use 2 shadow planes (almost the same) to avoid dial and hands overlap
	dshadowg->separator(true);
	dshadowg->add(_tShadow1 = new SnTransform);


	p = new SnPrimitive(GsPrimitive::Box, 2, 2, 2);	//7) snake body (8)
	p->prim().material.diffuse = GsColor::darkgreen;
	add_model(p, GsVec(0, 0, 0));



}



void MyViewer::collisions()
{
	//use geometry?
	//overlap = collision.
	//center + radius?

	//Head
	//Head box( 2, 2, 2 ) to: rootg()->get<SnGroup>(7)->get<SnTransform>(0)->get()(3)

		//walls r=100, floor=0? top=200
	if (rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(3) + 2 >= 100 || rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(3) - 2 <= -100)//x || -x
	{
		gsout << SCORE;
		gs_exit();
	}

	if (rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(7) + 2 >= 100 || rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(7) - 2 <= -100)//y || -y
	{
		gsout << SCORE;
		gs_exit();
	}
	if (rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(11) + 2 >= 100 || rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(11) - 2 <= -100)//z ||-z
	{
		gsout << SCORE;
		gs_exit();
	}


	//Donut( , , )R=3 r=1
	if (//x
		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(3) + 2
		>=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(3) - 3 - 1

		&&

		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(3) - 2
		<=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(3) + 3 + 1


		&&

		//y
		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(7) + 2
		>=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(7) - 3

		&&

		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(7) - 2
		<=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(7) + 3


		&&

		//z
		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(11) + 2
		>=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(11) - 3 - 1

		&&

		rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(11) - 2
		<=
		rootg()->get<SnGroup>(6)->get<SnTransform>(0)->get()(11) + 3 + 1

		)
	{
		SCORE++;
		gsout << "SCORE!" << SCORE;
		collide = true;
		if (collide == true) {
			SnTransform* donut = rootg()->get<SnGroup>(6)->get<SnTransform>(0);
			GsMat donutM = donut->get();
			GsMat move;
			move.translation(static_cast <float> (rand() % 100), static_cast <float> (rand() % 100), static_cast <float> (rand() % 100));
			collide = false;
			donut->set(move);
			render();
		}
	}



}


void MyViewer::camera_move() {


	camera().eye.x = 65;
	camera().eye.y = 65;
	camera().eye.z = -65;
	camera().center.x = 0;
	camera().center.y = 0;
	camera().center.z = 0;
	camera().up.x = -0.293702f;
	camera().up.y = 0.850535f;
	camera().up.z = 0.436211f;
	render();
	ws_check();




	/*{double lt, t0 = gs_time();
		lt = gs_time() - t0;
		rot++;

		//camera().eye.x -= .001f * GS_TORAD(rot);
		//camera().eye.y += .001f * GS_TORAD(rot);
		//camera().eye.z += .001f * GS_TORAD(rot);
		//camera().center.x += GS_TORAD(rot);
		//camera().center.y += GS_TORAD(rot);
		//camera().center.z += GS_TORAD(rot);
		//camera().up.x += .001f * GS_TORAD(rot);
		//camera().up.y += .001f * GS_TORAD(rot);
		//camera().up.z += .001f * GS_TORAD(rot);

		render();
		ws_check();
		message().setf("local time = % f", lt);
	} while (lt < 3.0f);*/

}



// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	if (_animating) return; // avoid recursive calls
	_animating = true;
	double t = gs_time();
	SnTransform* orignalPos = rootg()->get<SnGroup>(8)->get<SnTransform>(0);
	GsMat originalPosM = orignalPos->get();
	if (rootg()->size() == 13) {
		return;
	}

	do {
		for (int i = 8; i < rootg()->size(); i++) {
			if (Uturn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();

				GsMat move, res;
				
				move.translation(up, 0, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();

			}

			else if (Dturn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();

				GsMat move, res;
				//gsout << rootg()->size() << gsnl;
				move.translation(down, 0, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();

			}

			else if (Lturn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();
				GsMat move, res, newRes;
				if (i > 8 && originalPosM != bdyPartMove) {
					move.translation(up, 0, 0);
					res.mult(bdyPartMove, move);
					bdyPart->set(res);
					render();
					continue;
				}
				//gsout << rootg()->size() << gsnl;
				move.roty(1, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();

			}
			else if (Rturn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();
				GsMat move, res;

				if (i > 8 && originalPosM != bdyPartMove) {
					move.translation(up, 0, 0);
					res.mult(bdyPartMove, move);
					bdyPart->set(res);
					render();
					continue;
				}
				//gsout << rootg()->size() << gsnl;
				move.roty(-1, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();

			}
			else if (aUTurn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();
				GsMat move, res;
				if (i > 8 && originalPosM != bdyPartMove) {
					move.translation(up, 0, 0);
					res.mult(bdyPartMove, move);
					bdyPart->set(res);
					render();
					continue;
				}
				move.rotz(1, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();

			}

			else if (aDTurn == true) {
				SnTransform* bdyPart = rootg()->get<SnGroup>(i)->get<SnTransform>(0);
				GsMat bdyPartMove = bdyPart->get();
				GsMat move, res;
				if (i > 8 && originalPosM != bdyPartMove) {
					move.translation(up, 0, 0);
					res.mult(bdyPartMove, move);
					bdyPart->set(res);
					render();
					continue;
				}
				move.rotz(-1, 0);
				res.mult(bdyPartMove, move);

				bdyPart->set(res);
				render();
			}
			else {

			}

		}

	} while (t < 88);
	_animating = false;

	/*
	int ind = gs_random ( 0, rootg()->size()-1 ); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	double frdt = 1.0/30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t=0, lt=0, t0=gs_time();
	do // run for a while:
	{	while ( t-lt<frdt ) { ws_check(); t=gs_time()-t0; } // wait until it is time for next frame
		double yinc = (t-lt)*v;
		if ( t>2 ) yinc=-yinc; // after 2 secs: go down
		lt = t;
		m.e24 += (float)yinc;
		if ( m.e24<0 ) m.e24=0; // make sure it does not go below 0
		manip->initial_mat ( m );
		render(); // notify it needs redraw
		ws_check(); // redraw now
	}	while ( m.e24>0 );
	_animating = false;*/
}

void MyViewer::show_normals(bool view)
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.
	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for (int k = 0; k < r->size(); k++)
	{
		SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if (!view) { l->visible(false); continue; }
		l->visible(true);
		if (!l->empty()) continue; // build only once
		l->init();
		if (s->instance_name() == SnPrimitive::class_name)
		{
			GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face(fn);
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for (int i = 0; i < m.F.size(); i++)
			{
				const GsVec& a = m.V[m.F[i].a]; l->push(a, a + (*n++) * f);
				const GsVec& b = m.V[m.F[i].b]; l->push(b, b + (*n++) * f);
				const GsVec& c = m.V[m.F[i].c]; l->push(c, c + (*n++) * f);
			}
		}
	}
}

int MyViewer::handle_keyboard(const GsEvent& e)
{
	int ret = WsViewer::handle_keyboard(e); // 1st let system check events
	if (ret) return ret;


	switch (e.key)
	{
	case GsEvent::KeyEsc: gs_exit(); return 1;
	case 'n': { bool b = !_nbut->value(); _nbut->value(b); show_normals(b); return 1; }

	case GsEvent::KeySpace:

		/*if (cameraMode == 2)
			cameraMode = 1;
		else
			cameraMode++; */
		camera_move();// 2 is fixed. 1 is moving camera
		break;

	case GsEvent::KeyLeft:  left -= 0.1f; Lturn = true; run_animation(); render(); Lturn = false; collisions(); break;
	case GsEvent::KeyRight:  right += 0.1f; Rturn = true; run_animation(); render(); Rturn = false; collisions(); break;

	case GsEvent::KeyUp:  
		trailX = rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(3);
		trailY = rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(7);
		trailZ = rootg()->get<SnGroup>(8)->get<SnTransform>(0)->get()(11);
		up += 0.25f; Uturn = true; run_animation(); render(); Uturn = false; collisions(); /*trail();*/ break;

	case GsEvent::KeyDown:  down -= 0.1f; Dturn = true; run_animation(); render(); Dturn = false; collisions(); break;
	case 113: actUp += 0.1f; aUTurn = true; run_animation(); render(); aUTurn = false; collisions(); break;
	case 97: actDown += 0.1f; aDTurn = true; run_animation(); render(); aDTurn = false; collisions(); break;

	
	}

	return 0;
}

int MyViewer::uievent(int e)
{
	switch (e)
	{
	case EvNormals: show_normals(_nbut->value()); return 1;
	case EvAnimate: run_animation(); return 1;
	case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
