#include "PlayState.h"

static PlayState *state;

ParticleEffect g_ParticleEffect(1000);
Vector3 g_DefaultCameraTranslate(0, 0, -100);
Vector3 g_DefaultCameraRotate(40, 0, 0);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) { state->KeyCallback(window, key, scancode, action, mods); }
static void mouse_motion(GLFWwindow* window, double xpos, double ypos) { state->MouseMotion(window, xpos, ypos); }
static void mouse_button(GLFWwindow* window, int button, int action, int mods) { state->MouseButton(window, button, action, mods); }
static void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset) { state->MouseScroll(window, xoffset, yoffset); }

PlayState::PlayState(GLFWwindow* window) {
	this->window = window;
	Initialize();

	//in order for it to read two different players.  The players must be initialized with 
	//different numbers
	//player = Player(1);
}

int PlayState::InitGL() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSwapInterval(0);	// no vsync

	glfwGetFramebufferSize(window, &WinX, &WinY);
	ratio = WinX / (float)WinY;
}

int PlayState::Initialize() {
	InitGL();

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

	/*b1 = new Building(25, -20, 10, -40, -18);
	b2 = new Building(15, 20, 18, 20, 18);
	field.buildingList.push_back(b1);
	field.buildingList.push_back(b2);*/

	// Camera
	Cam.SetViewport(0, 0, WinX, WinY);
	Cam.ApplyViewport();

	Cam.SetProjection(60.0f, ratio, 0.1f, 1000.0f);
	Cam.ApplyProjectionTransform();

	Cam.SetTranslate(g_DefaultCameraTranslate);
	Cam.SetRotate(g_DefaultCameraRotate);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);

	// OSG - SunShrine
	/*viewer = new osgViewer::Viewer;

	//osg::Camera* os_camera = viewer->getCamera();
	//os_camera->setViewport(0, 0, WinX, WinY);
	//os_camera->setProjectionMatrixAsPerspective(60.0f, ratio, 0.1f, 1000.0f);
	//os_camera->setViewMatrix(osg::Matrix::lookAt(osg::Vec3(g_DefaultCameraTranslate.x, g_DefaultCameraTranslate.y, g_DefaultCameraTranslate.z),
															//osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0)));
	//os_camera->setClearColor(osg::Vec4(0.5, 0.0, 0.0, 0.0));

	root = osgDB::readNodeFile("models/RobotAnimation.fbx");

	// Animations
	root->accept(finder);
	const osgAnimation::AnimationList& animations = finder._am->getAnimationList();
	finder._am->playAnimation(animations[0].get());

	os_window = viewer->setUpViewerAsEmbeddedInWindow(0, 0, WinX, WinY);
	viewer->setSceneData(root.get());
	viewer->realize();*/

	// Particles
	/*if (g_ParticleEffect.LoadTexture("./particles/Textures/star.png"))
	{
		std::cout << "Successfully loaded particle texture." << std::endl;
	}
	else
	{
		std::cerr << "Failed to load particle texture!" << std::endl;
	}*/

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

	return 0;
}

void PlayState::Update() {
}

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

double lastTime = glfwGetTime();

void PlayState::Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up glStuff

	// Draw components
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//osg::Matrixd i = Cam.GetApplyViewTransform();
	//viewer->getCamera()->setViewMatrix(i);

	//viewer->frame();

	glPushMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Cam.ApplyViewTransform();

	drawAxis(100);
	player.update();

	glPopMatrix();
	// Begin drawing player and scene
	glPopAttrib();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void PlayState::Pause() {

}

void PlayState::Resume() {

}

////////////////////////////////////////////////////////////////////////////////

void PlayState::Input() {
	/*float playerRotation = -player.getRotation();

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
	}*/
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // exit
		glfwSetWindowShouldClose(window, GL_TRUE);
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseButton(GLFWwindow* window, int button, int action, int mods) {
	//float playerRotation = -player.getRotation();

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
			//Cam.SetAzimuth(playerRotation);
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

	const float rate = 1.0f;
	// Move camera
	// NOTE: this should really be part of Camera::Update()
	/*if (LeftDown) {
		Cam.SetAzimuth(Cam.GetAzimuth() + dx*rate);
		Cam.SetIncline(Cam.GetIncline() - dy*rate);
	}
	if (RightDown) {
		Cam.SetAzimuth(Cam.GetAzimuth() + dx*rate);
		Cam.SetIncline(Cam.GetIncline() - dy*rate);
		//player.setRotation(-Cam.GetAzimuth());
	}*/
	if (LeftDown || RightDown) {
		Cam.AddPitch(-dy*rate);
		Cam.AddYaw(dx*rate);
	}
}

////////////////////////////////////////////////////////////////////////////////

void PlayState::MouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
	const float rate = 10.0f;
	if (yoffset > 0) {
		Cam.TranslateZ(1.0f*rate);
	}
	else if (yoffset < 0) {
		Cam.TranslateZ(-1.0f*rate);
	}
}