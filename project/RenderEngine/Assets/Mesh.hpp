#ifndef MESH_HPP
#define MESH_HPP

#include "../IMesh.hpp"

#include <GL\glew.h>
#include <GL\GL.h>

class Mesh : public IMesh
{

public:

	virtual void init();
	virtual void release();

	virtual void setMeshData(void* data, size_t size, MeshDataLayout layout);
	
	virtual void bind();
	virtual void render();

	virtual void* map(size_t &dataSize);
	virtual void unmap();
	virtual void flush();

private:

	MeshDataLayout dataLayout;

	bool usesIndexBuffer;

	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;

	GLsizei vertexCount;
	size_t triangleCount;

	// mapping stuff

	bool isMapped;
	void* mapPtr;
	size_t mappedSize;

};

#endif