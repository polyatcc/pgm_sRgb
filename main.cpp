#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

using namespace std;

int a, widht, height, gamma;

double pow2(double x, int power) {
    if (power == 1) {
        return x;
    }
    if (power == 0) {
        return 1;
    }
    return (pow2(x * x, power / 2) * ((power % 2 == 0) ? 1 : x));
}

double distance_to(double& len_x, double& len_y, double& x1, double& y1, double& x2, double& y2, int& now_s_x, int& now_s_y, double& line_width) {
    double y_x = len_y / len_x, x_y = len_x / len_y;
    double koord_pr_x = (y_x) * x1 + x_y * now_s_x - y1 + now_s_y;
    koord_pr_x = koord_pr_x / (y_x + x_y);
    double koord_pr_y = x_y * ((double)(now_s_x) - koord_pr_x) + now_s_y;
    double dist_pr = sqrt(pow2(koord_pr_x - now_s_x, 2) + pow2(koord_pr_y - now_s_y, 2));

    if (koord_pr_x < x1) {
        double dist_ot = sqrt(pow2(x1 - koord_pr_x, 2) + pow2(y1 - koord_pr_y, 2));
        double korrect_dist = sqrt(pow2(dist_pr, 2) + pow2(dist_ot, 2));
        if ((line_width + 1) / 2 - korrect_dist > 0) {
            return min((line_width + 1) / 2 - dist_pr, 1 - dist_ot);
        }
        return 0;
    }

    if ((koord_pr_x >= x1) && (koord_pr_x <= x2)) {
        return (line_width + 1) / 2 - dist_pr;
    }
    double dist_ot = sqrt(pow2(x2 - koord_pr_x, 2) + pow2(y2 - koord_pr_y, 2));
    double korrect_dist = sqrt(pow2(dist_pr, 2) + pow2(dist_ot, 2));
    if ((line_width + 1) / 2 - korrect_dist > 0) {
        return min((line_width + 1) / 2 - dist_pr, 1 - dist_ot);
    }
    return 0;
}


void counter(int& now_s_x, int& now_s_y, double& dist, int& brightness, unsigned char* arr_pix, double& line_gamma) {
    if (dist > 1) {
        dist = 1;
    } else if (dist < 0) {
        dist = 0;
    }
    if ((now_s_x >= 0) && (now_s_x < widht) && (now_s_y >= 0) && (now_s_y < height)) {
        double pix = arr_pix[now_s_y * widht +  now_s_x] / 255.0, br = brightness / 255.0;
        double s2, s;
            if (pix <= 0.04045) {
                s = 25 * pix / 323;
            } else {
                s = pow((200 * pix + 11) / 211, line_gamma);
            }
            if (br <= 0.04045) {
                s2 = 25 * br / 323;
            } else {
                s2 = pow((200 * br + 11) / 211, line_gamma);
            }

            double final = s * (1 - dist) + s2 * dist;
            double fin_pix;
            if (final <= 0.0031308) {
                fin_pix = 323 * final / 25;
            } else {
                fin_pix = (211 * pow(final, (1 / line_gamma)) - 11) / 200;
            }
            arr_pix[now_s_y * widht + now_s_x] = round(fin_pix * 255);
        }

}

void processing(int& now_x, int& now_y, double& len_x, double& len_y, double& x1, double& x2, double& y1, double& y2, double& lwidth, unsigned char* arr_pix, int& brightness, double& line_gamma, int& ch) {
    int now_s_x = now_x, now_s_y = now_y;
    while (now_s_y >= 0) {
        double distance = distance_to(len_x, len_y, x1, y1, x2, y2, now_s_x, now_s_y, lwidth);
        if (distance > 0) {
            counter(now_s_x, now_s_y, distance, brightness, arr_pix, line_gamma);
        }
        --now_s_y;
    }
    now_s_x = now_x; now_s_y = now_y;
    ++now_s_y;
    while (now_s_y < height) {
        double distance = distance_to(len_x, len_y, x1, y1, x2, y2, now_s_x, now_s_y, lwidth);
        if (distance > 0) {
            counter(now_s_x, now_s_y, distance, brightness, arr_pix, line_gamma);
        }
        ++now_s_y;
    }
    if (ch == 1) { --now_x; }
    if (ch == 2) { ++now_x; }

}

const double ABS = 0.001;

void draw(unsigned char* arr_pixels, int &widht, int &height, int &brightness, double &x1, double &y1, double &x2, double &y2, double &line_width, double &line_gamma) {
    int x3, y3; double j; double pnow; int check = 0;
    if (x1 > x2) {
        swap(y1, y2);
        swap(x1, x2);
    }
    if  ((y1 > y2) && (x1 == x2)) {
        swap(x1, x2);
        swap(y1, y2);
    }
    double len_x = x2 - x1, len_y = y2 - y1;
    if (len_x == 0) {
        len_x = ABS;
        x2 += ABS;
    }
    if (len_y == 0) {
        len_y = ABS;
        y2 += ABS;
    }
    double len = sqrt(pow2(len_x, 2) + pow2(len_y, 2));
    double fx = len_x / len, fy = len_y / len;

    double s_line_w = line_width;
    int x1_2 = round(x1), y1_2 = round(y1);

    int now_x = x1_2 - 1, now_y = y1_2;
    while (now_x >= 0) {
        int ch = 1;
        processing(now_x, now_y, len_x, len_y, x1, x2, y1, y2, line_width, arr_pixels, brightness, line_gamma, ch);
    }
    now_x = x1_2; now_y = y1_2;
    while (now_x < widht) {
        int ch = 2;
        processing(now_x, now_y, len_x, len_y, x1, x2, y1, y2, line_width, arr_pixels, brightness, line_gamma, ch);
    }

}

int main(int argc, char * argv[]) {
    if ((argc > 10) || (argc < 9)){
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
    double x1 = atof(argv[5]);
    double y1 = atof(argv[6]);
    double x2 = atof(argv[7]);
    double y2 = atof(argv[8]);
    double line_w, line_g;
    line_w = atof(argv[4]);
    if (argv[9] != NULL) {
        line_g = atof(argv[9]);
    } else {
        line_g = 2.4;
    }


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
    auto* arr_pixels = (unsigned char*)malloc(sizeof(unsigned char) * widht * height);
    if (arr_pixels == NULL){
        cerr << "allocation memory failed \n";
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

    // brightness = 255 * pow2((brightness + 0.0) / 255, 2);
    draw(arr_pixels, widht, height, brightness, x1, y1, x2, y2, line_w, line_g);

    fprintf(output_file, "P%i\n%i %i\n%i\n", a, widht, height, gamma);
    fwrite(arr_pixels, sizeof(unsigned char), widht * height, output_file);

    free(arr_pixels);
    fclose(output_file);
    fclose(input_file);
    return 0;
}