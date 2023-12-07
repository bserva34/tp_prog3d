// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
// STD includes
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <float.h>
#include <queue>
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
// Local includes
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "Context.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SceneLoader.h"
#include "Texture.h"
#include "stb_image.h"

using namespace std;


// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;

void initSkybox()
{
	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &Context::skyboxVAO);
	glGenBuffers(1, &Context::skyboxVBO);
	glBindVertexArray(Context::skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, Context::skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);
}
unsigned int loadCubemap(std::vector<std::string> faces){
	unsigned int textureID;
	glGenTextures (1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);
	int width , height , nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
	unsigned char *data = stbi_load(faces[i].c_str(), &width , &height , &
	nrChannels , 0);
	if (data) {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	0, GL_RGB , width , height , 0, GL_RGB , GL_UNSIGNED_BYTE ,
	data);
	} else {
	std::cout << "Cubemap tex failed: " << faces[i] << std::endl;
	}
	stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	return textureID;
}

void init() {
	// Context::camera.initPos();
	Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	glCullFace (GL_BACK);
	glEnable (GL_CULL_FACE);
	glDepthFunc (GL_LESS);
	glEnable (GL_DEPTH_TEST);
	glClearColor (0.2f, 0.2f, 0.3f, 1.0f);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}

	
	Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	Context::skyboxProgram = load_shaders("../shaders/unlit/vertex_skybox.glsl", "../shaders/unlit/fragment_skybox.glsl");
	std::vector<std::string> faces
	{
		"../data/skybox/right.jpg",
		"../data/skybox/left.jpg",
		"../data/skybox/top.jpg",
		"../data/skybox/bottom.jpg",
		"../data/skybox/front.jpg",
		"../data/skybox/back.jpg"
	};
	Context::skyboxTexture = loadCubemap(faces);
	initSkybox();
}

void beforeLoop() {
	// Take first instance center as target
	if (!Context::instances.empty()) {
		glm::vec3 centerOfInstance0 = Context::instances.at(0).mesh->center;
		std::cerr << glm::to_string(centerOfInstance0) << std::endl;

		glm::vec3 target(Context::instances.at(0).matrix * glm::vec4(centerOfInstance0, 1.0));
		Context::camera.position = glm::vec3(0.5,0.5,0.5);
		Context::camera.target = target;//lookat
		Context::camera.forward = glm::normalize(target - Context::camera.position);
		Context::camera.view = Context::camera.getViewMatrix();
	}

	// std::cerr << "projection:" << glm::to_string(Context::camera::projection) << std::endl;
	// std::cerr << "view:" << glm::to_string(Context::camera::view) << std::endl;

}


void draw() {
	if (Context::refreshMatrices) {
		// Context::camera.refreshMatrices();
		Context::camera.view = Context::camera.getViewMatrix();
		Context::camera.projection = Context::camera.getProjectionMatrix();
		Context::refreshMatrices = false;
	}
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	//cout<<Context::instances.size()<<endl;

	
	glDepthMask(GL_FALSE);

	glUseProgram(Context::skyboxProgram);
	glUniformMatrix4fv(glGetUniformLocation(Context::skyboxProgram, "projection"), 1, false, glm::value_ptr(Context::camera.projection));
	glUniformMatrix4fv(glGetUniformLocation(Context::skyboxProgram, "view"), 1, false, glm::value_ptr(glm::mat4(glm::mat3(Context::camera.view))));

	glBindVertexArray(Context::skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);


	
	for (int i = 0; i < Context::instances.size(); ++i) {
		Instance& inst = Context::instances[i];
		Material* material = inst.material;
		Mesh* mesh = inst.mesh;
		material->bind();
		material->setMatrices(Context::camera.projection, Context::camera.view, inst.matrix);
		mesh->draw();
	}
}

/*
void charge(){
	Context::instances.resize(2);

}*/


void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	glFlush();
	glutSwapBuffers();
}




int main (int argc, char ** argv) {
	if (argc < 2) {
		std::cerr << "Missing parameter: <path-to-model>" << std::endl;
		exit (EXIT_FAILURE);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	window = glutCreateWindow("TP");

	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	key('?', 0, 0);

	std::string path(argv[1]);
	loadDataWithAssimp(path);
	beforeLoop();

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glutMainLoop();
	clearContext();
	return EXIT_SUCCESS;
}
