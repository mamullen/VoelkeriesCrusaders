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

PlayState::PlayState(GLFWwindow* window) : GameState(window) {
	Initialize();
	isNight = false;
	p_Light = new Light();
	p_Map = new Map();
	p_Shrine = new Shrine();
	p_regShade = new Shader("shader/shader.vert", "shader/shader.frag");
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
	deathbyparticle = false;
	parti = false;
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

	//normal thing
	m_pTrivialNormalMap.initTexture(GL_TEXTURE_2D, "ppm/normal_up.jpg");

	m_pTrivialNormalMap.Load();

	// Particles
	p_DeathByBlood = new ParticleEffect(100);
	p_DeathByBlood->LoadTexture("./particles/textures/glitter.png");

	ParticleEffect::ColorInterpolator explosion;
	explosion.AddValue(0.0f, Vector4(1, 0, 0, 1)); //red
	explosion.AddValue(0.2f, Vector4(.255, .102, 0, 1)); //orange
	explosion.AddValue(0.33f, Vector4(.255, .255, 0, .75)); //o
	explosion.AddValue(0.4f, Vector4(1, 0, 0, 1)); //red
	explosion.AddValue(0.6f, Vector4(.255, .102, 0, 1)); //orange
	explosion.AddValue(0.33f, Vector4(.255, .255, 0, .5)); //o
	explosion.AddValue(0.8f, Vector4(0, 0, 0, 0.75));
	explosion.AddValue(1.0f, Vector4(0, 0, 0, 0.5));
	explosion.AddValue(1.1f, Vector4(1, 0, 0, .5)); //red
	explosion.AddValue(1.2f, Vector4(.255, .102, 0, .5)); //orange

	p_DeathByBlood->SetColorInterplator(explosion);
	p_DeathByBlood->SetParticleEmitter(&g_ParticleEmitter);
	p_DeathByBlood->EmitParticles();
	p_DeathByBlood->SetCamera(&Cam);
	// End particles

	// Player attacks
	rotationChanged = false;
	attacking = false;
	Player = NULL;
	shader.init("shader/bump.vert", "shader/bump.frag");

	text_picture = LoadRAWTexture("ppm/building.ppm", 1024, 1024);
	text_normalmap = LoadRAWTexture("ppm/building_norm.ppm", 1024, 1024);
	floor_picture = LoadRAWTexture("ppm/floor.ppm", 1024, 1024);
	floor_normalmap = LoadRAWTexture("ppm/floor_norm.ppm", 1024, 1024);

	colortex.initTexture(GL_TEXTURE_2D, "ppm/bleh_C.png");
	normaltex.initTexture(GL_TEXTURE_2D, "ppm/bleh_N.png");

	colortex.Load();
	normaltex.Load();

	colorslant.initTexture(GL_TEXTURE_2D, "ppm/163.png");
	normalslant.initTexture(GL_TEXTURE_2D, "ppm/163_norm.png");

	colorslant.Load();
	normalslant.Load();

	glGenTextures(5, photos);
	t.loadTexture("ppm/c_front.ppm", photos[0]);
	t.loadTexture("ppm/c_back.ppm", photos[1]);
	t.loadTexture("ppm/c_right.ppm", photos[2]);
	t.loadTexture("ppm/c_left.ppm", photos[3]);
	t.loadTexture("ppm/c_top.ppm", photos[4]);

	once = true;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::UpdateParticles() {
	static ElapsedTime elapsedTime;
	float fDeltaTime = elapsedTime.GetElapsedTime();

	p_DeathByBlood->Update(fDeltaTime);
}

void PlayState::UpdateClient(ClientGame* client) {
	Packet * serverPacket = client->popServerEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strcmp(serverEvent, "create") == 0){
			int objectType;
			memcpy(&objectType, serverEvent + 7, sizeof(int));

			printf("CREATING RECEIVED, %d\n",objectType);
			switch (objectType){
			case 0:{//building
				float x1, y1, z1, x2, y2, z2, rot;
				memcpy(&x1, serverEvent + 11, sizeof(float));
				memcpy(&y1, serverEvent + 15, sizeof(float));
				memcpy(&z1, serverEvent + 19, sizeof(float));
				memcpy(&x2, serverEvent + 23, sizeof(float));
				memcpy(&y2, serverEvent + 27, sizeof(float));
				memcpy(&z2, serverEvent + 31, sizeof(float));
				memcpy(&rot, serverEvent + 35, sizeof(float));

				printf("BUILDING: %f,%f,%f,%f,%f,%f,%f\n", x1, y1, z1, x2, y2, z2, rot);

				Vector3* v1 = new Vector3(x1, y1, z1);
				Vector3* v2 = new Vector3(x2, y2, z2);

				b1 = new Building(v1, v2, rot, objID);
				b1->tex = text_picture;
				b1->norm = text_normalmap;
				b1->shade = shader;
				gameObjects.insert(std::pair<int, GameObject*>(objID, b1));
				break;
			}
			case 3://human
			case 4://crusader
			case 5://vampire
				//get init data
				float xPos;
				float yPos;
				float zPos;
				float rot;
				float hp;
				memcpy(&xPos, serverEvent + 11, sizeof(float));
				memcpy(&yPos, serverEvent + 15, sizeof(float));
				memcpy(&zPos, serverEvent + 19, sizeof(float));
				memcpy(&rot, serverEvent + 23, sizeof(float));
				memcpy(&hp, serverEvent + 27, sizeof(float));


				printf("PERSON: %f,%f,%f,%f,%f\n", xPos, yPos, zPos, rot, hp);
				
				//glScalef(0.01, 0.01, 0.01);
				PlayerType* p;
				if (objectType == 3) {
					p = new PlayerType(NULL, objID, 0);
				}
				else if (objectType == 4) {
					p = new CrusaderPlayer(objID, 1);
				}
				else {
					p = new VampirePlayer(objID, 2);
				}
				//glScalef(100, 100, 100);

				//glScalef(10, 10, 10);
				//add shriner here?
				p->setPos(xPos, yPos, zPos);
				p->setRotation(rot);
				p->setMaxHealth(100);
				p->setHealth(hp);
				p->loadShader(p_regShade);

				if (objID == client->getClientId()){
					Player = p;
				}
				else{
					std::map<unsigned int, std::pair<int, char*>>::iterator it = client->getPlayers()->find(objID);
					if (it != client->getPlayers()->end()){
						//attach name
						char* pName = it->second.second;
						p->setName(pName);
					}
					gameObjects.insert(std::pair<int, GameObject*>(objID, p));
				}
				break;
			}
		}

		if (strcmp(serverEvent, "pos") == 0){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 4, sizeof(float));
			memcpy(&yPos, serverEvent + 8, sizeof(float));
			memcpy(&zPos, serverEvent + 12, sizeof(float));

			if (Player && objID == Player->getID()){
				Player->setPos(xPos, yPos, zPos);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setPos(xPos, yPos, zPos);
			}
		}

		if (strcmp(serverEvent, "rot") == 0){
			float rot;
			memcpy(&rot, serverEvent + 4, sizeof(float));
			if (Player && objID == Player->getID()){
				Player->setRotation(rot);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setRotation(rot);
			}
		}

		if (strcmp(serverEvent, "dead") == 0)
		{
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 5, sizeof(float));
			memcpy(&yPos, serverEvent + 9, sizeof(float));
			memcpy(&zPos, serverEvent + 13, sizeof(float));
			particlepos = Vector3(xPos, yPos, zPos);
			deathbyparticle = true;
			parti = true;
		}

		if (strcmp(serverEvent, "hdir") == 0)
		{
			int hDir;
			memcpy(&hDir, serverEvent + 5, sizeof(int));

			if (Player && objID == Player->getID()){
				Player->setHDir(hDir);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				PlayerType* p = (PlayerType*)o;
				p->setHDir(hDir);
			}
		}

		if (strcmp(serverEvent, "vdir") == 0)
		{
			int vDir;
			memcpy(&vDir, serverEvent + 5, sizeof(int));

			if (Player && objID == Player->getID()){
				Player->setVDir(vDir);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				PlayerType* p = (PlayerType*)o;
				p->setVDir(vDir);
			}
		}

		if (strcmp(serverEvent, "particles") == 0)
		{
			printf("truetruetrue\n\n");
			parti = true;

		}


		if (strcmp(serverEvent, "hp") == 0){
			float hp;
			memcpy(&hp, serverEvent + 3, sizeof(float));
			if (Player && objID == Player->getID()){
				Player->setHealth(hp);
			}
			else if (gameObjects.find(objID) != gameObjects.end()){
				GameObject* o = gameObjects.at(objID);
				o->setHealth(hp);
			}
		}

		if (strcmp(serverEvent, "game_time") == 0){
			int time;
			memcpy(&time, serverEvent + 10, sizeof(int));
			currGameTime = time;
		}

		if (strcmp(serverEvent, "game_over") == 0){
			int winner;
			memcpy(&winner, serverEvent + 10, sizeof(int));
			//gameResult: -1 = lose, 0 = tie, 1 = win
			if (Player != NULL){
				if (winner == 0){//tie game
					gameResult = 0;
				}
				else if (winner == Player->getTeam()){
					gameResult = 1;
				}
				else{
					gameResult = -1;
				}
			}
		}

		delete serverPacket;
		serverPacket = client->popServerEvent();
	}
}

