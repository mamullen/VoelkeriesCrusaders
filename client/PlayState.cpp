////////////////////////////////////////
// PlayState.cpp
//	wesley
//	for handling the play state
//	of the current game
////////////////////////////////////////

#include "PlayState.h"

////////////////////////////////////////////////////////////////////////////////

static PlayState *state;

Vector3 g_DefaultCameraTranslate(0, 0, 100);
Vector3 g_DefaultCameraRotate(40, 0, 0);

////////////////////////////////////////////////////////////////////////////////

PlayState::PlayState(GLFWwindow* window) {
	this->window = window;
	Initialize();
}

////////////////////////////////////////////////////////////////////////////////

void InitLights() {
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
}

////////////////////////////////////////////////////////////////////////////////

int PlayState::Initialize() {
	double lastTime = glfwGetTime();

	string configWinX;
	string configWinY;
	ConfigSettings::config->getValue("WinX", configWinX);
	ConfigSettings::config->getValue("WinY", configWinY);
	WinX = stoi(configWinX);
	WinY = stoi(configWinY);

	LeftDown = MiddleDown = RightDown = BothDown = 0;
	MouseX = MouseY = 0;

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor(0.5, 0., 0., 1.);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Camera
	Cam.SetTranslate(g_DefaultCameraTranslate);
	Cam.SetRotate(g_DefaultCameraRotate);

	// Particles
	if (g_ParticleEffect.LoadTexture("./particles/Textures/Particle-Texture.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}

	ParticleEffect::ColorInterpolator colors;

	colors.AddValue(0.0f, Vector4(1, 0, 0, 1));     // red
	colors.AddValue(0.15f, Vector4(1, 0, 1, 1));     // magenta
	colors.AddValue(0.33f, Vector4(0, 0, 1, 1));     // blue
	colors.AddValue(0.5f, Vector4(0, 1, 1, 1));     // cyan
	colors.AddValue(0.67f, Vector4(0, 1, 0, 0.75));  // green
	colors.AddValue(0.84f, Vector4(1, 1, 0, 0.5));   // yellow
	colors.AddValue(1.0f, Vector4(1, 0, 0, 0));     // red

	g_ParticleEffect.SetColorInterplator(colors);

	g_ParticleEffect.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect.EmitParticles();
	g_ParticleEffect.SetCamera(&Cam);
	// End particles

	InitLights();
	rotationChanged = false;
	attacking = false;
	player = NULL;
	return 0;
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
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set up glStuff
	glViewport(0, 0, WinX, WinY);

	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;

	if (!player)
		return;

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(player->getPos().x, player->getPos().y, player->getPos().z);
	glRotatef(180, 0, 1, 0);

	player->update(true);

	drawsomeground();

	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		it->second->update(false);
	}

	g_ParticleEffect.Render();

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
		client->addEvent(player->getID(), data);
		//printf("STRING IS %s\n", data);
		//printf("AZIM %f\n", rotate );
		//printf("Player: %f\n", player->getRotation());
	}

	if (attacking){
		client->addEvent(player->getID(), "attack;");
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