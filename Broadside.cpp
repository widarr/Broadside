/*
 * Hier entsteht das Spieleprojekt für das Fach Computer Graphics
 * Der Main Loop des Programmes befindet sich hier.
 * Falls es effizient ist kann noch überlegt werden ob die Steuerung
 * in eine eigene Klasse ausgelagert wird.
 * Alle anderen Funktionen werden von der (um Obj3D) erweiterten libcgutils
 * übernommen.
 * Im ordner test befindet sich experimenteller code zum Erlernen von OpenGl
 * Dieser Text sollte später ersetzt/umgeschrieben werden und der test ordner
 * kann dann auch entfernt werden
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <oogl/GLSLShader.h>
#include <oogl/GLSLProgram.h>
#include "cube.h"
#include "hexagon.h"
#include "BufferedCube.h"
#include "Obj3D/Renderable.h"

#ifndef __cplusplus
#define __cplusplus
#endif

GLFWwindow* window;
int gwidth = 640;
int gheight = 480;
#define GLCOLOR1 0.91f, 0.128f, 0.64f, 1.0f
#define GLCOLOR2 0.0f, 1.0f, 0.0f, 0.5f
#define GLCOLOR3 0.0f, 0.0f, 1.0f, 0.5f

#define RGB2F(r,g,b) 1.0/255*r,1.0/255*g,1.0/255*b
#define F2RGB(r,g,b) 255*r,255*g,255*b

// Initial position vectors
glm::vec3 position = glm::vec3( 0.0, 5.0, 10.0 );
glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial z-Axis angle
float depthAngle = 0.0f;
// Initial Field of View
float FoV = 45.0f;
bool ortho = false;

// Forward declaration of our MVP matrix, GLSL variables and buffers
glm::mat4 MVP = glm::mat4();
GLuint vertexbuffer = 0;
GLuint colorbuffer = 0;
GLuint programID = 0;
GLuint MatrixID = 0;
GLuint vertexPosition_modelspaceID = 0;
GLuint vertexColorID = 0;

float speed = 0.05f; // 3 units / second
float mouseSpeed = 0.005f;
bool pointerLocked = false;
double xpos = 0.0f;
double ypos = 0.0f;
float deltaTime = 0.0f;

static GLfloat yRot = 0.0f;
static GLfloat xRot = 0.0f;
static GLfloat zRot = 0.0f;

void initialise();
void Rendering();
void runPipeline();
void Mortimer();
void mouse();
void keyboard();
void move();
void drawBufferedCube();
int LinMain(int, char**);

int main(int argc, char** argv) {
	
	printf("Starte Programm !\n");
	
	LinMain(argc, argv);
	
	return EXIT_SUCCESS;
}

int LinMain(int argc, char** argv) {
	  
	initialise();
	
	//create program
	programID = glCreateProgram();

	//compile shaders
	GLuint vertexshader = oogl::loadShader("shader/TransformVertexShader.vertexshader",GL_VERTEX_SHADER);
	GLuint fragmentshader = oogl::loadShader("shader/ColorFragmentShader.fragmentshader",GL_FRAGMENT_SHADER);

	//attach shaders
	glAttachShader(programID, vertexshader);
	glAttachShader(programID, fragmentshader);

	//link program
	GLint status;
	glLinkProgram(programID);
	
	MatrixID = glGetUniformLocation(programID, "MVP");
	vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	vertexColorID = glGetAttribLocation(programID, "vertexColor");
	
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	
	do {
		  Rendering();
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0 );
	
	glfwTerminate();

	return 0;
}

void Rendering() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glPolygonMode(GL_BACK,GL_FILL);
	glLoadIdentity();
		
	runPipeline();
	
	glTranslatef(0.0f, 1.0f, 0.0f);
	drawBufferedCube();
	
	glLoadIdentity();
	glPushMatrix();
		// Ground
		glBegin(GL_QUADS);
			glColor4f(0.5f,0.5f,0.5f,1.0f);
			glVertex3f(-50,0,50);
			glVertex3f(50,0,50);
			glVertex3f(50,0,-50);
			glVertex3f(-50,0,-50);
		glEnd();
	glPopMatrix();

	/*
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*15.0,3,j * 15.0);
			glScalef(3.0f, 3.0f, 3.0f);
			glRotatef(yRot,0.0,1.0,0.0); // Rotate on y
			drawHexagon();
			glPopMatrix();
		}
	
	 */
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void runPipeline(){
	
	Mortimer();
	move();
	
	glm::mat4 ProjectionMatrix;
	
	if(ortho) {
		ProjectionMatrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
	} else {
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.01f, 300.0f);
	}
	glm::mat4 ViewMatrix = glm::lookAt(position,position+direction, up);
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}

void Mortimer() {
	  
	static double lastTick = glfwGetTime();
	double currentTick = glfwGetTime();
	double tick = float(currentTick - lastTick);
	deltaTime = tick;
	
	if(tick > 33) {
		xRot += 2;
		yRot += 4;
		zRot += 6;
		
		if(yRot == 359)
			yRot = 0;
		if(xRot == 359)
			xRot = 0;
		if(zRot == 359)
			zRot = 0;
		tick = 0.0f;
	}
}

void mouse() {
}

void keyboard() {
	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	// Jump
	if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		position += up * deltaTime * speed;
	}
	// Duck
	if (glfwGetKey( window, GLFW_KEY_C ) == GLFW_PRESS){
		position -= up * deltaTime * speed;
	}
	
}

void move() {
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);
	glfwGetCursorPos(window, &xpos, &ypos);
	
	glfwSetCursorPos(window, gwidth/2, gheight/2);
	
	// Compute new orientation
	horizontalAngle += mouseSpeed * float( gwidth/2 - xpos );
	verticalAngle   += mouseSpeed * float( gheight/2 - ypos );
	  
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		tan(depthAngle),
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector
	up = glm::cross( right, direction );
	  
	keyboard();
	mouse();
}

void initialise() {
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	  
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( gwidth, gheight, "Broadside", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(-1);
	}
	
	printf("OpenGL Version %s initialised\n", glGetString(GL_VERSION));
	GLfloat m, p;
	glGetFloatv(GL_MAX_MODELVIEW_STACK_DEPTH, &m);
	glGetFloatv(GL_MAX_PROJECTION_STACK_DEPTH, &p);
	printf("The maximal depth of the Modelview Matrix stack is: %2f\n", m);
	printf("The maximal depth of the Projection Matrix stack is: %2f\n", p);
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, gwidth/2, gheight/2);
	
	// Standard background color
	glClearColor(0.0f, 0.156f, 0.392f, 0.f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}

void drawBufferedCube()
{
	// Use our shader
	glUseProgram(programID);
	
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID, // The attribute we want to configure
		3,                           // size
		GL_FLOAT,                    // type
		GL_FALSE,                    // normalized?
		0,                           // stride
		(void*)0                     // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(vertexColorID);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		vertexColorID,               // The attribute we want to configure
		3,                           // size
		GL_FLOAT,                    // type
		GL_FALSE,                    // normalized?
		0,                           // stride
		(void*)0                     // array buffer offset
	);
	
	// Draw the triangleS !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(vertexPosition_modelspaceID);
	glDisableVertexAttribArray(vertexColorID);
}
