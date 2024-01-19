#ifndef TOGLWINDOW_H
#define TOGLWINDOW_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "TOGWLayer.h"


namespace TAPP {

class TOGLWindow {
public:
    
    TOGLWindow(){
        m_window = 0;
        
    }
    bool create(int& w, int& h, std::string name);
    
    std::vector<TOGWLayer*> m_layers;
    
    
    GLFWwindow* m_window;
    int m_width, m_height;
    
    
    // These are the size of the windows and not the frambuffers
    int m_w_width, m_w_height;
    
    void hover(double  x, double y, int mods);
    void grab(double x, double y, int b, int mods /* 0 left, 1 right, 2 middle*/);
    void drag(double x, double y, int mods);
    void release(int mods);
    
    void scroll(double  y);
    
    void keypress(int key, int state, int mods);

    void render();
    
    void resize(int w, int h);
    
    void init();
    //
};


}// namespace


#endif // TOGLAPP_H
