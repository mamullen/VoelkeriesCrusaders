////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Voelkeries Crusaders"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
    
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard_down(unsigned char key, int x, int y)		{ TESTER->KeyboardDown(key, x, y); }
static void keyboard_up(unsigned char key, int x, int y)		{ TESTER->KeyboardUp(key, x, y); }
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}
static void mouseroll(int wheel,int dir,int x,int y)	{TESTER->MouseRoll(wheel, dir, x, y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=1024;
	WinY=768;
    
	LeftDown=MiddleDown=RightDown=BothDown=false;
	for (int i = 0; i < 256; i++) { //256 being the size of keys
		keys[i] = false;
	}
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0.5, 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard_down );
	glutKeyboardUpFunc( keyboard_up ); 
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutMouseWheelFunc( mouseroll );
	glutReshapeFunc( resize );

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
    
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	KeyOperations();
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	if (BothDown) {
		player.moveForward(0.01);
	}

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(player.getPos().x, player.getPos().y, player.getPos().z);

	//create ground plane
	glColor3f(0.7, 0.7, 0.7);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, 0, -20);
	glVertex3f(-20, 0, 20);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 0, -20);
	glEnd();
	glPopMatrix();

	player.update();

	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::KeyboardDown(int key, int x, int y) {
	keys[key] = BUTTON_DOWN;
}

////////////////////////////////////////////////////////////////////////////////

void Tester::KeyboardUp(int key, int x, int y) {
	keys[key] = BUTTON_UP;
}

////////////////////////////////////////////////////////////////////////////////

void Tester::KeyOperations() {
	float playerRotation = -player.getRotation();

	if (keys['w']) {
		player.moveForward();
		Cam.SetAzimuth(playerRotation);
	}
	if (keys['a']) {
		player.rotateLeft();
	}
	if (keys['s']) {
		player.moveBackward();
	}
	if (keys['d']) {
		player.rotateRight();
	}
	if (keys['q']) {
		player.moveLeft();
		Cam.SetAzimuth(playerRotation); // needs some kind of fade effect
	}
	if (keys['e']) {
		player.moveRight();
		Cam.SetAzimuth(playerRotation);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(rotate)
    {
        if(btn==GLUT_LEFT_BUTTON) {
			LeftDown = (state == GLUT_DOWN);
			BothDown = RightDown && (state == GLUT_DOWN);
        }
        else if(btn==GLUT_MIDDLE_BUTTON) {
            MiddleDown = (state==GLUT_DOWN);
        }
        else if(btn==GLUT_RIGHT_BUTTON) {
            RightDown = (state==GLUT_DOWN);
			BothDown = LeftDown && (state == GLUT_DOWN);
        }
    }
    else
    {
        if(btn==GLUT_LEFT_BUTTON) {
            LeftDownTwo = (state==GLUT_DOWN);
        } 
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseRoll(int wheel, int dir, int x, int y) {
	const float rate = 0.1f;
	if (dir == MOUSE_UP) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f - rate));
	} else if (dir == MOUSE_DOWN) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f + rate));
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	const float rate = 1.0f;
	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		Cam.SetAzimuth(Cam.GetAzimuth() + dx*rate);
		Cam.SetIncline(Cam.GetIncline() - dy*rate);
		player.setRotation(-Cam.GetAzimuth());
	}
}

////////////////////////////////////////////////////////////////////////////////
