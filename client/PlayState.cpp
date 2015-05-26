////////////////////////////////////////
// PlayState.cpp
//	wesley
//	for handling the play state
//	of the current game
////////////////////////////////////////

#include "PlayState.h"

////////////////////////////////////////////////////////////////////////////////

static PlayState *state;

ParticleEffect g_ParticleEffect1(5000);
ParticleEffect g_ParticleEffect2(5000);
ParticleEffect g_ParticleEffect3(5000);
ParticleEffect g_ParticleEffect4(5000);

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

int PlayState::InitGL() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;

	// Background color
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	

}

////////////////////////////////////////////////////////////////////////////////

int PlayState::Initialize() {
	// Configs initialization
	string configWinX;
	string configWinY;
	ConfigSettings::config->getValue("WinX", configWinX);
	ConfigSettings::config->getValue("WinY", configWinY);
	WinX = stoi(configWinX);
	WinY = stoi(configWinY);

	// Initialize GL stuff such as GLEW and background color
	InitGL();

	// Camera
	Cam.SetViewport(0, 0, WinX, WinY);
	Cam.ApplyViewport();

	Cam.SetProjection(60.0f, ratio, 0.1f, 1000.0f);
	Cam.ApplyProjectionTransform();

	Cam.SetTranslate(Vector3::g_DefaultCameraTranslate);
	Cam.SetRotate(Vector3::g_DefaultCameraRotate);

	// Mouse settings
	LeftDown = MiddleDown = RightDown = BothDown = 0;
	MouseX = MouseY = 0;

	// Particles
	if (g_ParticleEffect1.LoadTexture("./particles/textures/butterfly.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}

	if (g_ParticleEffect2.LoadTexture("./particles/textures/glitter.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}

	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}

	if (g_ParticleEffect3.LoadTexture("./particles/textures/circle.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}

	if (g_ParticleEffect4.LoadTexture("./particles/textures/arrow.png"))
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

	g_ParticleEffect1.SetColorInterplator(colors);
	g_ParticleEffect2.SetColorInterplator(colors);
	g_ParticleEffect3.SetColorInterplator(colors);
	g_ParticleEffect4.SetColorInterplator(colors);

	g_ParticleEffect1.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect2.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect3.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect4.SetParticleEmitter(&g_ParticleEmitter);
	g_ParticleEffect1.EmitParticles();
	g_ParticleEffect2.EmitParticles();
	g_ParticleEffect3.EmitParticles();
	g_ParticleEffect4.EmitParticles();
	g_ParticleEffect1.SetCamera(&Cam);

	g_ParticleEffect2.SetCamera(&Cam);
	g_ParticleEffect3.SetCamera(&Cam);
	g_ParticleEffect4.SetCamera(&Cam);
	// End particles

	// Lights
	InitLights();

	// Player attacks
	rotationChanged = false;
	attacking = false;
	player = NULL;
	shader.init("shader/bump.vert", "shader/bump.frag");

	text_picture = LoadRAWTexture("ppm/building.ppm", 1024, 1024);
	text_normalmap = LoadRAWTexture("ppm/building_norm.ppm", 1024, 1024);
	floor_picture = LoadRAWTexture("ppm/floor.ppm", 1024, 1024);
	floor_normalmap = LoadRAWTexture("ppm/floor_norm.ppm", 1024, 1024);
	
	//osgViewer::Viewer w;

	//osgViewer::Viewer view;
	glGenTextures(5, photos);
	t.loadTexture("ppm/c_front.ppm", photos[0]);
	t.loadTexture("ppm/c_back.ppm", photos[1]);
	t.loadTexture("ppm/c_right.ppm", photos[2]);
	t.loadTexture("ppm/c_left.ppm", photos[3]);
	t.loadTexture("ppm/c_top.ppm", photos[4]);

	//osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("");
	once = true;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::UpdateParticles() {
	static ElapsedTime elapsedTime;
	float fDeltaTime = elapsedTime.GetElapsedTime();

	g_ParticleEffect1.Update(fDeltaTime);
	g_ParticleEffect2.Update(fDeltaTime);
	g_ParticleEffect3.Update(fDeltaTime);
	g_ParticleEffect4.Update(fDeltaTime);
}

void PlayState::UpdateClient(ClientGame* client) {
	Packet * serverPacket = client->popServerEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strcmp(serverEvent, "create") == 0){
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


			printf("PERSON: %f,%f,%f,%f,%f\n", xPos, yPos, zPos, rot, hp);
			//glScalef(0.01, 0.01, 0.01);
			Player* p = new Player(objID);
			//glScalef(10, 10, 10);
			//add shriner here?
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

		if (strcmp(serverEvent, "create_building") == 0){
			float x1, y1, z1, x2, y2, z2, rot;
			memcpy(&x1, serverEvent + 16, sizeof(float));
			memcpy(&y1, serverEvent + 20, sizeof(float));
			memcpy(&z1, serverEvent + 24, sizeof(float));
			memcpy(&x2, serverEvent + 28, sizeof(float));
			memcpy(&y2, serverEvent + 32, sizeof(float));
			memcpy(&z2, serverEvent + 36, sizeof(float));
			memcpy(&rot, serverEvent + 40, sizeof(float));

			printf("BUILDING: %f,%f,%f,%f,%f,%f,%f\n", x1, y1, z1, x2, y2, z2, rot);

			Vector3* v1 = new Vector3(x1, y1, z1);
			Vector3* v2 = new Vector3(x2, y2, z2);

			b1 = new Building(v1, v2, rot, objID);
			b1->tex = text_picture;
			b1->norm = text_normalmap;
			b1->shade = shader;
			gameObjects.insert(std::pair<int, GameObject*>(objID,b1));


		}

		if (strcmp(serverEvent, "pos") == 0){
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

void PlayState::Update(ClientGame* client) {
	UpdateParticles();
	UpdateClient(client);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::drawsomeground() { // deprecate this one day
	glPushMatrix();

	shader.bind();
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	int texture_location = glGetUniformLocation(shader.id(), "color_texture");
	glUniform1i(texture_location, 0);
	glBindTexture(GL_TEXTURE_2D, floor_picture);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	int normal_location = glGetUniformLocation(shader.id(), "normal_texture");
	glUniform1i(normal_location, 1);
	glBindTexture(GL_TEXTURE_2D, floor_normalmap);
	//create ground plane
	glTranslatef(0.f, -1.f, 0.f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-180, -1, -180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-180, -1, 180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(180, -1, 180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(180, -1, -180);
	glEnd();
	shader.unbind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, photos[0]);
	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-180, 158, -180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-180, -22, -180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(180, -22, -180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(180, 158, -180);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, photos[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(180, 158, 180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(180, -22, 180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-180, -22, 180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-180, 158, 180);
	glEnd();


	//right
	glBindTexture(GL_TEXTURE_2D, photos[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(180, 158, -180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(180, -22, -180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(180, -22, 180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(180, 158, 180);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, photos[3]);
	glBegin(GL_QUADS);
	glNormal3f(1, 0, 0);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-180, 158, 180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-180, -22, 180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-180, -22, -180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-180, 158, -180);
	glEnd();

	//Top
	glNormal3f(0, -1, 0);
	glBindTexture(GL_TEXTURE_2D, photos[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-180, 158, 180);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-180, 158, -180);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(180, 158, -180);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(180, 158, 180);
	//glDisable(GL_TEXTURE_2D);
	glEnd();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::RenderParticles() {
	glPushMatrix();
	glTranslatef(100.0f, 0.4f, 100.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
	glScalef(0.3f, 0.3f, 0.3f);
	//g_ParticleEffect1.Render();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(100.0f, 0.4f, -100.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
	glScalef(0.3f, 0.3f, 0.3f);
	//g_ParticleEffect2.Render();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0f, 0.4f, 100.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
	glScalef(0.3f, 0.3f, 0.3f);
	//g_ParticleEffect3.Render();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-100.0f, 0.4f, -100.0f);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
	glScalef(0.3f, 0.3f, 0.3f);
	//g_ParticleEffect4.Render();
	glPopMatrix();
}

//////////////////////////////////////

void PlayState::Draw() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!player)
		return;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Draw components
	Cam.ApplyViewTransform();

	glTranslatef(player->getPos().x, player->getPos().y, player->getPos().z);
	glRotatef(180, 0, 1, 0);

	drawsomeground();
	RenderParticles();
	
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		it->second->update(false,Cam.GetRotation().y);
	}

	player->update(true, Cam.GetRotation().y);

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
		float rotate = -Cam.GetRotation().y + 360;
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
	int dx = xpos - MouseX;
	int dy = -(ypos - MouseY);

	const float rate = 1.0f;

	MouseX = xpos;
	MouseY = ypos;

	// Move camera
	Cam.AddPitch(-dy);
	Cam.AddYaw(dx);
	rotationChanged = true;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	const float rate = 3.0f;
	if (yoffset > 0) {
		Cam.TranslateZ(1.0f*rate);
	}
	else if (yoffset < 0) {
		Cam.TranslateZ(-1.0f*rate);
	}
}


GLuint PlayState::LoadRAWTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture); //generate the texture with the loaded data
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters

	//And if you go and use extensions, you can use Anisotropic filtering textures which are of an
	//even better quality, but this will do for now.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Here we are setting the parameter to repeat the texture instead of clamping the texture
	//to the edge of our shape.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture

	return texture; //return whether it was successfull
}
