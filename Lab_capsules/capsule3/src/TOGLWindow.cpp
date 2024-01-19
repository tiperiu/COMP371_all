#include <string>
#include "TOGLWindow.h"

// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

namespace TAPP {

    bool TOGLWindow::create(int& w, int& h, std::string name){
     
        m_width = w;
        m_height = h;
        
        m_w_width = w;
        m_w_height = h;
  
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        m_window = glfwCreateWindow(m_width, m_height, name.c_str(), NULL, NULL);
        if (m_window == NULL) {
            fprintf(stderr, "Failed to open GLFW window. Please check if your video driver is installed correctly and if it is OpenGL 3.3+.\n");
            return false;
        }
        
        glfwGetFramebufferSize(m_window, &w, &h);
        m_width = w;
        m_height = h;
        
        cout<<"Window size: "<<m_w_width<<" "<<m_w_height<<endl;
        cout<<"Buffer size: "<<m_width<<" "<<m_height<<endl;

        
        glfwMakeContextCurrent(m_window);
        glfwPollEvents();
        glfwSetCursorPos(m_window, m_width / 2, m_height / 2);
        
        return true;
    }
    

    void TOGLWindow::keypress(int key, int state, int mods){
        
        
        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->keypress(key, state, mods);
        }
    }
    
    void TOGLWindow::scroll(double  y){
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->scroll(y);
        }
    }
    
    void TOGLWindow::hover(double x, double y, int mods){
        
        x = (x / m_w_width * m_width);
        y = (y / m_w_height * m_height);

        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->hover(x,y , mods);
        }
        
    }
    
    
    void TOGLWindow::grab(double x, double y, int b, int mods){
        
        x = (x / m_w_width * m_width);
        y = (y / m_w_height * m_height);
        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->grab(x,y ,b,  mods);
        }

    }
    
    
    void TOGLWindow::drag(double x, double y, int mods){
        
        x = (x / m_w_width * m_width);
        y = (y / m_w_height * m_height);
      
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->drag(x,y , mods);
        }

    }
    
    
    void TOGLWindow::release(int mods){
        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->release(mods);
        }

    }
    
    void TOGLWindow::init(){
        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->init(m_width, m_height);
        }
        
    }
    
    void TOGLWindow::resize(int w, int h){
        
        int tw, th;
        glfwGetFramebufferSize(m_window, &tw, &th);
        
//        cout<<"Test: W size: "<<w<<" "<<h<<endl;
  //      cout<<"Test: B size: "<<tw<<" "<<th<<endl;
        
        m_w_width = w;
        m_w_height = h;
        
        
        m_width = tw;
        m_height = th;
        
        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->resize(w, h);
        }

    }
    
    
    void TOGLWindow::render(){
        
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0,0, m_width, m_height);
        

        for(int i=0;i<m_layers.size();++i){
            m_layers[i]->render();
        }
 
        
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    
};
