#ifndef SISTEMA_H
#define SISTEMA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

class ImageProcessingSystem {
private:
    //  Atributos privados
    vector<vector<int>> imageData;
    int width, height, maxPixelValue;
    string imageFilename;
    string volume;
    vector<vector<vector<int>>> volumeData;

    //  Métodos privados
    void loadImage(string filename);
    void loadVolume(string param);
    void infoImage();
    void infoVolume();
    void projection2D(string param);
    void encodeImage(string param);
    void decodeFile(string param);
    void segmentImage(string param);

    void handleCommand(string command);
    void showHelp(string cmd);

public:
    void start();  // Único método público
};

#endif

