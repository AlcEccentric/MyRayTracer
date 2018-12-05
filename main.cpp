#include <fstream>
#include <iostream>
#include "vec3.h"
int main(){
    
    std::fstream File;
    File.open("hello.ppm",std::ios::out);
    if(!File){
        File.close();
        std::cout<< "fail to open file\n";
    } 

    
    int nx = 2000, ny = 1000;
    File<< "P3\n" << nx << " " << ny << "\n" << "255\n";
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            vec3 v(float(i)/float(nx), float(j)/float(ny), 0.2);
            int ir = int(255.99 * v.r());
            int ig = int(255.99 * v.g());
            int ib = int(255.99 * v.b());
            File<< ir << " " << ig << " " << ib << "\n";
        }
    }
    

    File.close();
    return 0;
}    