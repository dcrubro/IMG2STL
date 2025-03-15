//Written by DcruBro, GPLv3 License. This software is provided with no warranty of any kind, use at your own risk.
//This script generates an STL file based on the black to white ratio of a pixel as its height.
//Originally created to make Destiny 2 emblems printable, but can be adapted to other uses (Already is lol).
//Also, this code was written quickly, so bugs may be present, and the code quality is not at its finest.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>

void writeSTL(const std::string& fn, const std::vector<float>& vertices, const std::vector<int>& faces) {
    std::ofstream file(fn, std::ios::binary);

    //80-byte header shit
    char header[80] = {0};
    file.write(header, 80);

    //Triangle count
    uint32_t numTriangles = faces.size() / 3;
    file.write(reinterpret_cast<char*>(&numTriangles), 4);

    //Write each triangle
    for (size_t i = 0; i < faces.size(); i += 3) {
        float normal[3] = {0.0f, 0.0f, 1.0f};
        file.write(reinterpret_cast<char*>(normal), 12);

        //Write triangle vertices
        for (int j = 0; j < 3; j++) {
            float v[3] = {
                vertices[faces[i + j] * 3 + 0],
                vertices[faces[i + j] * 3 + 1],
                vertices[faces[i + j] * 3 + 2]
            };
            file.write(reinterpret_cast<char*>(v), 12);
        }

        uint16_t attr = 0;
        file.write(reinterpret_cast<char*>(&attr), 2);
    }

    file.close();
}

void generateHeightmapSTL(const std::string& imagePath, const std::string& outputSTL, float minHeight, float maxHeight, float scale, float minThickness) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(imagePath.c_str(), &width, &height, &channels, 1);

    if (!imgData) {
        std::cout << "Failed to load image: " << imagePath << "\n";
        return;
    }

    std::vector<float> vertices;
    std::vector<int> faces;

    float min_pixel_value = 255.0f;
    for (int i = 0; i < width * height; i++) {
        min_pixel_value = std::min(min_pixel_value, static_cast<float>(imgData[i]));
    }

    float min_surface_height = (min_pixel_value / 255.0f) * maxHeight;
    float base_height = min_surface_height - minThickness;
    if (base_height > 0.0f) base_height = 0.0f;

    std::cout << "Base height adjusted to: " << base_height << " mm\n";

    int vertex_count = 0;

    //Generate vertices for surface and base
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float pixelValue = imgData[y * width + x] / 255.0f;
            float z_top = pixelValue * maxHeight;
            
            //Top surface vertex
            vertices.push_back(x * scale);
            vertices.push_back(y * scale);
            vertices.push_back(z_top);

            //Base vertex (at the base height)
            vertices.push_back(x * scale);
            vertices.push_back(y * scale);
            vertices.push_back(base_height);
        }
    }

    int row_stride = width * 2; //Each row has both surface and base vertices

    //Generate faces for the solid model
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width - 1; x++) {
            int top_v0 = (y * width + x) * 2;
            int top_v1 = ((y * width + (x + 1)) * 2);
            int top_v2 = (((y + 1) * width + x) * 2);
            int top_v3 = (((y + 1) * width + (x + 1)) * 2);

            int base_v0 = top_v0 + 1;
            int base_v1 = top_v1 + 1;
            int base_v2 = top_v2 + 1;
            int base_v3 = top_v3 + 1;

            // Top surface
            faces.push_back(top_v0);
            faces.push_back(top_v1);
            faces.push_back(top_v2);

            faces.push_back(top_v1);
            faces.push_back(top_v3);
            faces.push_back(top_v2);

            // Bottom surface (base)
            faces.push_back(base_v0);
            faces.push_back(base_v2);
            faces.push_back(base_v1);

            faces.push_back(base_v1);
            faces.push_back(base_v2);
            faces.push_back(base_v3);

            //Vertical walls (they connect the base to the surface)
            //Side face 1
            faces.push_back(top_v0);
            faces.push_back(base_v0);
            faces.push_back(base_v1);

            faces.push_back(top_v0);
            faces.push_back(base_v1);
            faces.push_back(top_v1);

            //Side face 2
            faces.push_back(top_v2);
            faces.push_back(base_v2);
            faces.push_back(base_v3);

            faces.push_back(top_v2);
            faces.push_back(base_v3);
            faces.push_back(top_v3);

            //Side face 3
            faces.push_back(top_v0);
            faces.push_back(base_v0);
            faces.push_back(base_v2);

            faces.push_back(top_v0);
            faces.push_back(base_v2);
            faces.push_back(top_v2);

            //Side face 4
            faces.push_back(top_v1);
            faces.push_back(base_v1);
            faces.push_back(base_v3);

            faces.push_back(top_v1);
            faces.push_back(base_v3);
            faces.push_back(top_v3);
        }
    }

    writeSTL(outputSTL, vertices, faces);
    stbi_image_free(imgData);

    std::cout << "STL file saved: " << outputSTL << "\n";
}


int main(int argc, char **argv) {
    //Input args: imagePath, outPath, minMm, maxMm, scale, minThinkness
    //Current version requires specifing image details manually, may rewrite in the future.
    if (argc < 7) {
        std::cout << "Too few arguments specified. Exiting.\n";
        return 1;
    }

    //Convert the arguments from char-array to correct types
    std::string path(argv[1]);
    std::string outPath(argv[2]);
    float minMm = std::atof(argv[3]);
    float maxMm = std::atof(argv[4]);
    float scale = std::atof(argv[5]);
    float minThinkness = std::atof(argv[6]);

    generateHeightmapSTL(path, outPath, minMm, maxMm, scale, minThinkness);

    return 0;
}
