#include "simpleppm.h"

/*
 This code was adapted from here:
 https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file#C.2B.2B
 */

using namespace std;

int save_ppm(std::string file_name, const std::vector<double>& buffer, int dimx, int dimy) {
   
    ofstream ofs(file_name, ios_base::out | ios_base::binary);
    ofs << "P6" << endl << dimx << ' ' << dimy << endl << "255" << endl;
 
    for (unsigned int j = 0; j < dimy; ++j)
        for (unsigned int i = 0; i < dimx; ++i)
            ofs << (char) (255.0 * buffer[3*j*dimx+3*i+0]) <<  (char) (255.0 * buffer[3*j*dimx+3*i+1]) << (char) (255.0 * buffer[3*j*dimx+3*i+2]);
 
    ofs.close();
 
    return 0;
}



