// Local includes
#include "Material.h"
#include "Shader.h"
#include "Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
#include "Texture.h"
#include <vector>
#include <string>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	//m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	//texture et normal map
	//m_program = load_shaders("../shaders/unlit/vertex.glsl", "../shaders/unlit/fragment.glsl");
	//pbr
	//m_program = load_shaders("../shaders/unlit/vertex.glsl", "../shaders/unlit/fragment_PBR.glsl");
	//skybox
	m_program = load_shaders("../shaders/unlit/vertex.glsl", "../shaders/unlit/fragment_refl.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = loadTexture2DFromFilePath("../data/BoomBox_baseColor.png");;
	//m_texture = loadTexture2DFromFilePath("../data/chair/chair_wood_albedo.png");;
	//m_texture = loadTexture2DFromFilePath("../data/TwoSided	Plane_BaseColor.png");;
	setDefaultTexture2DParameters(m_texture);

	//normal_texture=loadTexture2DFromFilePath("../data/BoomBox_normal.png");;
	normal_texture=loadTexture2DFromFilePath("../data/chair/chair_fabric_normal.png");;
	setDefaultTexture2DParameters(normal_texture);

	metallic_texture=loadTexture2DFromFilePath("../data/chair/chair_woodbrown_roughnessmetallic.png");;
	setDefaultTexture2DParameters(metallic_texture);

	roughness_texture=loadTexture2DFromFilePath("../data/chair/chair_woodblack_roughnessmetallic.png");;
	setDefaultTexture2DParameters(roughness_texture);

	albedo_texture==loadTexture2DFromFilePath("../data/chair/chair_wood_albedo.png");;
	setDefaultTexture2DParameters(albedo_texture);


}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// bind parameters
	/*
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture);		
		glUniform1i(getUniform("colorTexture"), 0);
		
	}
	if (normal_texture != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, normal_texture);		
		glUniform1i(getUniform("normalMap"), 0);
		
	}

	if (metallic_texture != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, metallic_texture);		
		glUniform1i(getUniform("metallicMap"), 1);
		
	}
	if (roughness_texture != -1) {
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, normal_texture);		
		glUniform1i(getUniform("roughnessMap"), 2);
		
	}

	if (albedo_texture != -1) {
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, metallic_texture);		
		glUniform1i(getUniform("albedoMap"), 3);
		
	}
	
	glm::vec3 lightPosition(1.0f, 1.0f, 1.0f);
	GLint lightPosLoc = glGetUniformLocation(m_program, "light_pos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosition));

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	GLint Color = glGetUniformLocation(m_program, "light_color");
	glUniform3fv(Color, 1, glm::value_ptr(lightColor));

	//glm::vec3 Camera(1.0f, 0.f, 0.f);
	GLint CamPosLoc = glGetUniformLocation(m_program, "camera");
	//glUniform3fv(CamPosLoc, 1, glm::value_ptr(Camera));
	glUniform3fv(CamPosLoc, 1, glm::value_ptr(Context::camera.position));
	*/
	
	//skybox
	glUniform3fv(getUniform("camera"), 1, glm::value_ptr(Context::camera.position));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Context::skyboxTexture);
	
	

	// TODO : Add your custom parameters here
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