void PlayState::Update(ClientGame* client) {
	if (parti)
	{
		UpdateParticles();
	}
	UpdateClient(client);

}

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
void PlayState::RenderParticle(float rot, ParticleEffect* p, float xx, float yy, float zz)
{
	glPushMatrix();
	glTranslatef(xx, yy, zz);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(rot, 0, 1, 0);
	glScalef(0.3f, 0.3f, 0.3f);
	p->Render();
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////

void drawRect(float x, float y, float w, float h){
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
}

void PlayState::drawHUD(ClientGame* client){
	GLint m_viewport[4];
	glGetIntegerv(GL_VIEWPORT, m_viewport);
	GLdouble width = GLdouble(m_viewport[2]-m_viewport[0]);
	GLdouble height = GLdouble(m_viewport[3]-m_viewport[1]);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	//draw top bar
	float p1w = ((float)client->getPhase1Time() / client->getPhase3Time()) * (width / 2);
	float p2w = ((float)(client->getPhase2Time() - client->getPhase1Time()) / client->getPhase3Time()) * (width / 2);
	glColor3f(1, 1, 0);
	drawRect(width / 4, height / 120, p1w, height / 70);
	glColor3f(1, 0, 0);
	drawRect(width/4+p1w, height / 120, p2w, height / 70);
	glColor3f(1, 1, 0);
	drawRect(width/4 + p1w + p2w, height / 120, width / 8, height / 70);
	glEnd();

	//timer triangle
	float timerX = width / 4 + ((float)currGameTime/client->getPhase3Time()) * (width/2);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 1.f, 1.f);
	glVertex2f(timerX,height/120+height/70);
	glVertex2f(timerX + width / 150, height / 120 + height / 70 + height / 60);
	glVertex2f(timerX - width / 150, height / 120 + height / 70 + height / 60);
	glEnd();

	//bottom panel
	glBegin(GL_QUADS);
	/*if (Player->getTeam() == 1){
		glColor3f(.4, .4, 1);
	}
	else{
		glColor3f(1, .4, .4);
	}
	drawRect(0, height - height / 10, width, height / 10);*/
	glEnd();

	//healthbar
	glPushMatrix();
	std::string s = std::to_string((int)Player->getHealth());
	char * healthString = (char*)s.c_str();
	glTranslatef(width / 7, height - height / 19, 1);
	glLineWidth(2);
	glScalef(width/6990.0f, height/4443.07f, 1);
	glRotatef(180, 1, 0, 0);
	glColor3f(1, 1, 1);
	for (int i = 0; i < strlen(healthString); i++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)healthString[i]);
	}
	glPopMatrix();

	glBegin(GL_QUADS);
	glColor3f(0, 1, 0);
	drawRect(width / 15, height - height / 12, (width / 5)*((float)Player->getHealth() / Player->getMaxHealth()), height / 25);
	glColor3f(1, 0, 0);
	drawRect(width/15,height-height/12,width/5, height/25);
	glEnd();
	
	glPushMatrix();
	char * hp = "HP";
	glTranslatef(width / 50, height - height / 20, 0);
	glLineWidth(3);
	glScalef(width/5120.0f,height/3800.0f,1);
	glRotatef(180, 1, 0, 0);
	glColor3f(1, 1, 1);
	for (int i = 0; i < strlen(hp); i++){
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)hp[i]);
	}
	glPopMatrix();

	//player's name
	glPushMatrix();
	char * name = client->getClientName();
	glTranslatef(width / 50, height - height / 10, 0);
	glLineWidth(2);
	glScalef(width/5242.88, height/3800.0f, 1);
	glRotatef(180, 1, 0, 0);
	glColor3f(1, 1, 1);
	for (int i = 0; i < strlen(name); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (char)name[i]);
	}
	glPopMatrix();

	if (gameResult == 1){
		glPushMatrix();
		char * name = "YOU WIN";
		glTranslatef(width / 7.5, height / 4, 0);
		glLineWidth(10);
		glScalef(width / 1024.0f, height / 768.0f, 1);
		glRotatef(180, 1, 0, 0);
		glColor3f(1, 1, 1);
		for (int i = 0; i < strlen(name); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}
		glPopMatrix();
	}
	else if (gameResult == -1){
		glPushMatrix();
		char * name = "YOU LOSE";
		glTranslatef(width / 10, height / 4, 0);
		glLineWidth(10);
		glScalef(width / 1024.0f, height / 768.0f, 1);
		glRotatef(180, 1, 0, 0);
		glColor3f(1, 1, 1);
		for (int i = 0; i < strlen(name); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}
		glPopMatrix();
	}
	else if (gameResult == 0){
		glPushMatrix();
		char * name = "TIE GAME";
		glTranslatef(width / 10, height / 4, 0);
		glLineWidth(10);
		glScalef(width / 1024.0f, height / 768.0f, 1);
		glRotatef(180, 1, 0, 0);
		glColor3f(1, 1, 1);
		for (int i = 0; i < strlen(name); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}
		glPopMatrix();
	}

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Draw(ClientGame* client) {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!Player)
		return;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Draw components
	Cam.ApplyViewTransform();
	glTranslatef(Player->getPos().x, 0, Player->getPos().z);
	glRotatef(180, 0, 1, 0);

	p_Light->Draw(client, currGameTime);
	p_Map->Draw(shader, colortex, normaltex, colorslant, normalslant, m_pTrivialNormalMap, photos);
	p_regShade->bind();
	p_Shrine->Draw();

	//Player->update(true, Cam.GetRotation().y);
	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		((PlayerType*)(it->second))->update(false, Cam.GetRotation().y);
	}
	Player->update(true, Cam.GetRotation().y);
	p_regShade->unbind();

	if (parti)
	{
		RenderParticle(Cam.GetRotation().y, p_DeathByBlood, particlepos.x, particlepos.y, particlepos.z);
	}

	drawHUD(client); //This includes the game over results

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

