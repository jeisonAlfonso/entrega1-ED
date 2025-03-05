#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm> // Para sort()
#include <numeric>   // Para accumulate()
using namespace std;
/// @brief Clase que implementa un sistema de procesamiento de imágenes.
/// Permite cargar imágenes en formato PGM, procesarlas y realizar diversas operaciones.

class ImageProcessingSystem {
private:
    vector<vector<int>> imageData; ///< Matriz que almacena la imagen en memoria.
    int width, height, maxPixelValue; ///< Dimensiones y valor máximo del píxel en la imagen.
    string imageFilename; ///< Nombre del archivo de imagen cargado.
    string volume; ///< Nombre del volumen de imágenes cargado.
    vector<vector<vector<int>>> volumeData; ///< Volumen de imágenes en memoria.

public:
     /// @brief Inicia el sistema de procesamiento de imágenes e interactúa con el usuario.
    void start() {
        cout << "Bienvenido al Sistema de Procesamiento de Imágenes. Escriba 'ayuda' para ver los comandos disponibles." << endl;
        string command;

        while (true) {
            cout << "$ ";
            getline(cin, command);

            if (command == "salir") {
                cout << "Saliendo del sistema..." << endl;
                break;
            }

            handleCommand(command);
        }
    }

private:
    /// @brief Maneja los comandos ingresados por el usuario.
    /// @param command Comando ingresado en la terminal.
    void handleCommand(string command) {
        string cmd, param, extra;
        int spacePos = command.find(' ');

        if (spacePos != string::npos) {
            cmd = command.substr(0, spacePos);
            param = command.substr(spacePos + 1);
        } else {
            cmd = command;
        }

        if (cmd == "ayuda") {
            showHelp(param);
        } else if (cmd == "cargar_imagen") {
            loadImage(param);
        } else if (cmd == "cargar_volumen") {
            loadVolume(param);
        } else if (cmd == "info_imagen") {
            infoImage();
        } else if (cmd == "info_volumen") {
            infoVolume();
        } else if (cmd == "proyeccion2D") {
            projection2D(param);
        } else if (cmd == "codificar_imagen") {
            encodeImage(param);
        } else if (cmd == "decodificar_archivo") {
            decodeFile(param);
        } else if (cmd == "segmentar") {
            segmentImage(param);
        } else {
            cout << "Comando no reconocido. Escriba 'ayuda' para ver los comandos disponibles." << endl;
        }
    }
    /// @brief Muestra información sobre los comandos disponibles o detalles específicos de un comando.
    /// @param cmd Comando sobre el cual se desea obtener ayuda.

