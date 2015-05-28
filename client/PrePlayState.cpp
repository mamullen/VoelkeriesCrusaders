#include "PrePlayState.h"
#include "GameEngine.h"


int PrePlayState::Initialize()
{
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
	glClearColor(0., 0., 0., 1.);
	glEnable(GL_DEPTH_TEST);

	// Initialize components
	Cam.SetViewport(0, 0, WinX, WinY);
	Cam.ApplyViewport();

	Cam.SetProjection(60.0f, ratio, 0.1f, 1000.0f);
	Cam.ApplyProjectionTransform();

	Cam.SetTranslate(Vector3::g_DefaultCameraTranslate);
	Cam.SetRotate(Vector3::g_DefaultCameraRotate);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	textureID = LoadRAWTexture("ppm/VCLogo.ppm", 1000, 1000);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Update(ClientGame* client) {
	Packet * serverPacket = client->popJoinGameEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strstr(serverEvent, "init") != NULL){
			int numCrusaders,numVampires,phase1,phase2,phase3;
			memcpy(&numCrusaders, serverEvent + 5, sizeof(int));
			memcpy(&numVampires, serverEvent + 9, sizeof(int));
			memcpy(&phase1, serverEvent + 13, sizeof(int));
			memcpy(&phase2, serverEvent + 17, sizeof(int));
			memcpy(&phase3, serverEvent + 21, sizeof(int));
			maxCrusaders = numCrusaders;
			maxVampires = numVampires;
			printf("%d,%d\n", maxCrusaders, maxVampires);

			client->setClientId(objID);
			client->savePhaseTimes(phase1,phase2,phase3);
			printf("Setting ID: %d\n", objID);
		}

		if (strstr(serverEvent, "player") != NULL){
			int team;
			memcpy(&team, serverEvent + 7, sizeof(int));

			printf("Team: %d\n", team);

			char* p_name = serverEvent + 11;
			char* saved_name = new char[strlen(p_name)];
			strcpy(saved_name, p_name);

			printf("Name: %s\n", saved_name);

			if (objID != client->getClientId()){
				client->addPlayer(objID, team, saved_name);
			}
			else{
				curr_team = team;
			}
		}

		if (strstr(serverEvent, "team_full") != NULL){
			team_full = true;
		}

		if (strstr(serverEvent, "game_start") != NULL){
			client->setStateChange("play_state");
		}

		if (strstr(serverEvent, "name_success") != NULL){
			char * n = new char[name.length()];
			strcpy(n, name.c_str());
			client->setClientName(n);
			name_state = false;
		}

		if (strstr(serverEvent, "name_taken") != NULL){
			name_error = true;
			wait_name = false;
		}

		delete serverPacket;
		serverPacket = client->popJoinGameEvent();
	}
}

