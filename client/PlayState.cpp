////////////////////////////////////////
// PlayState.cpp
//	wesley
//	for handling the play state
//	of the current game
////////////////////////////////////////

#include "PlayState.h"

////////////////////////////////////////////////////////////////////////////////

static PlayState *state;

bool testLight = false;


////////////////////////////////////////////////////////////////////////////////

PlayState::PlayState(GLFWwindow* window) : GameState(window) {
	Initialize();
	isNight = false;
	p_Light = new Light();
	p_Map = new Map();
	p_Shrine = new Shrine(0); // 0 id is temp for gameobj
	p_SunMace = new SunMace(0, Vector3(-75.f, -1.f, -60.f)); // 0 id is temp for gameobj
	p_DefenseShield = new DefenseShield(0, Vector3(-75.f, 1.f, 60.f)); // 0 id is temp for gameobj
	p_LightningBolt = new LightningBoltAtkSpd(0, Vector3(75.f, -5.f, -60.f)); // 0 id is temp for gameobj
	p_BatSword = new BatSword(0, Vector3(75.f, 0.f, 60.f)); // 0 id is temp for gameobj
	p_DavidsBuilding = new MeshLoader("models/BuildingTestForWes.fbx");
	p_DavidsBuilding->DisableBones();
	p_regShade = new Shader("shader/shader.vert", "shader/shader.frag");
	p_bumpShade = new Shader("shader/bump.vert", "shader/bump.frag");
	weap1 = weap2 = weap3 = weap4 = true;
	pnum1 = pnum2 = pnum3 = pnum4 = 9; // 9 is no one
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
	glEnable(GL_DEPTH_TEST);
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

////////////////////////////////////////////////////////////////////////////////

int PlayState::Initialize() {
	//p_bumpShade->init("shader/bump.vert", "shader/bump.frag");

	savedTime =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();

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
	p_ShrineFire = new ParticleEffect(5000);
	p_ShrineFire->LoadTexture("./particles/textures/glitter.png");

	ParticleEffect::ColorInterpolator explosion; 
	explosion.AddValue(0.0f, Vector4(1, 0, 0, 1));
	explosion.AddValue(0.2f, Vector4(1, 0, 0, 1));
	explosion.AddValue(0.4f, Vector4(1, 0.5, 0, 1));
	explosion.AddValue(0.6f, Vector4(1, 0.5, 0, 1));
	explosion.AddValue(0.8f, Vector4(1, 1, 0, 1));
	explosion.AddValue(1.0f, Vector4(1, 1, 0, 1));

	p_ShrineFire->SetColorInterplator(explosion);
	p_ShrineFire->SetParticleEmitter(&g_ParticleEmitter);
	p_ShrineFire->EmitParticles();
	p_ShrineFire->SetCamera(&Cam);

	p_Sparks = new ParticleEffect(10);
	p_Sparks->LoadTexture("./particles/textures/sparks.png");
	ParticleEffect::ColorInterpolator sparkle;
	sparkle.AddValue(0.0f, Vector4(1, 1, 1, 1));
	sparkle.AddValue(0.2f, Vector4(0, 1, 0, 1));
	sparkle.AddValue(0.4f, Vector4(0, 0, 1, 1));
	p_Sparks->SetColorInterplator(sparkle);
	p_Sparks->SetParticleEmitter(&g_ParticleEmitter);
	p_Sparks->EmitParticles();
	p_Sparks->SetCamera(&Cam);

	// End particles

	// Player attacks
	rotationChanged = false;
	attacking = false;
	Player = NULL;

	text_picture = LoadRAWTexture("ppm/building.ppm", 1024, 1024);
	text_normalmap = LoadRAWTexture("ppm/building_norm.ppm", 1024, 1024);
	//floor_picture = LoadRAWTexture("ppm/floor.ppm", 1024, 1024);
	//floor_normalmap = LoadRAWTexture("ppm/floor_norm.ppm", 1024, 1024);

	colorbuild.initTexture(GL_TEXTURE_2D, "ppm/building.png");
	normalbuild.initTexture(GL_TEXTURE_2D, "ppm/building_norm.png");

	colorbuild.Load();
	normalbuild.Load();

	glGenTextures(5, photos);
	t.loadTexture("ppm/i_ft.ppm", photos[0]);
	t.loadTexture("ppm/i_bk.ppm", photos[1]);
	t.loadTexture("ppm/i_lf.ppm", photos[2]);
	t.loadTexture("ppm/i_rt.ppm", photos[3]);
	t.loadTexture("ppm/i_top.ppm", photos[4]);

	once = true;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::UpdateParticles() {
	static ElapsedTime elapsedTime;
	float fDeltaTime = elapsedTime.GetElapsedTime();

	p_ShrineFire->Update(fDeltaTime);
	p_Sparks->Update(fDeltaTime);
}

enum ObjectTypes {BUILDING, none1, none2, HUMAN, CRUSADER, VAMPIRE, SUNMACE};

void PlayState::UpdateClient(ClientGame* client) {
	//do timing stuff here... for now?
	unsigned int currTime =
		std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now().time_since_epoch()).count();
	unsigned int elapsedTime = currTime - savedTime;
	savedTime = currTime;

	damagedTime -= elapsedTime;

	////////////////////////////////////////////////////////////////////

	Packet * serverPacket = client->popServerEvent();

	while (serverPacket != NULL){
		char * serverEvent = serverPacket->packet_data;
		unsigned int objID = serverPacket->id;

		//printf("%s\n", serverEvent);
		//printf("%d\n", objID);

		if (strcmp(serverEvent, "create") == 0){
			int objectType;
			memcpy(&objectType, serverEvent + 7, sizeof(int));

			printf("CREATING RECEIVED, %d\n",objectType);
			switch (objectType){
			case BUILDING:{//building
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

				Building* b1 = new Building(v1, v2, rot, objID);
				b1->tex = colorbuild;
				b1->norm = normalbuild;
				b1->shade = p_bumpShade;
				gameObjects.insert(std::pair<int, GameObject*>(objID, b1));
				break;
			}
			case HUMAN://human
			case CRUSADER://crusader
			case VAMPIRE://vampire
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
				
				//glScalef(0.01, 0.01, 0.01);
				PlayerType* p;
				if (objectType == HUMAN) {
					p = new PlayerType(objID, 0);
				}
				else if (objectType == CRUSADER) {
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

		if (strcmp(serverEvent, "create_stealth") == 0){
			float x1, y1, z1, x2, y2, z2, a;
			memcpy(&x1, serverEvent + 15, sizeof(float));
			memcpy(&y1, serverEvent + 19, sizeof(float));
			memcpy(&z1, serverEvent + 23, sizeof(float));
			memcpy(&x2, serverEvent + 27, sizeof(float));
			memcpy(&y2, serverEvent + 31, sizeof(float));
			memcpy(&z2, serverEvent + 35, sizeof(float));
			memcpy(&a, serverEvent + 39, sizeof(float));

			//printf("CRUBOX: %f,%f,%f,%f,%f,%f,%f\n", x1, y1, z1, x2, y2, z2, a);

			Vector3* v1 = new Vector3(x1, y1, z1);
			Vector3* v2 = new Vector3(x2, y2, z2);

			CrusaderBox* c = new CrusaderBox(v1, v2, a, objID);
			crusaderBoxes.insert(std::pair<int, CrusaderBox*>(objID, c));
		}

		if (strcmp(serverEvent, "stealth") == 0){
			float x1, y1, z1, x2, y2, z2, a;
			int pid, stealthed;
			memcpy(&x1, serverEvent + 8, sizeof(float));
			memcpy(&y1, serverEvent + 12, sizeof(float));
			memcpy(&z1, serverEvent + 16, sizeof(float));
			memcpy(&x2, serverEvent + 20, sizeof(float));
			memcpy(&y2, serverEvent + 24, sizeof(float));
			memcpy(&z2, serverEvent + 28, sizeof(float));
			memcpy(&a, serverEvent + 32, sizeof(float));
			memcpy(&pid, serverEvent + 36, sizeof(int));
			memcpy(&stealthed, serverEvent + 40, sizeof(int));

			//printf("cbox_update: %f, %f, %f, %f, %f, %f, %f, %d, %d\n", x1,y1,z1,x2,y2,z2,a,pid,stealthed);

			if (crusaderBoxes.find(objID) != crusaderBoxes.end()){
				CrusaderBox* o = crusaderBoxes.at(objID);
				o->getMin()->x = x1;
				o->getMin()->y = y1;
				o->getMin()->z = z1;
				o->getMax()->x = x2;
				o->getMax()->y = y2;
				o->getMax()->z = z2;
				o->setAlpha(a);
			}

			if (stealthed == 1){
				if (Player && pid == Player->getID()){
					Player->setVisible(false);
				}
				else if (gameObjects.find(pid) != gameObjects.end()){
					GameObject* o = gameObjects.at(pid);
					o->setVisible(false);
				}
			}
		}

		if (strcmp(serverEvent, "kill_stealth") == 0){
			float x1, y1, z1, x2, y2, z2, a;
			int pid, stealthed;
			memcpy(&x1, serverEvent + 13, sizeof(float));
			memcpy(&y1, serverEvent + 17, sizeof(float));
			memcpy(&z1, serverEvent + 21, sizeof(float));
			memcpy(&x2, serverEvent + 25, sizeof(float));
			memcpy(&y2, serverEvent + 29, sizeof(float));
			memcpy(&z2, serverEvent + 33, sizeof(float));
			memcpy(&a, serverEvent + 37, sizeof(float));
			memcpy(&pid, serverEvent + 41, sizeof(int));
			memcpy(&stealthed, serverEvent + 45, sizeof(int));

			printf("killin_stealth %d\n",pid);

			if (crusaderBoxes.find(objID) != crusaderBoxes.end()){
				std::map<int, CrusaderBox*>::iterator it2;
				it2 = crusaderBoxes.find(objID);
				crusaderBoxes.erase(it2);
			}
			if (Player && pid == Player->getID()){
				Player->setVisible(true);
			}
			else if (gameObjects.find(pid) != gameObjects.end()){
				GameObject* o = gameObjects.at(pid);
				o->setVisible(true);
			}
		}

		if (strcmp(serverEvent, "projectile") == 0){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 11, sizeof(float));
			memcpy(&yPos, serverEvent + 15, sizeof(float));
			memcpy(&zPos, serverEvent + 19, sizeof(float));

			Projectile* p = new Projectile(objID);
			p->setPos(xPos, yPos+7.3, zPos);

			printf("Projectile ID: %d\n", objID);

			projectiles.insert(std::pair<int, Projectile*>(objID, p));
		}

		if (strcmp(serverEvent, "projectile2") == 0){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 11, sizeof(float));
			memcpy(&yPos, serverEvent + 15, sizeof(float));
			memcpy(&zPos, serverEvent + 19, sizeof(float));

			PowerProjectile* p = new PowerProjectile(objID);
			p->setPos(xPos, yPos + 7.3, zPos);

			printf("Projectile2 ID: %d\n", objID);

			projectiles.insert(std::pair<int, Projectile*>(objID, p));
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

		if (strcmp(serverEvent, "p_pos") == 0){
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 6, sizeof(float));
			memcpy(&yPos, serverEvent + 10, sizeof(float));
			memcpy(&zPos, serverEvent + 14, sizeof(float));

			printf("p_pos: %f, %f, %f\n", xPos, yPos, zPos);

			if (projectiles.find(objID) != projectiles.end()){
				Projectile* o = projectiles.at(objID);
				o->setPos(xPos, yPos+7.3, zPos);
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
			deathTime = currGameTime;
		}

		if (strcmp(serverEvent, "dashRange") == 0)
		{
			float dRange;
			memcpy(&dRange, serverEvent + 10, sizeof(float));
			printf("Getting Range %f\n", dRange);
			if (Player && objID == Player->getID()){
				Player->setDashRange(dRange);
			}
		}

		if (strcmp(serverEvent, "score") == 0){
			int cS, vS;
			memcpy(&cS, serverEvent + 6, sizeof(int));
			memcpy(&vS, serverEvent + 10, sizeof(int));

			crusaderScore = cS;
			vampireScore = vS;
		}

		if (strcmp(serverEvent, "p_die") == 0){
			if (projectiles.find(objID) != projectiles.end()){
				std::map<int, Projectile*>::iterator it2;
				it2 = projectiles.find(objID);
				projectiles.erase(it2);
			}
		}

		if (strcmp(serverEvent, "weapon1") == 0)
		{
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 8, sizeof(float));
			memcpy(&yPos, serverEvent + 12, sizeof(float));
			memcpy(&zPos, serverEvent + 16, sizeof(float));
			weap1 = false;
			pnum1 = objID;
		}

		if (strcmp(serverEvent, "weapon2") == 0)
		{
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 8, sizeof(float));
			memcpy(&yPos, serverEvent + 12, sizeof(float));
			memcpy(&zPos, serverEvent + 16, sizeof(float));
			weap2 = false;
			pnum2 = objID;
		}

		if (strcmp(serverEvent, "weapon3") == 0)
		{
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 8, sizeof(float));
			memcpy(&yPos, serverEvent + 12, sizeof(float));
			memcpy(&zPos, serverEvent + 16, sizeof(float));
			weap3 = false;
			pnum3 = objID;
		}

		if (strcmp(serverEvent, "weapon4") == 0)
		{
			float xPos;
			float yPos;
			float zPos;
			memcpy(&xPos, serverEvent + 8, sizeof(float));
			memcpy(&yPos, serverEvent + 12, sizeof(float));
			memcpy(&zPos, serverEvent + 16, sizeof(float));
			weap4 = false;
			pnum4 = objID;
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
				//first calculate health loss amount
				int hpTemp = Player->getHealth();
				Player->setHealth(hp);
				hpTemp = Player->getHealth() - hpTemp;
				if (hpTemp < -1){
					damagedTime = damagedAnimLength;
					redBorder = true;
				}
				else if (hpTemp>40){
					damagedTime = damagedAnimLength;
					redBorder = false;
				}
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
	UpdateParticles();
	UpdateClient(client);

}

////////////////////////////////////////////////////////////////////////////////

void PlayState::RenderParticle(float rot, ParticleEffect* p, float scale, float xx, float yy, float zz)
{
	glPushMatrix();
	glTranslatef(xx, yy, zz);
	glScalef(scale, scale, scale);
	glRotatef(rot, 0, 1, 0);
	glRotatef(180, 1, 0, 0);
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
	for (unsigned int i = 0; i < strlen(healthString); i++){
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
	for (unsigned int i = 0; i < strlen(hp); i++){
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
	for (unsigned int i = 0; i < strlen(name); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (char)name[i]);
	}
	glPopMatrix();

	//scoreboard
	glPushMatrix();
	std::string cS = std::string("Crusaders: ");
	cS = cS + std::to_string(crusaderScore);
	char * cScore = (char *)cS.c_str();
	glTranslatef(2*width / 50, height / 30, 0);
	glLineWidth(2);
	glScalef(width / 5000.88, height / 3800.0f, 1);
	glRotatef(180, 1, 0, 0);
	glColor3f(1, 1, 1);
	for (unsigned int i = 0; i < strlen(cScore); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (char)cScore[i]);
	}
	glPopMatrix();

	glPushMatrix();
	std::string vS = std::string(" Vampires: ");
	vS = vS + std::to_string(vampireScore);
	char * vScore = (char *)vS.c_str();
	glTranslatef(width / 40, 2.5*height / 30, 0);
	glLineWidth(2);
	glScalef(width / 5000.88, height / 3800.0f, 1);
	glRotatef(180, 1, 0, 0);
	glColor3f(1, 1, 1);
	for (unsigned int i = 0; i < strlen(vScore); i++){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (char)vScore[i]);
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
		for (unsigned int i = 0; i < strlen(name); i++){
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
		for (unsigned int i = 0; i < strlen(name); i++){
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
		for (unsigned int i = 0; i < strlen(name); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)name[i]);
		}
		glPopMatrix();
	}
	//draw restart game text
	if (gameResult == -1 || gameResult == 0 || gameResult == 1){
		glPushMatrix();
		char * restartText = "Press R to Restart";
		glTranslatef(width*.2f, height * .35f, 0);
		glLineWidth(4);
		glScalef(width / 3024.0f, height / 2168.0f, 1);
		glRotatef(180, 1, 0, 0);
		glColor3f(1, 1, 1);
		for (unsigned int i = 0; i < strlen(restartText); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)restartText[i]);
		}
		glPopMatrix();

		glPushMatrix();
		char * escText = "Press ESC to Quit";
		glTranslatef(width*.21f, height * .45f, 0);
		glLineWidth(4);
		glScalef(width / 3024.0f, height / 2168.0f, 1);
		glRotatef(180, 1, 0, 0);
		glColor3f(1, 1, 1);
		for (unsigned int i = 0; i < strlen(escText); i++){
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (char)escText[i]);
		}
		glPopMatrix();
	}

	if (damagedTime > 0){
		//draw edges when getting hit or healed
		glPushMatrix();
		glEnable(GL_BLEND); //Enable blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		else{
			glColor4f(0.f, 1.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		glVertex2f(0, 0);
		glVertex2f(width, 0);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, 0);
		}
		else{
			glColor4f(0.f, 1.f, 0.f, 0);
		}
		glVertex2f(width, height / 8);
		glVertex2f(0, height / 8);

		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, 0);
		}
		else{
			glColor4f(0.f, 1.f, 0.f, 0);
		}
		glVertex2f(0, height - (height / 8));
		glVertex2f(width, height - (height / 8));
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		else{
			glColor4f(0.f, 1.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		glVertex2f(width, height);
		glVertex2f(0, height);
		glEnd();

		glDisable(GL_BLEND);
		glPopMatrix();

		// Making sure we can render 3d again
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);


		//had to do this again for each side rect because alphas were not
		//working well with eachother...
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_CULL_FACE);
		glClear(GL_DEPTH_BUFFER_BIT);

		glPushMatrix();
		glEnable(GL_BLEND); //Enable blending.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		else{
			glColor4f(0.f, 1.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		glVertex2f(0, 0);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, 0);
		}
		else{
			glColor4f(0.f, 1.f, 0.f, 0);
		}
		glVertex2f(width / 10, 0);
		glVertex2f(width / 10, height);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		else{
			glColor4f(0.f, 1.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		glVertex2f(0, height);

		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, 0);
		}
		else{
			glColor4f(0.f, 1.f, 0.f, 0);
		}
		glVertex2f(width - (width / 10), 0);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		else{
			glColor4f(0.f, 1.f, 0.f, ((float)damagedTime / damagedAnimLength));
		}
		glVertex2f(width, 0);
		glVertex2f(width, height);
		if (redBorder){
			glColor4f(1.f, 0.f, 0.f, 0);
		}
		else{
			glColor4f(0.f, 1.f, 0.f, 0);
		}
		glVertex2f(width - (width / 10), height);
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
	}

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

	if (currGameTime < client->getPhase2Time() && currGameTime >= client->getPhase1Time())
		isNight = true;
	else
		isNight = false;
	p_Light->Draw(client, currGameTime);
	p_bumpShade->bind();
	p_Map->Draw(p_bumpShade, m_pTrivialNormalMap, photos);
	p_bumpShade->unbind();
	
	p_regShade->bind();

	
	p_Shrine->Draw();

	//Player->update(true, Cam.GetRotation().y);
	Player->UpdateMoveAnimation(isNight, Player);
	Player->update(true, Cam.GetRotation().y);

	std::map<int, GameObject*>::iterator it;
	for (it = gameObjects.begin(); it != gameObjects.end(); it++)
	{
		((PlayerType*)(it->second))->UpdateMoveAnimation(isNight, Player);
		((PlayerType*)(it->second))->update(false, Cam.GetRotation().y);

		if (!weap1 && ((PlayerType*)(it->second))->getID() == pnum1) {
			((PlayerType*)(it->second))->EquipWeapon((Weapon*)p_SunMace);
		}
		else if (!weap2 && ((PlayerType*)(it->second))->getID() == pnum2) {
			((PlayerType*)(it->second))->EquipWeapon((Weapon*)p_DefenseShield);
		}
		else if (!weap3 && ((PlayerType*)(it->second))->getID() == pnum3) {
			((PlayerType*)(it->second))->EquipWeapon((Weapon*)p_LightningBolt);
		}
		else if (!weap4 && ((PlayerType*)(it->second))->getID() == pnum4) {
			((PlayerType*)(it->second))->EquipWeapon((Weapon*)p_BatSword);
		}
	}

	std::map<int, Projectile*>::iterator it2;
	for (it2 = projectiles.begin(); it2 != projectiles.end(); it2++)
	{
		((PlayerType*)(it2->second))->update(false, Cam.GetRotation().y);
	}

	// David's building(no collision)
	glPushMatrix();
	glTranslatef(250, 50, 250);
	p_DavidsBuilding->Render();
	glPopMatrix();
	if (Player->getTeam() == 1) {
		if (!weap1 && Player->getID() == pnum1) {
			Player->EquipWeapon((Weapon*)p_SunMace);
		}
		else if (!weap2 && Player->getID() == pnum2) {
			Player->EquipWeapon((Weapon*)p_DefenseShield);
		}
		else if (!weap3 && Player->getID() == pnum3) {
			Player->EquipWeapon((Weapon*)p_LightningBolt);
		}
		else if (!weap4 && Player->getID() == pnum4) {
			Player->EquipWeapon((Weapon*)p_BatSword);
		}
	}
	p_SunMace->Draw();
	p_DefenseShield->Draw();
	p_LightningBolt->Draw();
	p_BatSword->Draw();
	
	//draw transparent stuff here
	std::map<int, CrusaderBox*>::iterator it3;
	for (it3 = crusaderBoxes.begin(); it3 != crusaderBoxes.end(); it3++)
	{
		(it3->second)->update(false, Cam.GetRotation().y);
	}

	p_regShade->unbind();

	RenderParticle(-Cam.GetRotation().y, p_ShrineFire, 0.1, 0, 14, 0);
	RenderParticle(-Cam.GetRotation().y, p_Sparks, 1, Player->getPos().x, Player->getPos().y, Player->getPos().z);
	
	drawHUD(client); //This includes the game over results

	glfwSwapBuffers(window);
	glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input(ClientGame* client) {
	if (!Player)
		return;
	
	float speedup;
	if (isNight && Player->getTeam() == 2)
		speedup = 3.f;
	else
		speedup = 2.f;

	if (glfwGetKey(window, FORWARD) || glfwGetKey(window, BACKWARD))
	{
		if (glfwGetKey(window, FORWARD)) {
			/*if (!attacking) {
				Player->setAnimation(a_RUNFORWARD, 0, speedup);
			}*/
			client->addEvent(Player->getID(), "move_forward;", ACTION_EVENT);

			if (glfwGetKey(window, STRAFELEFT)) {
				/*if (!attacking) {
					Player->setAnimation(a_RUNFORWARD, 30, speedup);
				}*/
				client->addEvent(Player->getID(), "move_left;", ACTION_EVENT);
			}
			if (glfwGetKey(window, STRAFERIGHT)) {
				/*if (!attacking) {
					Player->setAnimation(a_RUNFORWARD, -30, speedup);
				}*/
				client->addEvent(Player->getID(), "move_right;", ACTION_EVENT);
			}
		}
		if (glfwGetKey(window, BACKWARD)) {
			/*if (!attacking) {
				Player->setAnimation(a_WALKBACK);
			}*/
			client->addEvent(Player->getID(), "move_backward;", ACTION_EVENT);

			if (glfwGetKey(window, STRAFELEFT)) {
				/*if (!attacking) {
					Player->setAnimation(a_RUNFORWARD, -30, speedup);
				}*/
				client->addEvent(Player->getID(), "move_left;", ACTION_EVENT);
			}
			if (glfwGetKey(window, STRAFERIGHT)) {
				/*if (!attacking) {
					Player->setAnimation(a_RUNFORWARD, 30, speedup);
				}*/
				client->addEvent(Player->getID(), "move_right;", ACTION_EVENT);
			}
		}

	}
	else if (glfwGetKey(window, STRAFELEFT)) {
		/*if (!attacking) {
			Player->setAnimation(a_STRAFELEFT, 0, speedup);
		}*/
		client->addEvent(Player->getID(), "move_left;", ACTION_EVENT);
	}
	else if (glfwGetKey(window, STRAFERIGHT)) {
		/*if (!attacking) {
			Player->setAnimation(a_STRAFERIGHT, 0, speedup);
		}*/
		client->addEvent(Player->getID(), "move_right;", ACTION_EVENT);
	}
	else {
		/*if (!attacking) {
			Player->setAnimation(a_IDLE);
		}*/
	}

	if (glfwGetKey(window, JUMP)) {
		client->addEvent(Player->getID(), "move_jump;", ACTION_EVENT);
	}
	if (glfwGetKey(window, Q)) {
		client->addEvent(Player->getID(), "q;", ACTION_EVENT);
	}
	if (glfwGetKey(window, GLFW_KEY_B)) {
		client->addEvent(Player->getID(), "b;", ACTION_EVENT);
	}
	// attack mode change //////////////////////////////////////////////
	if (glfwGetKey(window, GLFW_KEY_1)){
		client->addEvent(Player->getID(), "sw1;", ACTION_EVENT);
	}
	else if (glfwGetKey(window, GLFW_KEY_2)){
		client->addEvent(Player->getID(), "sw2;", ACTION_EVENT);
	}
	///////////////////////////////////////////////////////////////////
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

	if (restartGame){
		client->setStateChange("pre_state");
		client->addEvent(Player->getID(), "restart;", ACTION_EVENT);
	}

	if (attacking){
		client->addEvent(Player->getID(), "attack;", ACTION_EVENT);
	}

	if (Player->getAttacking2Starts() >= Player->getAttacking2Ends() && Player->getAttacking2Starts()>0 && !startJustSent){
		printf("Attaching Start\n");
		client->addEvent(Player->getID(), "attack2Start;", ACTION_EVENT);
		Player->setAttacking2Starts(Player->getAttacking2Starts()-1);
		Player->setAttacking2(true);
		startJustSent = true;
	}
	else if (Player->getAttacking2Starts() < Player->getAttacking2Ends() && startJustSent){
		printf("Attaching End\n");
		client->addEvent(Player->getID(), "attack2End;", ACTION_EVENT);
		Player->setAttacking2Ends(Player->getAttacking2Ends() - 1);
		Player->setAttacking2(false);
		startJustSent = false;
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (gameResult == -1 || gameResult == 0 || gameResult == 1){
		if (key == GLFW_KEY_R && action == GLFW_PRESS){
			restartGame = true;
		}
	}
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
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
		Player->setAnimation(a_COMBOATTACK);
		Player->setAttacking2Starts(Player->getAttacking2Starts() + 1);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		Player->setAttacking2Ends(Player->getAttacking2Ends() + 1);
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseMotion(GLFWwindow* window, double xpos, double ypos) {
	int dx = xpos - MouseX;
	int dy = -(ypos - MouseY);

	const float rate = 1.0f;

	MouseX = xpos;
	MouseY = ypos;
	int sign = 1;
	if (dx < 0){
		sign = -1;
	}

	float newDx = abs(dx)/4.0f;
	if (newDx < .1){
		newDx = .1;
	}
	else if (newDx > 10){
		newDx = 10;
	}

	// Move camera
	Cam.AddPitch(-dy/2.0f);
	Cam.AddYaw(sign*newDx);
	rotationChanged = true;
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	/*const float rate = 3.0f;
	if (yoffset > 0) {
		Cam.TranslateZ(1.0f*rate);
	}
	else if (yoffset < 0) {
		Cam.TranslateZ(-1.0f*rate);
	}*/
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
