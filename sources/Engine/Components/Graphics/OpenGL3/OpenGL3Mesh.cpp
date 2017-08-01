#include "OpenGL3Mesh.h"

OpenGL3Mesh::OpenGL3Mesh()
	: m_vertexArrayObject(NULL), m_vertexBufferObject(NULL), m_elementBufferObject(NULL)
{

}

OpenGL3Mesh::OpenGL3Mesh(const std::vector<Vertex>& vertices)
	: m_vertices(vertices),
	m_vertexArrayObject(NULL), m_vertexBufferObject(NULL), m_elementBufferObject(NULL)
{
	updateBuffers();
}

OpenGL3Mesh::OpenGL3Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices) 
	: m_vertices(vertices), m_indices(indices),
	m_vertexArrayObject(NULL), m_vertexBufferObject(NULL), m_elementBufferObject(NULL) 
{
	updateBuffers();
}

OpenGL3Mesh::~OpenGL3Mesh() {
	if (m_vertexArrayObject) {
		glDeleteVertexArrays(1, &m_vertexArrayObject);
	}

	if (m_vertexBufferObject) {
		glDeleteBuffers(1, &m_vertexBufferObject);
	}

	if (m_elementBufferObject) {
		glDeleteBuffers(1, &m_elementBufferObject);
	}
}

void OpenGL3Mesh::addVertex(const Vertex& vertex) {
	m_vertices.push_back(vertex);
}

void OpenGL3Mesh::addIndex(uint32 index) {
	m_indices.push_back(index);
}

void OpenGL3Mesh::updateBuffers() {
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	if (m_indices.size() > 0) {
		glGenBuffers(1, &m_elementBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint32), m_indices.data(), GL_STATIC_DRAW);
	}

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoordinates));

	glBindVertexArray(0);
}

GLuint OpenGL3Mesh::getVertexArrayObjectPointer() const {
	return m_vertexArrayObject;
}

size_t OpenGL3Mesh::getVerticesCount() const {
	return m_vertices.size();
}

size_t OpenGL3Mesh::getIndicesCount() const {
	return m_indices.size();
}