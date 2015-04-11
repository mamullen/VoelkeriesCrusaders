////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Project 1 Skeleton"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
    
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
    n = true;
    s = false;
    temp = true;
    posjoint=0;
    skelskin = 0;
    tt=0;
    dofnum =0;
    rotate = true;
    newx=0.1;
    newy=0.1;
    newz=0.1;
    xtimes = 0;
    ytimes = 0;
    ztimes = 0;
    gridon=false;
    floor = true;
    wind.Zero();
    
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );



	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
    /*
    std::string input = "";
   if(n)
    {
        s = false;
    
    bones.clearVals();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glFinish();
        glutSwapBuffers();
    
    std::cout << "Please Enter name of skin file and skel file  ";
    getline(std::cin, input);
    std::cout << "You entered: " << input << std::endl << std::endl;
    if(input == "tube.skel tube.skin")
    {
        bones.Load("/Users/michaelmullenjr/Desktop/tube.skel");
        skins.Load("/Users/michaelmullenjr/Desktop/tube.skin");
        cull =false;
    }
    else
    {
        bones.Load("/Users/michaelmullenjr/Desktop/wasp.skel");
        skins.Load("/Users/michaelmullenjr/Desktop/wasp.skin");
        tion.Load("/Users/michaelmullenjr/Desktop/wasp_walk.anim");
        cull=true;
    }
        n = false;
    }*/
    

    /*if(light != 0)
    {
    glEnable(GL_LIGHTING);
    }
    
 
        
    glEnable(GL_DEPTH_TEST);
       if(cull)
       {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
       }
       else{
           glDisable(GL_CULL_FACE);
       }*/
   
    
   // bones.Update();
   // worldmat = bones.getListt();
   // skins.Update(bones);
   // tt = tt+0.005;
  //  printf("%f\n",tt);
    flag.Update();
  //  ground.createTile();
    
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
  
    //reset function?
	Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
    std::string inputtwo = "";
    std::string inputthree = "";
    std::string inputnum, inputnumtwo, inputnumthree;
     if(s)
     {
         s = false;
         std::cout << "Please enter which balljoint to edit:\n";
         std::cout<< " root\n left\n right \n lookout\n post\n post_part\n sail\n";
         getline(std::cin, inputtwo);
         std::cout << "You entered: " << inputtwo << std::endl << std::endl;
         
         std::cout << "Please enter offset\n";
         getline(std::cin, inputnum);
         getline(std::cin, inputnumthree);
         getline(std::cin, inputnumtwo);
         int in = ::atof(inputnum.c_str());
         int intwo = ::atof(inputnumtwo.c_str());
         int inthree = ::atof(inputthree.c_str());
         
         
         
         
         
         
     }
    if(skelskin ==1)
    {
     //  bones.Draw();
    }
    else if(skelskin==2)
    {
      //  skins.Draw();
    }
    else{
      /*  bones.Draw();
        glEnable(GL_LIGHTING);
        GLfloat light_ambient[] = { 2.0, 0.0, 0.3, 1.0 };
        GLfloat light_diffuse[] = { 1.0, 0.0, 0.3, 1.0 };
        GLfloat light_specular[] = { 1.5, 1.5, 1.5, 1.0 };
        GLfloat light_position[] = { -2.0, 7.0, 2.0, 0.0 };
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glEnable(GL_LIGHT0);
        //glEnable(GL_CULL_FACE);
        glColor3f(1,1,1);//, <#GLfloat green#>, <#GLfloat blue#>)
       // glDisable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        /*glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        
        float mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        float shininess = 0;
        
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);*/

       // glColor3f(1,0,0);
  //      skins.Draw();*/
    }
    flag.Draw(gridon);
    //skins.Draw();
    //bones.getchildren();
    //tion.Eval(tt,bones);
    //   ground.createTile();
   
   // printf("%f\n",tt);

	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
        case 'n':
            n = true;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            break;
        case 's':
            s = true;
            break;
        //skel skin 1 for z
       
       /* case 'y':
         //   newy++;
            if(posjoint==0)
            {
                bones.root->roty.setValue(bones.root->roty.getValue()+newy);
            }
            else
            {
                bones.joffinal->at(posjoint-1)->roty.setValue(bones.joffinal->at(posjoint-1)->roty.getValue()+newy);
            }
            
            //skelskin = 2;
            break;
        case 'Y':
            //newy--;
            if(posjoint==0)
            {
                bones.root->roty.setValue(bones.root->roty.getValue()-newy);
            }
            else
            {
                bones.joffinal->at(posjoint-1)->roty.setValue(bones.joffinal->at(posjoint-1)->roty.getValue()-newy);
            }
            //skelskin = 2;
            break;*/
        case 'y':
            flag.movement(0,-0.1,0);
            break;
        case 'X':
            flag.movement(0.1,0,0);
            break;
        case 'x':
            flag.movement(-0.1,0,0);
            break;
        case 'Z':
            flag.movement(0,0,0.1);
            break;
        case 'z':
            flag.movement(0,0,-0.1);
            break;
        case 'Y':
            flag.movement(0,0.1,0);
            break;
       
            //skelskin = 2;
            break;
        
      
        case 'l':
            light++;
            if(light>3)
            {
                light = 0;
            }
            break;
        case 'f':
            if(floor)
            {
                floor = false;
            }
            else
            {
                floor = true;
            }
            break;
        case 'h':
            dofnum++;
            break;
       /* case 'g':
            bones.changeDOF(dofnum, .1);
            //bones.setPosBool(-1, true);
            break;
        case 'G':
            bones.changeDOF(dofnum, -.1);
           // bones.setPosBool(-1, true);*/
          //  break;
       /* case 'b':
            bones.changeDOFY(dofnum, .1);
            break;
        case 'B':
            bones.changeDOFY(dofnum, -.1);
            break;
        case 't':
            bones.changeDOFZ(dofnum, .1);
            break;
        case 'T':
            bones.changeDOFZ(dofnum, -.1);
            //tion.Eval(tt,bones);
            //tt = tt+0.1;
            break;*/
        case 'g':
            gridon = true;
            break;
        case 'G':
            gridon = false;
            break;
        case 'D':
           flag.grab();
            break;
        
            
        /*case 'j':
            if(posjoint==bones.joffinal->size())
            {
                printf("Upper Limit Reached\n");
            }
            else
            {
                posjoint++;
            }
            break;
        case 'J':
            if(posjoint==0)
            {
                printf("Lower Limit Reached\n");
            }
            else
            {
                posjoint--;
            }
            break;*/
        case 'k':
            if(rotate)
            {
                rotate = false;
            }
            else
            {
                rotate = true;
            }
            break;
        case '1':
            flag.wind.z+=0.5;
            break;
        case '2':
            flag.wind.z-=0.5;
            break;
        case '3':
            flag.wind.x+=0.5;
            break;
        case '4':
            flag.wind.x-=0.5;
            break;
       
        case 'd':
            flag.drop();
            break;
        case 'q':
            flag.shadows=true;
            printf("on");
            break;
        case 'Q':
            flag.shadows=false;
            printf("off");
            break;
        case 'v':
            Cam.cy+=0.03;
            break;
        case 'V':
            Cam.cy-=0.03;
            break;
        
        case 'c':
            Cam.cx+=0.03;
            break;
        case 'C':
            Cam.cx-=0.03;
            break;
        
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(rotate)
    {
        if(btn==GLUT_LEFT_BUTTON) {
            LeftDown = (state==GLUT_DOWN);
        }
        else if(btn==GLUT_MIDDLE_BUTTON) {
            MiddleDown = (state==GLUT_DOWN);
        }
        else if(btn==GLUT_RIGHT_BUTTON) {
            RightDown = (state==GLUT_DOWN);
        }
    }
    else
    {
        if(btn==GLUT_LEFT_BUTTON) {
            LeftDownTwo = (state==GLUT_DOWN);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
    if(LeftDownTwo)
    {
        const float rate = 0.01f;
        if(posjoint==0)
        {
            bones.root->rotx.setValue(dx*rate);
            bones.root->roty.setValue(dy*rate);
        }
        else
        {
            bones.joffinal->at(posjoint-1)->rotx.setValue(dx*rate);
            bones.joffinal->at(posjoint-1)->roty.setValue(dy*rate);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
