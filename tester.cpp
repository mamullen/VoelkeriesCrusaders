////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

////////////////////////////////////////////////////////////////////////////////

static Tester *GAME;
static PlayState* State;

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glfw call member functions instead of static functions
static void error_callback(int error, const char* description)									{ fputs(description, stderr); }

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	GAME = new Tester(argc, argv);	//initialize

	double currentTime = glfwGetTime();

	while (!glfwWindowShouldClose(GAME->window)) {
		double newTime = glfwGetTime();
		double frameTime = newTime - currentTime;

		currentTime = newTime;
		std::cout << currentTime << std::endl;

		//currently goes as fast as possible
		GAME->Input();
		GAME->Update();
		GAME->Draw();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	window = glfwCreateWindow(1024, 768, WINDOWTITLE, NULL, NULL); // pass in glfwGetPrimaryMonitor() to first null for fullscreen

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	State = new PlayState(window);
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	//glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Input() {
	State->Input();
}

////////////////////////////////////////////////////////////////////////////////

// Update Game state
// Here we would handle AI and Physics
void Tester::Update() {
	// Update the components in the world
	State->Update();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	State->Draw();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	m_running = false;
	glFinish();
	//glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	//GLuint image = loadBMP_custom("./my_texture.bmp");
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////
GLuint loadBMP_custom(const char * imagepath)
{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;
	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file)                            
	{ 
		printf("Image could not be opened\n");
		return 0; 
	}
	if (fread(header, 1, 54, file) != 54){ // If not 54 bytes read : problem
		printf("Not a correct BMP file\n");
		return false;
	}
	if (header[0] != 'B' || header[1] != 'M'){
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way


	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


}