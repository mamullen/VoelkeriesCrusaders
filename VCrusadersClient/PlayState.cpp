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

void PlayState::Update(ClientGame* client) {
	const char * serverEvent = client->popServerEvent();
	while (serverEvent != NULL){

		if (strstr(serverEvent, "pos:") != NULL){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 4, sizeof(float));
			memcpy(&yPos, serverEvent + 8, sizeof(float));
			memcpy(&zPos, serverEvent + 12, sizeof(float));

			//char msgbuf[2048];
			//sprintf(msgbuf, "pos: with %f, %f, %f\n", xPos, yPos, zPos);
			//OutputDebugString(msgbuf);

			player.setPos(xPos, yPos, zPos);
		}

		serverEvent = client->popServerEvent();
	}
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

////////////////////////////////////////////////////////////////////////////////

void PlayState::Draw() {
	/*if (BothDown) {
		player.MoveForward(0.01);
	}*/

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player.update();

	glTranslatef(player.getPos().x, player.getPos().y, player.getPos().z);

	drawsomeground();
	// Begin drawing player and scene
    /*field.createFloor(0, 0);
    field.createFloor(40, 0);
    field.createFloor(-40, 0);
    field.createFloor(40, 40);
    field.createFloor(-40, 40);
    field.createFloor(40, -40);
    field.createFloor(-40, -40);
    field.createFloor(0, 40);
    field.createFloor(0, -40);*/

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input(ClientGame* client) {
	float playerRotation = -player.getRotation();

	if (glfwGetKey(window, FORWARD)) {
		client->pushEvent("move_forward;");
		//Cam.SetAzimuth(playerRotation);
	}

	if (glfwGetKey(window, STRAFELEFT)) {
		client->pushEvent("move_left;");
		//Cam.SetAzimuth(playerRotation); // needs some kind of fade effect
	}

	if (glfwGetKey(window, STRAFERIGHT)) {
		client->pushEvent("move_right;");
	}

	if (glfwGetKey(window, BACKWARD)) {
		client->pushEvent("move_backward;");
	}

	if (glfwGetKey(window, ROTATELEFT)) {
		client->pushEvent("rotate_left;");
	}

	if (glfwGetKey(window, ROTATERIGHT)) {
		client->pushEvent("rotate_right;");
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