#include "perlin3d.h"

int main(){
    Perlin3D p(512, 512, 1, 128, 8, 0.5);
    std::cout << "ecriture" << std::endl;
    p.writePGMImage("test.pgm");

return 0;
}
