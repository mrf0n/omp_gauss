#include <iostream>
#include <omp.h>
#include <cmath>
#include <ctime>
#include "C://Users//Ваня//source//repos//parallel//parallel//FreeImage.h"
#pragma comment (lib, "C://Users//Ваня//source//repos//parallel//parallel//FreeImage.lib")
using namespace std;

double gaus(int x, int y, double sigma)
{
    return exp(-(pow(x, 2) + pow(y, 2)) / (pow(sigma, 2) * 2)) / (acos(-1.0) * 2 * pow(sigma,2));
}

struct pix
{
    int r=0;
    int g=0;
    int b=0;
};

double** made(double sigma, int& size, int n)
{
    size = ceil(n * sigma);
    if (size % 2 == 0)
        size += 1;
    double** pic = new double* [size];
    for (int i = 0; i < size; i++)
        pic[i] = new double[size];
    double temp = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            pic[i][j] = gaus((-size / 2 + j), (size / 2 - i), sigma);
            temp += pic[i][j];
        }
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            pic[i][j] /= temp;
    return pic;
}


int main()
{
    int sigma;
    double sigma2;
    cout << "sgm= ";
    cin >> sigma2;

    double** cr = made(sigma2, sigma, 5);

    //вывод ядра

    const char* input = "C:/Users/Ваня/Desktop/111.jpeg";
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(input);
    FIBITMAP* map = FreeImage_Load(format, input);
    if (map)
    {
        BYTE* data = FreeImage_GetBits(map);
        int h = FreeImage_GetHeight(map);
        int w = FreeImage_GetWidth(map);

        const char* output = "C:/Users/Ваня/Pictures/Screenshots/out.jpeg";
        //вывод длинны высоты
        pix** curently = new pix * [h];
        for (int i = 0; i < h; i++)
            curently[i] = new pix[w];
        int a = 0;
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                curently[i][j].b = data[a++];
                curently[i][j].r = data[a++];
                curently[i][j].g = data[a++];
            }
        }
        BYTE* newdate = new BYTE[h * w * 3];
        int cur = 0;
        unsigned int time = clock();

        #pragma omp parallel for
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                double r = 0;
                double g = 0;
                double b = 0;

                for (int q = 0; q < sigma; q++)
                {
                    for (w = 0; w < sigma; w++)
                    {
                        int sw = w - sigma / 2;
                        int sh = q - sigma / 2;
                        if (i + sh >= 0 && j + sw >= 0 && i + sh < h && j + w < w)
                        {
                            r = curently[i + sh][j + sw].r * cr[q][w];
                            g = curently[i + sh][j + sw].g * cr[q][w];
                            b = curently[i + sh][j + sw].b * cr[q][w];
                        }
                        else
                        {
                            r += 0;
                            g += 0;
                            b += 0;
                        }
                    }
                    newdate[i * 3 * w + (j + 1) * 3 - 3] = round(r);
                    newdate[i * 3 * w + (j + 1) * 3 - 2] = round(g);
                    newdate[i * 3 * w + (j + 1) * 3 - 1] = round(b);
                }
            }
        }



    }

}


