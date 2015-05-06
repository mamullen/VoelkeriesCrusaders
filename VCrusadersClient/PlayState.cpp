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

	player = NULL;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Update(ClientGame* client) {
	Packet * serverPacket = client->popServerEvent();
	
	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		//printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strstr(serverEvent, "new") != NULL){
			if (!player){
				player = new Player(objID);
			}
			else{
				gameObjects.insert(std::pair<int, GameObject*>(objID, new Player(objID)));
			}
		}

		if (strstr(serverEvent, "create") != NULL){
			//get init data
			float xPos;
			float yPos;
			float zPos;
			float rot;
			memcpy(&xPos, serverEvent + 7, sizeof(float));
			memcpy(&yPos, serverEvent + 11, sizeof(float));
			memcpy(&zPos, serverEvent + 15, sizeof(float));
			memcpy(&rot, serverEvent + 19, sizeof(float));

			Player* p = new Player(objID);
			p->setPos(xPos, yPos, zPos);
			p->setRotation(rot);

			gameObjects.insert(std::pair<int, GameObject*>(objID, p));
		}

		if (strstr(serverEvent, "pos") != NULL){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 4, sizeof(float));
			memcpy(&yPos, serverEvent + 8, sizeof(float));
			memcpy(&zPos, serverEvent + 12, sizeof(float));

			//char msgbuf[2048];
			//sprintf(msgbuf, "pos: with %f, %f, %f\n", xPos, yPos, zPos);
			//OutputDebugString(msgbuf);
			if (objID == player->getID()){
				player->setPos(xPos, yPos, zPos);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setPos(xPos, yPos, zPos);
			}
		}

		if (strcmp(serverEvent, "rot") == 0){
			float rot;
			memcpy(&rot, serverEvent + 4, sizeof(float));
			printf("%f\n", rot);
			if (objID == player->getID()){
				player->setRotation(rot);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setRotation(rot);
			}
		}
		delete serverPacket;
		serverPacket = client->popServerEvent();
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
	if (!player)
		return;

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	player->update(true);

	//glTranslatef(player->getPos().x, player->getPos().y, player->getPos().z);

	drawsomeground();
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		it->second->update(false);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input(ClientGame* client) {
	if (!player)
		return;

	if (glfwGetKey(window, FORWARD)) {
		client->addEvent(player->getID(),"move_forward;");
		//Cam.SetAzimuth(playerRotation);
	}

	if (glfwGetKey(window, STRAFELEFT)) {
		client->addEvent(player->getID(), "move_left;");
		//Cam.SetAzimuth(playerRotation); // needs some kind of fade effect
	}

	if (glfwGetKey(window, STRAFERIGHT)) {
		client->addEvent(player->getID(), "move_right;");
	}

	if (glfwGetKey(window, BACKWARD)) {
		client->addEvent(player->getID(), "move_backward;");
	}

	if (glfwGetKey(window, ROTATELEFT)) {
		client->addEvent(player->getID(), "rotate_left;");
	}

	if (glfwGetKey(window, ROTATERIGHT)) {
		client->addEvent(player->getID(), "rotate_right;");
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (!player)
		return;

	float playerRotation = -player->getRotation();

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
	if (!player)
		return;

	int dx = xpos - MouseX;
	int dy = -(ypos - MouseY);

	MouseX = xpos;
	MouseY = ypos;

	// Move camera
	Cam.Update(LeftDown, RightDown, dx, dy);
	if (RightDown) {
		player->setRotation(-Cam.GetAzimuth());
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