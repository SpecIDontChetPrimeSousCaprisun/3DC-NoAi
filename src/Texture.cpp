#include <stb/stb_image.h>
#include <glad/glad.h>
#include <iostream>

#include "Texture.hpp"

Texture::Texture() {
    glGenTextures(1, &id);    
}

void Texture::gen(std::string path) {
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    } else {
	std::cout << "Failed to load texture with path: " << path << "\n";
    }

    stbi_image_free(data);
}
