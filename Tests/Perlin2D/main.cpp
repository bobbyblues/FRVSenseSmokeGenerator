#include "perlin2d.h"

#include <iostream>

int main(){
    Perlin2D p(512, 512, 128, 8, 0.5);
    std::cout << "ecriture" << std::endl;
    p.writePGMImage("test.pgm");

return 0;
}
