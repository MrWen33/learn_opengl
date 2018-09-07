#include "Model.h"
#include"utils.h"
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::vector;
void Model::Draw(Shader & shader)
{
	for (auto& mesh : meshes) {
		mesh.Draw(shader);
	}
}
void Model::load_model(string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, 
		aiProcess_Triangulate|aiProcess_FlipUVs|
		aiProcess_GenNormals);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode * node, const aiScene * scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(std::move(process_mesh(mesh, scene)));
	}
	for (size_t i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh * mesh, const aiScene * scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		auto& pos_info = mesh->mVertices[i];
		auto& normal_info = mesh->mNormals[i];
		vertex.position = glm::vec3(pos_info.x, pos_info.y, pos_info.z);
		vertex.normal = glm::vec3(normal_info.x, normal_info.y, normal_info.z);
		if (mesh->mTextureCoords[0]) {
			auto& tex_info = mesh->mTextureCoords[0][i];
			vertex.TexCoords = glm::vec2(tex_info.x, tex_info.y);
		}
		else {
			vertex.TexCoords = glm::vec2(0, 0);
		}
		vertices.push_back(vertex);
	}
	
	for (size_t i = 0; i < mesh->mNumFaces; ++i) {
		auto& face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		auto diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
		auto spec_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), spec_maps.begin(), spec_maps.end());
	}

}

vector<Texture> Model::load_material_textures(aiMaterial * material, aiTextureType type, std::string typeName)
{
	vector<Texture> textures;
	for (size_t i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		Texture texture;

		texture.ID = loadMaterial(directory + string(str.C_Str()));
		texture.type = typeName;
		texture.path = string(str.C_Str());
		textures.push_back(texture);

	}
	return textures;
}
