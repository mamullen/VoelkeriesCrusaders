#include "PrePlayState.h"


PrePlayState::PrePlayState(GLFWwindow* window)
{
	this->window = window;
	string configWinX;
	string configWinY;
	ConfigSettings::config->getValue("WinX", configWinX);
	ConfigSettings::config->getValue("WinY", configWinY);
	WinX = stoi(configWinX);
	WinY = stoi(configWinY);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor(0.5, 0., 0., 1.);
	glEnable(GL_DEPTH_TEST);

	// Initialize components
	Cam.SetAspect(float(WinX) / float(WinY));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Update(ClientGame* client) {
	Packet * serverPacket = client->popServerEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		//printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strstr(serverEvent, "crusader") != NULL){
			
		}

		if (strstr(serverEvent, "vampire") != NULL){
			
		}

		if (strstr(serverEvent, "team_full") != NULL){
			
		}

		if (strstr(serverEvent, "start_game") != NULL){

		}

		delete serverPacket;
		serverPacket = client->popServerEvent();
	}
}

void drawsomeground2() { // deprecate this one day
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

void PrePlayState::Draw() {

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawsomeground2();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Input(ClientGame* client) {

	if (glfwGetKey(window, CHOOSE_CRUSADER)) {
		client->addEvent(0, "choose_crusader;", JOIN_GAME);
	}

	if (glfwGetKey(window, CHOOSE_VAMPIRE)) {
		client->addEvent(0, "choose_vampire;", JOIN_GAME);
	}
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}