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

void PlayState::Initialize() {
	string configWinX;
	string configWinY;
	ConfigSettings::config->getValue("WinX", configWinX);
	ConfigSettings::config->getValue("WinY", configWinY);
	WinX = stoi(configWinX);
	WinY = stoi(configWinY);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	rotationChanged = false;
	attacking = false;
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

		if (strstr(serverEvent, "create") != NULL){
			//get init data
			float xPos;
			float yPos;
			float zPos;
			float rot;
			float hp;
			memcpy(&xPos, serverEvent + 7, sizeof(float));
			memcpy(&yPos, serverEvent + 11, sizeof(float));
			memcpy(&zPos, serverEvent + 15, sizeof(float));
			memcpy(&rot, serverEvent + 19, sizeof(float));
			memcpy(&hp, serverEvent + 23, sizeof(float));

			Player* p = new Player(objID);
			p->setPos(xPos, yPos, zPos);
			p->setRotation(rot);
			p->setMaxHealth(100);
			p->setHealth(hp);

			if (objID == client->getClientId()){
				player = p;
			}
			else{
				gameObjects.insert(std::pair<int, GameObject*>(objID, p));
			}
		}

		if (strstr(serverEvent, "pos") != NULL){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 4, sizeof(float));
			memcpy(&yPos, serverEvent + 8, sizeof(float));
			memcpy(&zPos, serverEvent + 12, sizeof(float));

			if (player && objID == player->getID()){
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
			if (player && objID == player->getID()){
				player->setRotation(rot);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setRotation(rot);
			}
		}

		if (strcmp(serverEvent, "hp") == 0){
			float hp;
			memcpy(&hp, serverEvent + 3, sizeof(float));
			if (player && objID == player->getID()){
				player->setHealth(hp);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setHealth(hp);
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

	glTranslatef(player->getPos().x, player->getPos().y, player->getPos().z);
	glRotatef(180, 0, 1, 0);

	player->update(true,Cam.GetAzimuth());

	drawsomeground();
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		it->second->update(false,Cam.GetAzimuth());
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input(ClientGame* client) {
	if (!player)
		return;

	if (glfwGetKey(window, FORWARD)) {
		client->addEvent(player->getID(),"move_forward;",ACTION_EVENT);
		//Cam.SetAzimuth(playerRotation);
	}

	if (glfwGetKey(window, STRAFELEFT)) {
		client->addEvent(player->getID(), "move_left;", ACTION_EVENT);
		//Cam.SetAzimuth(playerRotation); // needs some kind of fade effect
	}

	if (glfwGetKey(window, STRAFERIGHT)) {
		client->addEvent(player->getID(), "move_right;", ACTION_EVENT);
	}

	if (glfwGetKey(window, BACKWARD)) {
		client->addEvent(player->getID(), "move_backward;", ACTION_EVENT);
	}

	if (rotationChanged){
		rotationChanged = false;
		float rotate = -Cam.GetAzimuth() + 360;
		char * floatStr = new char[3];
		sprintf(floatStr, "%3.0d", (int)rotate);
		char * data = new char[PACKET_DATA_LEN];
		int pointer = 0;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, "rotate", 6);
		pointer += 6;
		memcpy_s(data + pointer, PACKET_DATA_LEN - pointer, floatStr, 3);
		pointer += 3;
		data[pointer] = ';';
		pointer++;
		data[pointer] = '\0';
		client->addEvent(player->getID(), data, ACTION_EVENT);
		//printf("STRING IS %s\n", data);
		//printf("AZIM %f\n", rotate );
		//printf("Player: %f\n", player->getRotation());
	}

	if (attacking){
		client->addEvent(player->getID(), "attack;", ACTION_EVENT);
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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		attacking = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		attacking = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	/*
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
	*/
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
	Cam.Update(true, false, dx, dy);
	rotationChanged = true;
	//printf("AZIM %f\n", Cam.GetAzimuth());
	//printf("Player: %f\n", player->getRotation());
	//player->setRotation(-Cam.GetAzimuth());
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