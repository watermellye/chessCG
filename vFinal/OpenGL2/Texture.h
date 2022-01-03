#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include"stb_image.h"

#include"shaderClass.h"

class Texture1
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture1(const char* image, const char* texType, GLuint slot);

	// Assigns a texture unit to a texture
	/*void texUnit(Shader1& shader, const char* uniform, GLuint unit);*/
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif