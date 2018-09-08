#pragma once
#include<string>
#include<vector>
#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class Model {
public:
	Model(std::string path) {
		load_model(path);
	}
	void Draw(Shader& shader);
private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures_loaded;
	std::string directory;
	void load_model(std::string path);
	void process_node(aiNode *node, const aiScene *scene);
	Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> load_material_textures(aiMaterial *material, aiTextureType type, std::string typeName);
	bool is_loaded(const std::string& texture_path);
};