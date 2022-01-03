#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"EVAOBO.h"
#include"Camera1.h"
#include"Texture.h"

class Mesh1
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture1> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;
	// Initializes the mesh
	Mesh1(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture1>& textures);

	// Draws the mesh
	void Draw
	(
		Shader1& shader,
		Camera1& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
	//void Draw1
	//(
	//	Shader1& shader,
	//	Camera1& camera
	//);

};
#endif