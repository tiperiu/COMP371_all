



#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "json.hpp"
#include "simpleppm.h"

#include <sstream>

using namespace std;
using namespace nlohmann;


int test_save_ppm(){
    int dimx = 800;
    int dimy = 600;
    
    int w = 100;
    
    std::vector<double> buffer(3*dimx*dimy);
    for(int j=0;j<dimy;++j){
        for(int i=0;i<dimx;++i){
            if(((i+j)/w)%2==0){
                buffer[3*j*dimx+3*i+0]=1;
                buffer[3*j*dimx+3*i+1]=1;
                buffer[3*j*dimx+3*i+2]=0;
            } else {
                buffer[3*j*dimx+3*i+0]=0;
                buffer[3*j*dimx+3*i+1]=1;
                buffer[3*j*dimx+3*i+2]=1;
            }
        }
    }
               
             
    save_ppm("test.ppm", buffer, dimx, dimy);
    
    return 0;
}
