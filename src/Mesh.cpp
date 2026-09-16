#include <iostream>

#include "Mesh.hpp"
#include "Window.hpp"

Shader* Mesh::shader;

void Mesh::init() {
    Mesh::shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
}

void Mesh::loadModel(std::string path) {
    Assimp::Importer import;

    const aiScene* scene = import.ReadFile(
	path,
	aiProcess_Triangulate |
	aiProcess_JoinIdenticalVertices |
	aiProcess_FlipUVs
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
	std::cout << "Error loading model : " << import.GetErrorString() << "\n";
    }

    std::string dir = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, dir);
}

void Mesh::processNode(aiNode* node, const aiScene* scene, std::string dir) {
    for (int i = 0; i < node->mNumMeshes; i++) {
	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	processMesh(mesh, scene, dir); 
    }

    for (int i = 0; i < node->mNumChildren; i++) {
	processNode(node->mChildren[i], scene, dir);
    }
}

void Mesh::processMesh(aiMesh* mesh, const aiScene* scene, std::string dir) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (int i = 0; i < mesh->mNumVertices; i++) {
	Vertex vertex;
	glm::vec3 vector;

	vector.x = mesh->mVertices[i].x;
	vector.y = mesh->mVertices[i].y;
	vector.z = mesh->mVertices[i].z; 
	vertex.position = vector;

	vector.x = mesh->mNormals[i].x;
	vector.y = mesh->mNormals[i].y;
	vector.z = mesh->mNormals[i].z;
	vertex.normal = vector;  

	if (mesh->mTextureCoords[0]) { 
	    glm::vec2 vec;
	    vec.x = mesh->mTextureCoords[0][i].x; 
	    vec.y = mesh->mTextureCoords[0][i].y;
	    vertex.texCoords = vec;
	} else vertex.texCoords = glm::vec2(0.0f, 0.0f);

	vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; i++) {
	aiFace face = mesh->mFaces[i];
	for (int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }  

    if (mesh->mMaterialIndex >= 0) {
	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, 
					    aiTextureType_DIFFUSE, "texture_diffuse", dir);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = loadMaterialTextures(material, 
					    aiTextureType_SPECULAR, "texture_specular", dir);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    new Mesh(vertices, indices, textures);
}

std::vector<Texture> Mesh::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::string directory) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture texture;
        //texture.id = TextureFromFile(str.C_Str(), directory);
        texture.type = typeName;
        textures.push_back(texture);
    }

    return textures;
}  

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures
    ) : indices(indices), textures(textures) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);

    setParent(Window::parent);
}

void Mesh::draw() {
    int diffuseNr = 1;
    int specularNr = 1;

    glUseProgram(shader->program);

    for (int i = 0; i < textures.size(); i++) {
	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
	std::string number;
	std::string name = textures[i].type;

        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if (name == "texture_specular") number = std::to_string(specularNr++);

        shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    sendMatrix();
    shader->setVec3("lightPos", glm::vec3(1.0, 0.2, 3.0));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::sendMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position);

    glm::mat4 projection = glm::perspective(
	glm::radians(70.0f),
	(float)Window::width / (float)Window::height,
	0.1f,
	100.0f
    );

    shader->setMatrix("model", model);
    shader->setMatrix("projection", projection);
}
