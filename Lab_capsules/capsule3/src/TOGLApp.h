#ifndef TOGLAPP_H
#define TOGLAPP_H


// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "TOGLWindow.h"

namespace TAPP {

    
    
class TOGLApp {
public:

    
    bool initialize_libraries();

    // this calls also initialize_libraries
    bool initialize(int& w, int& h,  std::string name);


    void run();
    
    bool release();
    
    
    // for now a single window
    TOGLWindow m_window;
    
    
    static void mouse_button_event(GLFWwindow * window, int button, int action, int mods);
    static void keyboard_button_event(GLFWwindow *, int inputKey, int b, int action, int d);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void resize_callback(GLFWwindow* window, int wx, int wyy);
    
    static bool gbutton; /* true if some button pressed */
    static int gmods;
    static double gx;
    static double gy;

    
};


extern TOGLApp gapp;


}// namespace

#endif // TOGLAPP_H
