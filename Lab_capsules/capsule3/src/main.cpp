#include <iostream>
#include "TOGLApp.h"

#include "Virtual3DLayer.h"

#include "RenderModel.h"

#include "Text.h"

using namespace TAPP;
using namespace std;

std::string global_path;

int main(int argc, char* argv[]){

    if(argc!=2){
        cout<<"Usage: "<<argv[0]<<" objname.obj"<<endl;
        return -1;
    }
    
    int w = 1024;
    int h = 768;
    
    std::string name = "OGL Viewer";
    
    if(!gapp.initialize(w, h, name)){
        cout<<"Unable to initialize!"<<endl;
        return -1;
    }
    
    Virtual3DLayer* view = new Virtual3DLayer(w, h);
    gapp.m_window.m_layers.push_back(view);
    
    RenderModel* sp = new RenderModel(argv[1]);
   
    
    
    
    view->m_objects.push_back(sp);
    
   
/*
    Text* text = new Text();
    text->m_text = "Hello World!";
    text->m_x = 300;
    text->m_y = 300;
    text->m_size = 60;
    view->m_objects.push_back(text);
 */   
 
    gapp.run();
    
    gapp.release();

	return 0;
}

