#include <iostream>

#include "Mesh.hpp"
#include "Window.hpp"
#include "PointLight.hpp"

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

    processNode(scene->mRootNode, scene);
}

void Mesh::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	processMesh(mesh); 
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
	processNode(node->mChildren[i], scene);
    }
}

void Mesh::processMesh(aiMesh* mesh) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
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

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
	aiFace face = mesh->mFaces[i];
	for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }  

    new Mesh(vertices, indices);
}

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices
    ) : vertices(vertices), indices(indices) {
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

    material.diffuse.gen("textures/logo2.png");
    material.specular.gen("textures/logo2.png");

    setParent(Window::parent);
}

void Mesh::draw() {
    glUseProgram(shader->program);

    /*for (unsigned int i = 0; i < textures.size(); i++) {
	glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
	std::string number;
	std::string name = textures[i].type;

        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if (name == "texture_specular") number = std::to_string(specularNr++);

        shader->setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }*/

    glActiveTexture(GL_TEXTURE0);

    sendMatrix();
    shader->setMaterial(material);
    shader->setDirLight(Window::dirLight);
    shader->setVec3("viewPos", Window::camera.position);

    for (int i = 0; i < 8; i++) {
	shader->setPointLight(i, PointLight::lights[i]);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::sendMatrix() {
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::lookAt(
	Window::camera.position,
	Window::camera.position + Window::camera.forward,
	glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::vec3 worldRotation = getWorldRotation();

    model = glm::translate(model, getWorldPosition());
    model = glm::rotate(model, glm::radians(worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, size);

    glm::mat4 projection = glm::perspective(
	glm::radians(70.0f),
	(float)Window::width / (float)Window::height,
	0.1f,
	100.0f
    );

    shader->setMatrix("model", model);
    shader->setMatrix("view", view);
    shader->setMatrix("projection", projection);
}

glm::vec3 Mesh::getBounds() {
    glm::vec3 bounds(0.0f, 0.0f, 0.0f);
    glm::vec3 lowest(0.0f, 0.0f, 0.0f);
    glm::vec3 highest(0.0f, 0.0f, 0.0f);
    
    for (Vertex vertex : vertices) {
	if (vertex.position.x < lowest.x) lowest.x = vertex.position.x;
	else if (vertex.position.x > highest.x) highest.x = vertex.position.x;

	if (vertex.position.y < lowest.y) lowest.y = vertex.position.y;
	else if (vertex.position.y > highest.y) highest.y = vertex.position.y;

	if (vertex.position.z < lowest.z) lowest.z = vertex.position.z;
	else if (vertex.position.z > highest.z) highest.z = vertex.position.z;
    }

    bounds = highest - lowest;
    bounds *= size;

    return bounds;
}
