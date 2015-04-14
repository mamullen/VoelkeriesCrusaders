////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Voelkeries Crusaders"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
static GLFWwindow* window;

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glfw call member functions instead of static functions
static void keyboard_down(GLFWwindow* window, int key, int scancode, int action, int mods)		{ TESTER->KeyCallback(window, key, scancode, action, mods); }
static void mouse_button(GLFWwindow* window, int button, int action, int mods)					{ TESTER->MouseButton(window, button, action, mods); }
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)					{ TESTER->MouseMotion(window, xpos, ypos); }
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)					{ TESTER->MouseScroll(window, xoffset, yoffset); }

static void error_callback(int error, const char* description)									{ fputs(description, stderr); }

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	TESTER = new Tester(argc, argv);	//initialize
	TESTER->Loop();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=1024;
	WinY=768;
    
	LeftDown=MiddleDown=RightDown=BothDown=false;
	MouseX=MouseY=0;

	// Create the window
	window = glfwCreateWindow(1366, 768, WINDOWTITLE, NULL, NULL); // pass in glfwGetPrimaryMonitor() to first null for fullscreen
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);	// no vsync

	glfwSetKeyCallback(window, keyboard_down); 
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button);
	glfwSetScrollCallback(window, scroll_callback);

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor( 0.5, 0., 0., 1. );
	glEnable(GL_DEPTH_TEST);

	// Initialize components
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
    
	// Tell glut to re-display the scene
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void drawsomeground() { // deprecate this one day
	glPushMatrix();
	//glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
	//create ground plane
	glTranslatef(0.f, -1.f, 0.f);
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glVertex3f(-20, 0, -20);
	glVertex3f(-20, 0, 20);
	glVertex3f(20, 0, 20);
	glVertex3f(20, 0, -20);
	glEnd();
	glPopMatrix();
}

void Tester::Loop() {
	while (!glfwWindowShouldClose(window))
	{
		if (BothDown) {
			player.MoveForward(0.01);
		}

		// Set up glStuff
		glViewport(0, 0, WinX, WinY);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw components
		Cam.Draw(ratio);		// Sets up projection & viewing matrices

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		player.update();

		glTranslatef(player.getPos().x, player.getPos().y, player.getPos().z);

		// Begin drawing player and scene
		drawsomeground();


		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);

	float playerRotation = -player.getRotation();

	if (glfwGetKey(window, FORWARD)) {
		player.MoveForward();
		Cam.SetAzimuth(playerRotation);
	}

	if (glfwGetKey(window, STRAFELEFT)) {
		player.StrafeLeft();
		Cam.SetAzimuth(playerRotation); // needs some kind of fade effect
	}

	if (glfwGetKey(window, STRAFERIGHT)) {
		player.StrafeRight();
	}

	if (glfwGetKey(window, BACKWARD)) {
		player.MoveBackward();
	}

	if (glfwGetKey(window, ROTATELEFT)) {
		player.rotateLeft();
	}

	if (glfwGetKey(window, ROTATERIGHT)) {
		player.rotateRight();
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(GLFWwindow* window, int button, int action, int mods) {
	float playerRotation = -player.getRotation();

	if (action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if(rotate)
    {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			LeftDown = (action == GLFW_PRESS);
			BothDown = RightDown && (action == GLFW_PRESS);
        }
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			MiddleDown = (action == GLFW_PRESS);
        }
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				Cam.SetAzimuth(playerRotation);
			}
			RightDown = (action == GLFW_PRESS);
			BothDown = LeftDown && (action == GLFW_PRESS);
        }
    }
    else
    {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			LeftDownTwo = (action == GLFW_PRESS);
        } 
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(GLFWwindow* window, double xpos, double ypos) {
	int dx = xpos - MouseX;
	int dy = -(ypos - MouseY);

	MouseX = xpos;
	MouseY = ypos;

	const float rate = 1.0f;
	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		Cam.SetAzimuth(Cam.GetAzimuth() + dx*rate);
		Cam.SetIncline(Cam.GetIncline() - dy*rate);
	}
	if (RightDown) {
		Cam.SetAzimuth(Cam.GetAzimuth() + dx*rate);
		Cam.SetIncline(Cam.GetIncline() - dy*rate);
		player.setRotation(-Cam.GetAzimuth());
	}
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	const float rate = 0.1f;
	if (yoffset > 0) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f - rate));
	} else if (yoffset < 0) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f + rate));
	}
}
