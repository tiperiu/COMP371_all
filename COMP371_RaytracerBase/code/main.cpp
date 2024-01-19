

#ifdef COURSE_SOLUTION
#include "src/RayTracer.h"
#endif

#ifdef STUDENT_SOLUTION
#include "src/RayTracer.h"
#endif


#include <iostream>
#include <string>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "external/json.hpp"
#include "external/simpleppm.h"


using namespace std;

int test_eigen();
int test_save_ppm();
int test_json(nlohmann::json& j);
    
int main(int argc, char* argv[])
{
    if(argc!=2){
        cout<<"Invalid number of arguments"<<endl;
        cout<<"Usage: ./raytracer [scene] "<<endl;
        cout<<"Run sanity checks"<<endl;
        
        test_eigen();
        test_save_ppm();
        
    } else {
        
        cout<<"Scene: "<<argv[1]<<endl;
        
        std::ifstream t(argv[1]);
        if(!t){
            cout<<"File "<<argv[1]<<" does not exist!"<<endl;
            return -1;
        }
        
        std::stringstream buffer;
        buffer << t.rdbuf();
        
        nlohmann::json j = nlohmann::json::parse(buffer.str());
        cout<<"Parsed successfuly"<<endl;
        
#ifdef COURSE_SOLUTION
        srand(234);
        cout<<"Running course solution"<<endl;
        time_t tstart, tend;
        tstart = time(0);
        RT371::RayTracer<RT371::Kernelf> rt(j);
        cout<<"Running!"<<endl;
        rt.run();
        tend = time(0);
        cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;
#else
        
#ifdef STUDENT_SOLUTION
        cout<<"Running student solution"<<endl;

        time_t tstart, tend;
        tstart = time(0);
        RayTracer rt(j);
        rt.run();
        tend = time(0);
        cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;
#else
        // GIven code - a bunch of test functions to showcase the funcitonality
        test_eigen();
        test_save_ppm();
        
        if(test_json(j)==0){
            
        } else {
            cout<<"Could not load file!"<<endl;
        }
#endif
        
        
#endif
    }
  
    
    
    return 0;
}

