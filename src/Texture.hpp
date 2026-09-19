#pragma once

#include <string>

class Texture {
public:
    Texture();

    void gen(std::string path);

    unsigned int id;
private:
    int width;
    int height;
    int nrChannels;
};
