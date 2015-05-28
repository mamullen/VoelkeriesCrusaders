#ifndef PLAYSTATE_H_
#define PLAYSTATE_H_

#include "core.h"
#include "ElapsedTime.h"
#include "ParticleSystem.h"
#include "Emitter.h"
#include "GameState.h"
#include "Building.h"
#include "Floor.h"

/*#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgAnimation/BasicAnimationManager>
#include <osgGA/TrackBallManipulator>
#include <osgViewer/Viewer>*/
/*
struct AnimationManagerFinder : public osg::NodeVisitor
{
	osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;

	AnimationManagerFinder() { setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN); }

	void apply(osg::Node& node) {

		if (_am.valid())
			return;

		if (node.getUpdateCallback()) {
			_am = dynamic_cast<osgAnimation::BasicAnimationManager*>(node.getUpdateCallback());
			return;
		}

		traverse(node);
	}
};{*/

class PlayState : public GameState
{
public:
	PlayState(GLFWwindow* window);
	int Initialize();
	int InitGL();

	void Pause();
	void Resume();

	void Input();
	void Update();
	void Draw();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseButton(GLFWwindow* window, int button, int action, int mods);
	void MouseMotion(GLFWwindow* window, double xpos, double ypos);
	void MouseScroll(GLFWwindow* window, double xoffset, double yoffset);
private:
	GLFWwindow* window;
	// Window management
	float ratio;
	int WindowHandle;
	int WinX, WinY;

	// Input
	bool LeftDown, MiddleDown, RightDown, LeftDownTwo, BothDown;
	double MouseX, MouseY;
	//Building* b1, *b2;
	//Floor field;
	// Components
	Camera Cam;
	Player player;

	SphereEmitter g_ParticleEmitter;
	CubeEmitter g_CubeEmitter;

/*	osg::ref_ptr<osg::Node> root;
	osg::ref_ptr<osgViewer::Viewer> viewer;
	osg::observer_ptr<osgViewer::GraphicsWindowEmbedded> os_window;
	AnimationManagerFinder finder;*/
};

#endif