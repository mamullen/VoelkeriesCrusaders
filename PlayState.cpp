////////////////////////////////////////
// PlayState.cpp
//	wesley
//	for handling the play state
//	of the current game
////////////////////////////////////////

#include "PlayState.h"

////////////////////////////////////////////////////////////////////////////////

static PlayState *state;

////////////////////////////////////////////////////////////////////////////////

PlayState::PlayState(GLFWwindow* window) {
	this->window = window;
	Initialize();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Initialize() {
	WinX = 1024;
	WinY = 768;

	LeftDown = MiddleDown = RightDown = BothDown = 0;
	MouseX = MouseY = 0;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor(0.5, 0., 0., 1.);
	glEnable(GL_DEPTH_TEST);

	// Initialize components
	Cam.SetAspect(float(WinX) / float(WinY));

	b1 = new Building(25, -20, -10, -20, -10);
	b2 = new Building(15, 20, 18, 20, 18);
	field.buildingList.push_back(b1);
	field.buildingList.push_back(b2);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Update() {

}

////////////////////////////////////////////////////////////////////////////////

void lightmeup()
{
	GLfloat light1_ambient[] = { .4, .4, .4, 1.0 };
	GLfloat light1_diffuse[] = { .9, .9, .9, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0, 4.0, 0.0, 1.0 };

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.f, 4.f, 0.f);
	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light1_position);

	glEnable(GL_LIGHT0);
	glPopMatrix();
}

void PlayState::Draw() {
	if (BothDown) {
		player.MoveForward(0.01);
	}

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	lightmeup();

	glLoadIdentity();
	player.update();

	glTranslatef(player.getPos().x, player.getPos().y, player.getPos().z);

	// Begin drawing player and scene
    field.createFloor(0, 0);
    field.createFloor(40, 0);
    field.createFloor(-40, 0);
    field.createFloor(40, 40);
    field.createFloor(-40, 40);
    field.createFloor(40, -40);
    field.createFloor(-40, -40);
    field.createFloor(0, 40);
    field.createFloor(0, -40);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input() {
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

void PlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseButton(GLFWwindow* window, int button, int action, int mods) {
	float playerRotation = -player.getRotation();

	if (action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

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

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseMotion(GLFWwindow* window, double xpos, double ypos) {
	int dx = xpos - MouseX;
	int dy = -(ypos - MouseY);

	MouseX = xpos;
	MouseY = ypos;

	// Move camera
	Cam.Update(LeftDown, RightDown, dx, dy);
	if (RightDown) {
		player.setRotation(-Cam.GetAzimuth());
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	const float rate = 0.1f;
	if (yoffset > 0) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f - rate));
	}
	else if (yoffset < 0) {
		Cam.SetDistance(Cam.GetDistance()*(1.0f + rate));
	}
}