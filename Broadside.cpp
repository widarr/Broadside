
#include "libcgutils/Obj3D/Obj3D.h"

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
#include "Broadside.hpp"

#ifndef __cplusplus
#define __cplusplus
#endif

GLFWwindow* window;
int gwidth = 640;
int gheight = 480;

// Initial position vectors
glm::vec3 position = glm::vec3( 0.0, 0.0, 40.0 );
glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
// Initial horizontal angle : toward -Z
float horizontalAngle = Helpers::deg2rad(356.5f);
// Initial vertical angle : none
float verticalAngle = Helpers::deg2rad(222.5f);
// Initial z-Axis angle
float depthAngle = 0.0f;
// Initial Field of View
float FoV = 45.0f;
bool ortho = false;

float speed = 0.05f; // 3 units / second
float mouseSpeed = 0.005f;
bool pointerLocked = false;
double xpos = 0.0f;
double ypos = 0.0f;
float deltaTime = 0.0f;

Obj3D* testmodel1 = NULL;
Obj3D* testmodel2 = NULL;
Obj3D* testmodel3 = NULL;
Obj3D* testmodel4 = NULL;
Obj3D* testmodel5 = NULL;
Obj3D* testmodel6 = NULL;
Obj3D* testmodel7 = NULL;
Obj3D* testmodel8 = NULL;
Obj3D* sphere = NULL;
Obj3D* suzanne = NULL;

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
	
	testmodel1 = new Obj3D();
	testmodel1->initialise("cube");
	testmodel1->spawnAt(-10,10,-10);
	
	testmodel2 = new Obj3D();
	testmodel2->initialise("cube");
	testmodel2->spawnAt(10,10,-10);
	
	testmodel3 = new Obj3D();
	testmodel3->initialise("cube");
	testmodel3->spawnAt(10,10,10);
	
	testmodel4 = new Obj3D();
	testmodel4->initialise("cube");
	testmodel4->spawnAt(-10,10,10);
	
	testmodel5 = new Obj3D();
	testmodel5->initialise("cube");
	testmodel5->spawnAt(-10,-10,-10);
	
	testmodel6 = new Obj3D();
	testmodel6->initialise("cube");
	testmodel6->spawnAt(10,-10,-10);
	
	testmodel7 = new Obj3D();
	testmodel7->initialise("cube");
	testmodel7->spawnAt(10,-10,10);
	
	testmodel8 = new Obj3D();
	testmodel8->initialise("cube");
	testmodel8->spawnAt(-10,-10,10);
	
	sphere = new Obj3D();
	sphere->initialise("sphere");
	sphere->spawnAt(0,0,0);
	
	suzanne = new Obj3D();
	suzanne->initialise("suzanne");
	suzanne->spawnAt(0,10,0);
	
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
	
	testmodel1->draw();
	testmodel2->draw();
	testmodel3->draw();
	testmodel4->draw();
	testmodel5->draw();
	testmodel6->draw();
	testmodel7->draw();
	testmodel8->draw();
	sphere->draw();
	suzanne->draw();
	
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void runPipeline(){
	
	Mortimer();
	move();
	
	Matrices m;
		
	if(ortho) {
		m.P = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f);
	} else {
		m.P = glm::perspective(FoV, 4.0f / 3.0f, 0.01f, 300.0f);
	}
	m.V = glm::lookAt(position,position+direction, up);
	m.M = Pipeline.getIdentityMatrix();
	m.MVP = m.P * m.V * m.M;
	
	Pipeline.setPipeline(m);
}

void Mortimer() {
	  
	static double lastTick = glfwGetTime();
	double currentTick = glfwGetTime();
	double tick = float(currentTick - lastTick);
	deltaTime = tick;
	
	if(tick > 33) {
		// put animation timer variable here
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
	
	/* We don't need this LSD effect for now ;)
	if (glfwGetKey( window, GLFW_KEY_E ) == GLFW_PRESS){
		FoV += 5.0f;
		if(FoV >= 360.0f)
		  FoV = 45.0f;
	}
	if (glfwGetKey( window, GLFW_KEY_Q ) == GLFW_PRESS){
		FoV -= 5.0f;
		if(FoV <= 0.0f)
		  FoV = 45;
	}
	*/
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