enum AnimationIndices { a_IDLE, a_RUNFORWARD, a_RUNMELEE, a_STRAFELEFT, a_STRAFEFORWARDLEFT, a_STRAFERIGHT, a_STRAFEFORWARDRIGHT, a_WALKBACK};

void PlayState::Input(ClientGame* client) {
	if (!Player)
		return;
	
	if (glfwGetKey(window, FORWARD) || glfwGetKey(window, BACKWARD))
	{
		if (glfwGetKey(window, FORWARD)) {
			Player->setAnimation(a_RUNFORWARD);
			client->addEvent(Player->getID(), "move_forward;", ACTION_EVENT);
		}
		if (glfwGetKey(window, BACKWARD)) {
			Player->setAnimation(a_WALKBACK);
			client->addEvent(Player->getID(), "move_backward;", ACTION_EVENT);
		}

		if (glfwGetKey(window, STRAFELEFT)) {
			Player->setAnimation(a_STRAFEFORWARDLEFT);
			client->addEvent(Player->getID(), "move_left;", ACTION_EVENT);
		}
		if (glfwGetKey(window, STRAFERIGHT)) {
			Player->setAnimation(a_STRAFEFORWARDRIGHT);
			client->addEvent(Player->getID(), "move_right;", ACTION_EVENT);
		}
	}
	else if (glfwGetKey(window, STRAFELEFT)) {
		Player->setAnimation(a_STRAFELEFT);
		client->addEvent(Player->getID(), "move_left;", ACTION_EVENT);
	}
	else if (glfwGetKey(window, STRAFERIGHT)) {
		Player->setAnimation(a_STRAFERIGHT);
		client->addEvent(Player->getID(), "move_right;", ACTION_EVENT);
	}
	else {
		Player->setAnimation(a_IDLE);
	}

	if (glfwGetKey(window, JUMP)) {
		client->addEvent(Player->getID(), "move_jump;", ACTION_EVENT);
	}
	if (glfwGetKey(window, Q)) {
		client->addEvent(Player->getID(), "q;", ACTION_EVENT);
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
		client->addEvent(Player->getID(), data, ACTION_EVENT);
	}

	if (attacking){
		client->addEvent(Player->getID(), "attack;", ACTION_EVENT);
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void PlayState::CharCallback(GLFWwindow* window, unsigned int code) {
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (!Player)
		return;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		Player->setAnimation(a_RUNMELEE);
		attacking = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
		attacking = false;
		Player->setAnimation(a_IDLE);
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		Player->setAnimation(a_RUNMELEE);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Player->setAnimation(a_IDLE);
	}
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
