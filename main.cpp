#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int a, widht, height, gamma;

void draw(unsigned char** arr_pixels, int &widht, int &height, int &brightness, int &x1, int &y1, int &x2, int &y2, double &line_width, double &line_gamma) {
    int x3, y3; double j; double pnow; int check = 0;
    if (abs(y1 - y2) > abs(x1 - x2)) {
        check = 1;
        swap(x2, y2);
        swap(x1, y1);
    }
    if (x1 > x2) {
        swap(y1, y2);
        swap(x1, x2);
    }
    if  ((y1 > y2) && (x1 == x2)) {
        swap(x1, x2);
        swap(y1, y2);
    }
    double grad = 1.0 * (y2 - y1) / (x2 - x1);
    for (int absc = x1; absc < x2; absc++) {
        if (absc < 0 || absc >= widht) {continue;}
        j = (absc - x1) * grad + y1;
        int k1 = (line_width - 1) / 2; int k2 = (line_width + 1) / 2;
        for (int ordin = int(j - k1); ordin <= int(j + k2); ordin++) {
            if (ordin < 0 || ordin >= height) {continue;}
            x3 = absc; y3 = ordin;
            if (check) {swap(x3, y3);}
            double check1 = (k2 - abs(ordin - j)) < 1 ? (k2 - abs(ordin - j)) : 1.0;
            pnow = pow(check1 * brightness / 255.0, 0.5);
            (*arr_pixels)[x3 + y3 * widht] = pnow * 255;
        }
    }
}

int main(int argc, char * argv[]) {
    if (argc != 10){
        cerr << "input data can't be accepted \n";
        return 1;
    }
    FILE * input_file; FILE * output_file;
    input_file = fopen(argv[1], "rb");
    if (input_file == NULL){ //не удалось открыть файл
        cerr << "the file was not found \n";
        return 1;
    }
    int brightness = atoi(argv[3]);
    int x1 = atoi(argv[5]);
    int y1 = atoi(argv[6]);
    int x2 = atoi(argv[7]);
    int y2 = atoi(argv[8]);
    double line_w, line_g;
    string str4 = argv[4], str9 = argv[9];
    char s4 [str4.size()] = "";
    char s9 [str9.size()] = "";
    for (int i = 0; i < str4.size(); ++i) {
        s4[i] = str4[i];
    }
    for (int i = 0; i < str9.size(); ++i) {
        s9[i] = str9[i];
    }
    line_w = strtod(s4, 0);
    line_g = strtod(s9, 0);
    if (fscanf(input_file, "P%i%i%i%i\n", &a, &widht, &height, &gamma) != 4){
        cerr << "invalid file \n"; //нет 4х аргументов файла
        fclose(input_file);
        return 1;
    }
    if (a != 5) {
        cerr << "unsupported type of file \n";
        fclose(input_file);
        return 1;
    }
    unsigned char* arr_pixels = (unsigned char*)malloc(sizeof(unsigned char) * widht * height);
    if (arr_pixels == NULL){
        cerr << "allocation memory failed \n";
        fclose(input_file);
        free(arr_pixels);
        return 1;
    }
    if (fread(arr_pixels, sizeof(unsigned char), widht * height, input_file) != widht * height){
        cerr << "incorrect size of data in input file \n"; //некоректный размер массива данных
        fclose(input_file);
        free(arr_pixels);
        return 1;
    }
    output_file = fopen(argv[2], "wb");
    if (output_file == NULL){
        cerr << "the output file was not found \n"; //не удалось открыть файл
        fclose(input_file);
        free(arr_pixels);
        return 1;
    }

    brightness = 255 * pow((brightness + 0.0) / 255, 2);
    draw(&arr_pixels, widht, height, brightness, x1, y1, x2, y2, line_w, line_g);

    fprintf(output_file, "P%i\n%i %i\n%i\n", a, widht, height, gamma);
    fwrite(arr_pixels, sizeof(unsigned char), widht * height, output_file);
    
    free(arr_pixels);
    fclose(output_file);
    fclose(input_file);
    return 0;
}