    void showHelp(string cmd) {
        if (cmd.empty()) {
            cout << "Comandos disponibles:\n"
                 << "  cargar_imagen <nombre_imagen.pgm>\n"
                 << "  cargar_volumen <nombre_base> <n_im>\n"
                 << "  info_imagen\n"
                 << "  info_volumen\n"
                 << "  proyeccion2D <dirección> <criterio> <nombre_archivo.pgm>\n"
                 << "  codificar_imagen <nombre_archivo.huf>\n"
                 << "  decodificar_archivo <nombre_archivo.huf> <nombre_imagen.pgm>\n"
                 << "  segmentar <salida_imagen.pgm> <sx1> <sy1> <sl1> [<sx2> <sy2> <sl2> ...]\n"
                 << "  salir" << endl;
        } else {
            if (cmd == "cargar_imagen") {
                cout << "Uso: cargar_imagen <nombre_imagen.pgm>\nCarga una imagen en memoria." << endl;
            } else if (cmd == "cargar_volumen") {
                cout << "Uso: cargar_volumen <nombre_base> <n_im>\nCarga un volumen de imágenes en memoria." << endl;
            } else if (cmd == "info_imagen") {
                cout << "Uso: info_imagen\nMuestra información de la imagen cargada." << endl;
            } else if (cmd == "info_volumen") {
                cout << "Uso: info_volumen\nMuestra información del volumen cargado." << endl;
            } else if (cmd == "proyeccion2D") {
                cout << "Uso: proyeccion2D <dirección> <criterio> <nombre_archivo.pgm>\nGenera una proyección 2D del volumen cargado." << endl;
            } else if (cmd == "codificar_imagen") {
                cout << "Uso: codificar_imagen <nombre_archivo.huf>\nCodifica la imagen cargada usando Huffman." << endl;
            } else if (cmd == "decodificar_archivo") {
                cout << "Uso: decodificar_archivo <nombre_archivo.huf> <nombre_imagen.pgm>\nDecodifica un archivo de Huffman a una imagen." << endl;
            } else if (cmd == "segmentar") {
                cout << "Uso: segmentar <salida_imagen.pgm> <sx1> <sy1> <sl1> [<sx2> <sy2> <sl2> ...]\nSegmenta la imagen cargada utilizando semillas." << endl;
            } else {
                cout << "No hay ayuda disponible para el comando '" << cmd << "'." << endl;
            }
        }
    }
    /// metodo para mostrar la informacion que se guardo de la imagen en el vector, incluyendo los datos
    void mostrarImagen() {
    cout << "Contenido de la imagen cargada (" << width << "x" << height << "):" << endl;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << imageData[i][j] << " ";
        }
        cout << endl;
    }
}
      /// @brief Carga una imagen en formato PGM en memoria.
    /// @param filename Nombre del archivo de imagen a cargar en el vector.

 void loadImage(string filename) {
    if (filename.empty()) {
        cout << "Uso incorrecto del comando. Escriba 'ayuda cargar_imagen' para más información." << endl;
        return;
    }

    ifstream file(filename);
    if (!file) {
        cout << "La imagen " << filename << " no ha podido ser cargada." << endl;
        return;
    }

    string format;
    file >> format;
    if (format != "P2") {
        cout << "Error: El archivo no está en formato PGM (P2)." << endl;
        return;
    }

    // Leer width, height y maxPixelValue, ignorando comentarios
    width = height = maxPixelValue = -1;
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;  // Saltar líneas vacías y comentarios

        stringstream ss(line);
        if (width == -1) {
            ss >> width >> height;  // Leer dimensiones
        } else {
            ss >> maxPixelValue;  // Leer valor máximo de píxel
            break;  // Una vez leído, salimos del bucle
        }
    }

    if (width <= 0 || height <= 0 || maxPixelValue <= 0 || maxPixelValue > 255) {
        cout << "Error: Formato PGM inválido." << endl;
        return;
    }

    // Leer los datos de la imagen
    imageData.assign(height, vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!(file >> imageData[i][j]) || imageData[i][j] < 0 || imageData[i][j] > maxPixelValue) {
                cout << "Error: Datos de imagen corruptos." << endl;
                return;
            }
        }
    }

    imageFilename = filename;
    cout << "La imagen " << filename << " ha sido cargada." << endl;
    mostrarImagen();
}

  void loadVolume(string param) {
    int spacePos = param.find(' ');
    if (spacePos == string::npos) {
        cout << "Uso incorrecto del comando. Escriba 'ayuda cargar_volumen' para más información." << endl;
        return;
    }

    string base = param.substr(0, spacePos);
    string numStr = param.substr(spacePos + 1);

    int num_images;
    try {
        num_images = stoi(numStr);
    } catch (exception &e) {
        cout << "Error: Número de imágenes no válido." << endl;
        return;
    }

    if (num_images < 1 || num_images > 99) {
        cout << "Error: La cantidad de imágenes debe estar entre 1 y 99." << endl;
        return;
    }

    volumeData.clear(); // Almacenar todas las imágenes cargadas
    int commonWidth = -1, commonHeight = -1, commonMaxPixel = -1;

    for (int i = 1; i <= num_images; i++) {
        stringstream ss;
        ss << base << (i < 10 ? "0" : "") << i << ".pgm"; // Genera nombres nombre_base01.pgm, nombre_base02.pgm, etc.
        string filename = ss.str();

        // Llamamos a la función loadImage para cargar la imagen en memoria
        string prevFilename = imageFilename;
        vector<vector<int>> prevImageData = imageData; // Guardamos la imagen actual temporalmente
        int prevWidth = width, prevHeight = height, prevMaxPixel = maxPixelValue;

        loadImage(filename);

        if (imageFilename.empty()) { // Si hubo error al cargar la imagen, restauramos la imagen anterior
            imageFilename = prevFilename;
            imageData = prevImageData;
            width = prevWidth;
            height = prevHeight;
            maxPixelValue = prevMaxPixel;
            cout << "Error: No se pudo cargar " << filename << endl;
            return;
        }

        // Validar que todas las imágenes tengan el mismo tamaño
        if (commonWidth == -1) {
            commonWidth = width;
            commonHeight = height;
            commonMaxPixel = maxPixelValue;
        } else if (width != commonWidth || height != commonHeight || maxPixelValue != commonMaxPixel) {
            cout << "Error: Las dimensiones de " << filename << " no coinciden con las anteriores." << endl;
            return;
        }

        // Agregamos la imagen al volumen
        volumeData.push_back(imageData);
    }

    // Guardamos el volumen en memoria solo si todas las imágenes fueron cargadas correctamente
    volume = base;
    cout << "El volumen " << base << " ha sido cargado con " << num_images << " imágenes." << endl;
}
      /// @brief Muestra información sobre la imagen cargada.

    void infoImage() {
        if (imageFilename.empty()) {
            cout << "No hay una imagen cargada en memoria." << endl;
        } else {
            cout << "Imagen cargada en memoria: " << imageFilename << endl;
            cout << "Dimensiones: " << width << " x " << height << " píxeles" << endl;
            cout << "Valor máximo de píxel: " << maxPixelValue << endl;
        }
    }
 /// @brief Muestra información sobre el volumen cargado.

    void infoVolume() {
        if (volume.empty() || volumeData.empty()) {
            cout << "No hay un volumen cargado en memoria." << endl;
        } else {
            cout << "Volumen cargado en memoria: " << volume << endl;
            cout << "Cantidad de imágenes: " << volumeData.size() << endl;
        if (!volumeData.empty()) {
            cout << "Dimensiones de cada imagen: " << width << " x " << height << " píxeles" << endl;
            cout << "Valor máximo de píxel: " << maxPixelValue << endl;
        }
        }
    }
    /// @brief Genera una proyección 2D de un volumen cargado según un criterio dado.
    /// @param param Dirección de la proyección (x, y o z), criterio (max, min, prom, med) y nombre del archivo de salida.

    void projection2D(string param) {
        stringstream ss(param);
    string direccion, criterio, filename;

    ss >> direccion >> criterio >> filename;

    if (direccion.empty() || criterio.empty() || filename.empty()) {
        cout << "Uso incorrecto del comando. Escriba 'ayuda proyeccion2D' para más información." << endl;
        return;
    }

    if (volumeData.empty()) {
        cout << "Error: No hay un volumen cargado en memoria." << endl;
        return;
    }

    int profundidad = volumeData.size(); // Número de imágenes en el volumen
    vector<vector<int>> resultado;

    if (direccion == "z") {  // Proyección en el eje Z (vista superior)
        resultado.assign(height, vector<int>(width, 0));

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                vector<int> valoresZ;
                for (int k = 0; k < profundidad; k++) {
                    valoresZ.push_back(volumeData[k][i][j]);
                }

                if (criterio == "max") {
                    resultado[i][j] = *max_element(valoresZ.begin(), valoresZ.end());
                } else if (criterio == "min") {
                    resultado[i][j] = *min_element(valoresZ.begin(), valoresZ.end());
                } else if (criterio == "prom") {
                    resultado[i][j] = accumulate(valoresZ.begin(), valoresZ.end(), 0) / profundidad;
                } else if (criterio == "med") {
                    sort(valoresZ.begin(), valoresZ.end());
                    int n = valoresZ.size();
                    if (n % 2 == 1) {
                        resultado[i][j] = valoresZ[n / 2]; // Si es impar, tomamos el del centro
                    } else {
                        resultado[i][j] = (valoresZ[n / 2 - 1] + valoresZ[n / 2]) / 2; // Si es par, promediamos los dos del centro
                    }
                } else {
                    cout << "Error: Criterio no válido. Use 'max', 'min', 'prom' o 'med'." << endl;
                    return;
                }
            }
        }

    } else if (direccion == "y") {  // Proyección en el eje Y (vista lateral)
        resultado.assign(profundidad, vector<int>(width, 0));

        for (int k = 0; k < profundidad; k++) {
            for (int j = 0; j < width; j++) {
                vector<int> valoresY;
                for (int i = 0; i < height; i++) {
                    valoresY.push_back(volumeData[k][i][j]);
                }

                if (criterio == "max") {
                    resultado[k][j] = *max_element(valoresY.begin(), valoresY.end());
                } else if (criterio == "min") {
                    resultado[k][j] = *min_element(valoresY.begin(), valoresY.end());
                } else if (criterio == "prom") {
                    resultado[k][j] = accumulate(valoresY.begin(), valoresY.end(), 0) / height;
                } else if (criterio == "med") {
                    sort(valoresY.begin(), valoresY.end());
                    int n = valoresY.size();
                    if (n % 2 == 1) {
                        resultado[k][j] = valoresY[n / 2]; // Si es impar, tomamos el del centro
                    } else {
                        resultado[k][j] = (valoresY[n / 2 - 1] + valoresY[n / 2]) / 2; // Si es par, promediamos los dos del centro
                    }
                } else {
                    cout << "Error: Criterio no válido. Use 'max', 'min', 'prom' o 'med'." << endl;
                    return;
                }
            }
        }

    } else if (direccion == "x") {  // Proyección en el eje X (vista frontal)
        resultado.assign(height, vector<int>(profundidad, 0));

        for (int i = 0; i < height; i++) {
            for (int k = 0; k < profundidad; k++) {
                vector<int> valoresX;
                for (int j = 0; j < width; j++) {
                    valoresX.push_back(volumeData[k][i][j]);
                }

                if (criterio == "max") {
                    resultado[i][k] = *max_element(valoresX.begin(), valoresX.end());
                } else if (criterio == "min") {
                    resultado[i][k] = *min_element(valoresX.begin(), valoresX.end());
                } else if (criterio == "prom") {
                    resultado[i][k] = accumulate(valoresX.begin(), valoresX.end(), 0) / width;
                } else if (criterio == "med") {
                    sort(valoresX.begin(), valoresX.end());
                    int n = valoresX.size();
                    if (n % 2 == 1) {
                        resultado[i][k] = valoresX[n / 2]; // Si es impar, tomamos el del centro
                    } else {
                        resultado[i][k] = (valoresX[n / 2 - 1] + valoresX[n / 2]) / 2; // Si es par, promediamos los dos del centro
                    }
                } else {
                    cout << "Error: Criterio no válido. Use 'max', 'min', 'prom' o 'med'." << endl;
                    return;
                }
            }
        }

    } else {
        cout << "Error: Dirección no válida. Use 'x', 'y' o 'z'." << endl;
        return;
    }

    // Guardar la imagen proyectada en un archivo PGM
    ofstream file(filename);
    if (!file) {
        cout << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }

    file << "P2\n";
    file << "# Proyección 2D generada\n";

    if (direccion == "z") {
        file << width << " " << height << "\n";
    } else if (direccion == "y") {
        file << width << " " << profundidad << "\n";
    } else if (direccion == "x") {
        file << profundidad << " " << height << "\n";
    }

    file << maxPixelValue << "\n";

    for (const auto& fila : resultado) {
        for (int pixel : fila) {
            file << pixel << " ";
        }
        file << "\n";
    }

    file.close();
    cout << "Proyección 2D guardada en " << filename << endl;
    }

    void encodeImage(string param) {
        cout << "No hay una imagen cargada en memoria." << endl;
    }

    void decodeFile(string param) {
        cout << "El archivo no ha podido ser decodificado." << endl;
    }

    void segmentImage(string param) {
        cout << "No hay una imagen cargada en memoria." << endl;
    }
};
/// @brief Función principal que inicia el sistema de procesamiento de imágenes.
/// @return Código de salida del programa.

int main() {
    ImageProcessingSystem system;
    system.start();
    return 0;
}