void drawText(const char *text, int x, int y, float r, float g, float b){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 600, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);
	glColor3f(r,g,b);
	glRasterPos2i(x, y); // raster position in 2D
	for (int i = 0; i<strlen(text); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]); // generation of characters in our text with 9 by 15 GLU font
	}
	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawSquare(float r, float g, float b, float x, float y, float size){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 600, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(x, y);
	glVertex2f(x+size, y);
	glVertex2f(x+size, y+size);
	glVertex2f(x, y+size);
	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawRect(float r, float g, float b, float x, float y, float w, float h){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 600, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void PrePlayState::drawTitleImage(){
	glEnable(GL_TEXTURE_2D);            // Enable textures
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 800, 600, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();        ----Not sure if I need this
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(800, 0);
	glTexCoord2f(1, 1);
	glVertex2f(800, 500);
	glTexCoord2f(0, 1);
	glVertex2f(0, 500);
	glEnd();

	// Making sure we can render 3d again
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D, 0);

}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Draw(ClientGame* client) {

	// Set up glStuff
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.ApplyViewTransform();		// Sets up projection & viewing matrices

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (!name_state){
		drawText("Press 1 to be a Crusader", 100, 20, 1, 1, 1);
		drawText("Press 2 to be a Vampire", 550, 20, 1, 1, 1);

		if (curr_team == 1){
			drawText("You are a Crusader!", 345, 20, 1, 1, 1);
		}
		else if (curr_team == 2){
			drawText("You are a Vampire!", 345, 20, 1, 1, 1);
		}

		if (team_full){
			drawText("Can't join. Team is full.", 325, 50, 1, 1, 1);
		}

		//draw slots
		for (int i = 0; i < maxCrusaders; i++){
			drawRect(.2, .2, .2, 60, 34 + i * 30, 245, 25);
		}
		for (int i = 0; i < maxVampires; i++){
			drawRect(.2, .2, .2, 510, 34 + i * 30, 245, 25);
		}

		//draw other players
		int crusaderCount = 0;
		int vampireCount = 0;

		map<unsigned int, std::pair<int, char*>>::iterator it;
		for (it = client->getPlayers()->begin(); it != client->getPlayers()->end(); it++)
		{
			int teamNum = it->second.first;
			char* pName = it->second.second;
			if (teamNum == 1){
				drawSquare(0, 0, 1, 65, 39 + 30 * crusaderCount, 15);
				drawText(pName, 100, 50 + 30 * crusaderCount, 1, 1, 1);
				crusaderCount++;
			}
			else if (teamNum == 2){
				drawSquare(1, 0, 0, 515, 39 + 30 * vampireCount, 15);
				drawText(pName, 550, 50 + 30 * vampireCount, 1, 1, 1);
				vampireCount++;
			}
		}

		if (curr_team == 0){
			drawRect(.2, .2, .2, 300, 545, 245, 25);
			drawText(client->getClientName(), 315, 561, 0, 1, 0);
		}
		else if (curr_team == 1){
			drawSquare(0, 0, 1, 65, 39 + 30 * crusaderCount, 15);
			drawText(client->getClientName(), 100, 50 + 30 * crusaderCount, 0, 1, 0);
		}
		else if (curr_team == 2){
			drawSquare(1, 0, 0, 515, 39 + 30 * vampireCount, 15);
			drawText(client->getClientName(), 550, 50 + 30 * vampireCount, 0, 1, 0);
		}
	}
	else{
		//drawTitleImage();
		drawText("Username:", 230, 316, 1, 1, 1);
		drawRect(.2, .2, .2, 300, 300, 245, 25);
		drawText(name.c_str(), 315, 316, 0, 1, 0);
		if (name_error){
			drawText("Name already taken!  Choose another name.",280, 250,1,1,1);
		}
		
	}


	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::Input(ClientGame* client) {
	if (!name_state){
		bool choosing = false;
		if (glfwGetKey(window, CHOOSE_CRUSADER)) {
			team_full = false;
			client->addEvent(0, "choose_crusader;", JOIN_GAME);
			choosing = true;
		}

		if (!choosing){
			if (glfwGetKey(window, CHOOSE_VAMPIRE)) {
				team_full = false;
				client->addEvent(0, "choose_vampire;", JOIN_GAME);
			}
		}
	}
	else{
		if (send_name){
			std::string temp = "player_name";
			std::string temp2 = temp + name;
			temp2 += ";";
			client->addEvent(0, (char*)temp2.c_str(), JOIN_GAME);
			send_name = false;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void PrePlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		if (name_state && name.length() > 0 && !wait_name){
			send_name = true;
			wait_name = true;
			name_error = false;
		}
	}

	if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS && name_state){
		if (name.length() > 0 && !wait_name){
			name.pop_back();
		}
	}
}

void PrePlayState::CharCallback(GLFWwindow* window, unsigned int code){
	if (name_state && !wait_name){
		if (code >= 32 && code <= 126){
			char in = (char)code;
			name += in;
		}
	}
}

GLuint PrePlayState::LoadRAWTexture(const char * filename, int width, int height)
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