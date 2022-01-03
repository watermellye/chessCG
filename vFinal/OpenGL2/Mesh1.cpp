#include "Mesh1.h"

Mesh1::Mesh1(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture1>& textures)
{
	Mesh1::vertices = vertices;
	Mesh1::indices = indices;
	Mesh1::textures = textures;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	//VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh1::Draw
(
	Shader1& shader,
	Camera1& camera,
	glm::mat4 matrix,
	glm::vec3 translation,
	glm::quat rotation,
	glm::vec3 scale
)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();
	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;
	unsigned int numnormal = 0; //后加的
	//光照
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "albedoMap")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "metallicMap")
		{
			num = std::to_string(numSpecular++);
		}
		else if (type == "normalMap") //后加的
		{
			num = std::to_string(numnormal++);
		}
		//这里调用了texture类，加normal类
		// Gets the location of the uniform
		// Shader needs to be activated before changing the value of a uniform
		shader.Activate();
		// Sets the value of the uniform
		glUniform1i(glGetUniformLocation(shader.ID, (type + num).c_str()), i);
		 //首先是setint,插入的是0、1、2
		textures[i].Bind(); //active 
	}

	// Take care of the camera Matrix
	//shader传入camPos
	shader.setVec3("camPos", camera.Position);
	//shader传入camMatrix
	camera.Matrix(shader, "camMatrix");

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Push the matrices to the vertex shader
	shader.setMat4("translation", trans);
	shader.setMat4("rotation", rot);
	shader.setMat4("scale", sca);
	shader.setMat4("model", matrix);
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
