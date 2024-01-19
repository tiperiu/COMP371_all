#include <iostream>
#include "TOGLApp.h"



using namespace std;

namespace TAPP {

    TOGLApp gapp;
    
    bool TOGLApp::initialize(int& w, int& h,  std::string name){
        
        
        if (!glfwInit())
        {
            fprintf(stderr, "Failed to initialize GLFW\n");
            getchar();
            return false;
        }

        
        m_window.create(w, h, name);
        
        
        glfwSetMouseButtonCallback(m_window.m_window, TOGLApp::mouse_button_event);
        glfwSetKeyCallback(m_window.m_window, TOGLApp::keyboard_button_event);
        glfwSetCursorPosCallback(m_window.m_window, TOGLApp::cursor_position_callback);
        glfwSetScrollCallback(m_window.m_window, TOGLApp::scroll_callback);
        glfwSetWindowSizeCallback(m_window.m_window, TOGLApp::resize_callback);
       
        
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
            cout<<"Step B1.2"<<endl;
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            return false;
        }
        
        return true;
        
    }
    
   
    void TOGLApp::run(){
        
        m_window.init();
        
        do {
            
            
            m_window.render();
            
            glfwPollEvents();
            
        } // Check if the ESC key was pressed or the window was closed
        while (glfwGetKey(m_window.m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
               glfwWindowShouldClose(m_window.m_window) == 0);
        
        
    }
    
    
    bool TOGLApp::release(){
     
        glfwTerminate();
        
        return true;
    }
    
    
    // callbacks
    
     bool TOGLApp::gbutton=false;
     int TOGLApp::gmods=0; /* same as glfw */
     double TOGLApp::gx = 0;
     double TOGLApp::gy = 0;
    
    
     void TOGLApp::mouse_button_event(GLFWwindow * window, int button, int action, int mods){
        
        gmods = mods;
        
        if(action==GLFW_PRESS){
            
            gbutton = true;
            
            if(button==GLFW_MOUSE_BUTTON_LEFT){
                gapp.m_window.grab(gx, gy, 0, mods);
            } else if(button==GLFW_MOUSE_BUTTON_RIGHT){
                gapp.m_window.grab(gx, gy, 1, mods);
            } else if(button==GLFW_MOUSE_BUTTON_MIDDLE){
                gapp.m_window.grab(gx, gy, 2, mods);
            } else {
                cout<<"What button? "<<button<<endl;
            }
        } else if(action==GLFW_RELEASE){
            
            
            gbutton = false;
            
            gapp.m_window.release(mods);
            
            if(button==GLFW_MOUSE_BUTTON_LEFT){
                
            } else if(button==GLFW_MOUSE_BUTTON_RIGHT){
                
            } else if(button==GLFW_MOUSE_BUTTON_MIDDLE){
                
            } else {
                cout<<"What button? "<<button<<endl;
            }

        } else {
            cout<<"What action? "<<action<<endl;
        }
        
     }
    
    
     void TOGLApp::keyboard_button_event(GLFWwindow *, int inputKey, int b, int action, int d){
       
         gapp.m_window.keypress(inputKey, action, d);
        
    }
    
    
     void TOGLApp::cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
        gx = xpos;
        gy = ypos;
        
        // cout<<"Debug MP: "<<gx<<" "<<gy<<endl;
         
        // any button depressed
        if(gbutton){
            gapp.m_window.drag(gx, gy, gmods);
        } else {
            gapp.m_window.hover(gx, gy, gmods);
        }
        
     }
    
    
    void TOGLApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
        gapp.m_window.scroll(yoffset);
        
    }
    
    
    void TOGLApp::resize_callback(GLFWwindow* window, int wx, int wy){
        gapp.m_window.resize(wx, wy);
    }
    
};  // namespace

