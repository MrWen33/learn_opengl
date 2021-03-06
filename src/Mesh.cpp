#include "Mesh.h"

using std::vector;
using std::string;
Mesh::Mesh(
	vector<Vertex> vertices, 
	vector<unsigned int> indices, 
	vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader & shader)
{
	shader.use();
	unsigned int diffID = 1;
	unsigned int specID = 1;
	for (size_t i = 0; i < textures.size(); ++i) {
		Texture& tex = textures[i];
		glActiveTexture(GL_TEXTURE0 + i);
		string num;
		string name = tex.type;
		if (name == "texture_diffuse") {
			num = std::to_string(diffID++);
		}
		else if (name == "texture_specular") {
			num = std::to_string(specID++);
		}
		shader.setFloat("material." + name + num, i);
		glBindTexture(GL_TEXTURE_2D, tex.ID);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER, 
		vertices.size() * sizeof(Vertex), 
		&vertices[0], 
		GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		indices.size() * sizeof(unsigned int),
		&indices[0], 
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, 
		GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 3, GL_FLOAT,
		GL_FALSE, sizeof(Vertex), 
		(void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, 2, GL_FLOAT,
		GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, TexCoords)
	);

	glBindVertexArray(0);
}
