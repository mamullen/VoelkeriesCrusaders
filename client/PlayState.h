////////////////////////////////////////
// PlayState.h
////////////////////////////////////////

#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include <stdlib.h>
#include "core.h"
#include "Weapon.h"
#include "Shrine.h"
#include "Map.h"
#include "Light.h"
#include "PlayerType.h"
#include "CrusaderPlayer.h"
#include "VampirePlayer.h"
#include "ElapsedTime.h"
#include "ParticleSystem.h"
#include "Emitter.h"
#include "GameState.h"
#include "Building.h"
#include "CrusaderBox.h"
#include "Shockwave.h"
#include "Projectile.h"
#include "Floor.h"
#include "ClientGame.h"
#include <map>
#include "Shader.h"
#include "Texture.h"
#if ( (defined(__MACH__)) && (defined(__APPLE__)) )
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <OpenGL/glext.h>
#else
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <chrono>

//#include <fbxsdk/fbxsdk_def.h>

class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window);
	
	int Initialize();
	int InitGL();

	void Input(ClientGame* Client);
	void Update(ClientGame* Client);

	// Draw Methods
	void Draw(ClientGame* client);
	void RenderParticles(float rot);
	void RenderShrineParticles(float rot);
	void UpdateParticles();
	void UpdateClient(ClientGame* Client);

	void RenderParticle(float rot, ParticleEffect *p, float xx, float yy, float zz);

	void drawHUD(ClientGame* Client);

	GLuint LoadRAWTexture(const char * filename, int width, int height);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CharCallback(GLFWwindow* window, unsigned int code);
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
private:
	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	// Input
	bool LeftDown, MiddleDown, RightDown, LeftDownTwo, BothDown;
	bool rotationChanged = false, attacking = false;
	bool attacking2Sent = false;
	double MouseX, MouseY;
	double lastTime;
	//time in milliseconds
	unsigned int savedTime;//curr time of client
	int damagedTime = 0, damagedAnimLength = 1000;
	bool restartGame = false, redBorder = false, startJustSent = false;

	// Components
	Camera Cam;
	PlayerType* Player;

	SphereEmitter g_ParticleEmitter;
	CubeEmitter g_CubeEmitter;

	int pid = -1;
	//player is not stored in this map
	std::map<int, GameObject*> gameObjects;
	std::map<int, Projectile*> projectiles;
	std::map<int, CrusaderBox*> crusaderBoxes;
	std::list<Shockwave*> shockwaves;
	int currGameTime = 0, gameResult = -2;
	int vampireScore = 0, crusaderScore = 0;
	float cooldown1 = 0, cooldown2 = 0;

	//Building* b1;
	Texture t;
	GLuint photos[5];
	//Shader floorshader;
	
	Texture colorbuild;
	Texture normalbuild;
	GLuint text_picture, text_normalmap;
	GLuint floor_picture, floor_normalmap;
	GLuint skyfront_picture, skyfront_normalmap, skyback_picture, skyback_normalmap, skyleft_picture, skyleft_normalmap, skyright_picture, skyright_normalmap, skytop_picture, skytop_normalmap;
	bool once;
	bool parti;
	bool weap1, weap2, weap3, weap4;
	unsigned int pnum1, pnum2, pnum3, pnum4;
	Vector3 particlepos;
	bool deathbyparticle;
	float deathTime;

	////// - David's meshes
	// Items
	SunMace *p_SunMace;
	BatSword* p_BatSword;
	DefenseShield *p_DefenseShield;
	LightningBoltAtkSpd *p_LightningBolt;
	Shrine* p_Shrine;
	MeshLoader* p_DavidsBuilding;

	Texture m_pTrivialNormalMap;
	ParticleEffect* p_ShrineFire;
	Light* p_Light;
	Map* p_Map;
	bool isNight;
	bool debug = false;
	std::list<int> ints;

	Shader* p_regShade;
	Shader* p_bumpShade;

	GLuint attackpic[2];
	GLuint avatar[2];
	GLuint hud[3];
	void drawCircleOutline(float r, Vector3 pos, int i);

	GLuint timer[11];//load numbers here

	int oType;
	GLuint sunID;
	GLuint moonID;
	int timercoeef;
	float timerX;
};

////////////////////////////////////////////////////////////////////////////////

#endif