#include <iostream>

#include "Mesh.hpp"
#include "Window.hpp"
#include "PointLight.hpp"

std::vector<Mesh*> Mesh::meshes;
Shader* Mesh::shader;

void Mesh::init() {
    Mesh::shader = new Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
}

std::vector<Mesh*> Mesh::loadModel(std::string path) {
    std::vector<Mesh*> meshes;
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

    for (Mesh* mesh : processNode(scene->mRootNode, scene)) {
	meshes.push_back(mesh);
    }

    return meshes;
}

std::vector<Mesh*> Mesh::processNode(aiNode* node, const aiScene* scene) {
    std::vector<Mesh*> meshes;

    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	meshes.push_back(processMesh(mesh)); 
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
	for (Mesh* mesh : processNode(node->mChildren[i], scene)) {
	    meshes.push_back(mesh);
	}
    }

    return meshes;
}

Mesh* Mesh::processMesh(aiMesh* mesh) {
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

    return new Mesh(vertices, indices);
}

aiMesh* Mesh::getFirstMesh(const aiScene* scene, aiNode* node) {
    if (node->mNumMeshes > 0) return scene->mMeshes[node->mMeshes[0]];

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
	aiMesh* mesh = getFirstMesh(scene, node->mChildren[i]);

	if (mesh) return mesh;
    }

    return nullptr;
}

Mesh::Mesh(std::string path) {
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

    aiMesh* mesh = getFirstMesh(scene, scene->mRootNode);

    if (!mesh) {
	std::cout << "Failed to process mesh with path " << path << "\n";
	return;
    }

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

    this->vertices = vertices;
    this->indices = indices;

    init(vertices, indices);
}

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices
    ) : vertices(vertices), indices(indices) {
    init(vertices, indices);
}

void Mesh::init(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
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
    meshes.push_back(this);
}

void Mesh::draw() {
    if (transparency >= 1.0f) return;

    glUseProgram(shader->program);

    sendMatrix();
    shader->setMaterial(material);
    shader->setDirLight(Window::dirLight);
    shader->setVec3("viewPos", Window::camera.position);
    shader->setFloat("transparency", transparency);

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

BoundResult Mesh::getBounds() {
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 worldRotation = getWorldRotation();

    model = glm::rotate(model, glm::radians(worldRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(worldRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(worldRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 bounds(0.0f, 0.0f, 0.0f);
    glm::vec3 lowest(0.0f, 0.0f, 0.0f);
    glm::vec3 highest(0.0f, 0.0f, 0.0f);
    
    for (Vertex vertex : vertices) {
	glm::vec3 pos = model * glm::vec4(vertex.position, 1.0f);

	if (pos.x < lowest.x) lowest.x = pos.x;
	else if (pos.x > highest.x) highest.x = pos.x;

	if (pos.y < lowest.y) lowest.y = pos.y;
	else if (pos.y > highest.y) highest.y = pos.y;

	if (pos.z < lowest.z) lowest.z = pos.z;
	else if (pos.z > highest.z) highest.z = pos.z;
    }

    bounds *= size;

    BoundResult result;

    result.min = lowest * size;
    result.max = highest * size;
    result.size = bounds;

    return result;
}

void Mesh::beforeUpdate() {}

void Mesh::update() {
    beforeUpdate();

    if (anchored) return;

    linearVelocity.y -= 1.0f * (float)Window::dt;

    if (!canCollide) return;
    for (Mesh* other : meshes) {
	if (other == this) continue;
	if (!other->canCollide) continue;
	if (intersects(*other)) resolveCollision(*other);
    }
}

bool Mesh::intersects(Mesh other) {
    BoundResult bounds = getBounds();
    BoundResult oBounds = other.getBounds();

    glm::vec3 min = getWorldPosition() + bounds.min;
    glm::vec3 max = getWorldPosition() + bounds.max;

    glm::vec3 oMin = other.getWorldPosition() + oBounds.min;
    glm::vec3 oMax = other.getWorldPosition() + oBounds.max;

    return min.x <= oMax.x &&
	   max.x >= oMin.x &&
	   min.y <= oMax.y &&
	   max.y >= oMin.y &&
	   min.z <= oMax.z &&
	   max.z >= oMin.z;
}

void Mesh::resolveCollision(Mesh other) {
    BoundResult bounds = getBounds();
    BoundResult oBounds = other.getBounds();

    glm::vec3 min = getWorldPosition() + bounds.min;
    glm::vec3 max = getWorldPosition() + bounds.max;

    glm::vec3 oMin = other.getWorldPosition() + oBounds.min;
    glm::vec3 oMax = other.getWorldPosition() + oBounds.max;
    
    glm::vec3 overlap(
	std::min(max.x, oMax.x) - std::max(min.x, oMin.x),
	std::min(max.y, oMax.y) - std::max(min.y, oMin.y),
	std::min(max.z, oMax.z) - std::max(min.z, oMin.z)
    );
	
    if (overlap.x < overlap.y && overlap.x < overlap.z) {
        setWorldPosition(glm::vec3((getWorldPosition().x < other.getWorldPosition().x) ? -overlap.x : overlap.x, 0, 0));
	linearVelocity.x = 0.0f;
    } else if (overlap.y < overlap.z) {
        setWorldPosition(glm::vec3(0, (getWorldPosition().y < other.getWorldPosition().y) ? -overlap.y : overlap.y, 0));
	linearVelocity.y = 0.0f;
    } else {
	setWorldPosition(glm::vec3(0, 0, (getWorldPosition().z < other.getWorldPosition().z) ? -overlap.z : overlap.z));
	linearVelocity.z = 0.0f;
    }
}
