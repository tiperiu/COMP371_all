



#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "json.hpp"
#include "simpleppm.h"

#include <sstream>

using namespace std;
using namespace nlohmann;



bool test_parse_geometry(json& j){
    cout<<"Geometry: "<<endl;
    int gc = 0;
    
    // use iterators to read-in array types
    for (auto itr = j["geometry"].begin(); itr!= j["geometry"].end(); itr++){
        
        std::string type;
        if(itr->contains("type")){
           // type = static_cast<std::string>((*itr)["type"]);
            type = (*itr)["type"].get<std::string>();
        } else {
            cout<<"Fatal error: geometry shoudl always contain a type!!!"<<endl;
            return false;
        }
        
        if(type=="sphere"){
            cout<<"Sphere: "<<endl;
            Eigen::Vector3f centre(0,0,0);
            int i = 0;
            for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
                if(i<3){
                    centre[i++] = (*itr2).get<float>();
                } else {
                    cout<<"Warning: Too many entries in centre"<<endl;
                }
            }
            cout<<"Centre: "<<centre<<endl;
            
        }
        ++gc;
    }
    
    cout<<"We have: "<<gc<<" objects!"<<endl;
    return true;
}

bool test_parse_lights(json& j){
    cout<<"Light: "<<endl;
    int lc = 0;
    
    // use iterators to read-in array types
    for (auto itr = j["light"].begin(); itr!= j["light"].end(); itr++){
        
        std::string type;
        if(itr->contains("type")){
          //  type = static_cast<std::string>((*itr)["type"]);
            type = (*itr)["type"].get<std::string>();
        } else {
            cout<<"Fatal error: light shoudl always contain a type!!!"<<endl;
            return false;
        }
        
        if(type=="point"){
            cout<<"Point based light: "<<endl;
            Eigen::Vector3f centre(0,0,0);
            int i = 0;
            for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
                if(i<3){
                    centre[i++] = (*itr2).get<float>();
                } else {
                    cout<<"Warning: Too many entries in centre"<<endl;
                }
            }
            cout<<"Centre: "<<centre<<endl;
            
        }
        ++lc;
    }
    
    cout<<"We have: "<<lc<<" objects!"<<endl;
    
    return true;
}

bool test_parse_output(json& j){
    cout<<"Outputs: "<<endl;
    int lc = 0;
    
    // use iterators to read-in array types
    for (auto itr = j["output"].begin(); itr!= j["output"].end(); itr++){
        
        std::string filename;
        if(itr->contains("filename")){
          //  filename = static_cast<std::string>((*itr)["filename"]);
            filename = (*itr)["filename"].get<std::string>();
        } else {
            cout<<"Fatal error: output shoudl always contain a filename!!!"<<endl;
            return false;
        }
        
        
        int size[2];
        int i = 0;
        for (auto itr2 =(*itr)["size"].begin(); itr2!= (*itr)["size"].end(); itr2++){
            if(i<2){
                size[i++] = (*itr2).get<float>();
            } else {
                cout<<"Warning: Too many entries in size"<<endl;
            }
        }
        
        Eigen::Vector3f lookat(0,0,0), up(0,0,0), centre(0,0,0);
        
         i = 0;
        for (auto itr2 =(*itr)["centre"].begin(); itr2!= (*itr)["centre"].end(); itr2++){
            if(i<3){
                centre[i++] = (*itr2).get<float>();
            } else {
                cout<<"Warning: Too many entries in centre"<<endl;
            }
        }
        
        // Similarly to the centre array you need to read the lookat and up
        //Maybe create a separate functiomn to read arrays - ythey are pretty common
        
        
        // I am retrieving the field of view
        // this is mandatory field here, but if I dont check if it exists,
        // the code will throw an exception which if not caught will end the execution of yoru program
        cout<<"A"<<endl;
        float fov = (*itr)["fov"].get<float>();
        cout<<"B"<<endl;
        
        cout<<"Filename: "<<filename<<endl;
        cout<<"Camera centre: "<<centre<<endl;
        cout<<"FOV: "<<fov<<endl;
        
        ++lc;
    }
    
    cout<<"We have: "<<lc<<" objects!"<<endl;
    return true;
}

int test_json(json& j){
    
    
    
    // 1 - parse geometry
    test_parse_geometry(j);
    
    // 2 - parse lights
    test_parse_lights(j);
    
    // 3 - parse lights
    test_parse_output(j);
    
    return 0;
}

