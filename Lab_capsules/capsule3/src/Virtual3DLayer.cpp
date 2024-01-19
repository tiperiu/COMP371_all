#include "Virtual3DLayer.h"

// Include GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "RenderModel.h"

#include "GLUtils.h"


using namespace std;


namespace TAPP {

   
    
    
    void Virtual3DLayer::hover(double  x, double y, int mods){
        
        for(int i=0;i<m_objects.size();++i){
            m_objects[i]->hover(x, y, mods);
        }
        
    }
    
    
    void mat4toarray(glm::mat4 m4, double arr[])
    {
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                arr[4*j+k]=m4[j][k];
            }
        }
        
    }
    
    void Virtual3DLayer::compute_ray(double x, double y, glm::vec3& origin, glm::vec3& direction){
        glm::mat4 m = m_camera.gview;
        glm::mat4 p = m_camera.proj;
        
        glm::vec4 Z(0,0,0,1);
        Z = inverse(m_camera.gview) * Z;
        origin[0] = Z[0];
        origin[1] = Z[1];
        origin[2] = Z[2];
        
        GLdouble model[16] =
        { 0.0 }, projection[16] =
        { 0.0 };
        
        mat4toarray(m, model);
        mat4toarray(p, projection);
        
        direction = m_camera.Screen2World(x, m_height - y, model, projection);
        direction = direction - origin;
        
        direction = glm::normalize(direction);

        
    }

    
    void Virtual3DLayer::grab(double x, double y, int b, int mods /* 0 left, 1 right, 2 middle*/){
        
        cx = x;
        cy = y;
        
        if(action_state==0 || (mods & GLFW_MOD_CONTROL)){
            
            for(int i=0;i<m_objects.size();++i){
                m_objects[i]->grab(x, y, b, mods);
            }
            
        } else if(action_state==1){
            
            PickDataback pd;
            
            // compute the ray
            
            
            compute_ray(x, y, pd.origin, pd.dir);
            
            
            
            for(int i=0;i<m_objects.size();++i){
                m_objects[i]->render_pick_detail(pd);
            }
            
            tmp = pd;
        }
        


        
    }
    
    void Virtual3DLayer::keypress(int key, int state, int mods){
        // sets the state
        if (key == GLFW_KEY_M && state == GLFW_PRESS){
            action_state = 1;
        }
        
        if (key == GLFW_KEY_V && state == GLFW_PRESS){
            action_state = 0;
        }
        
        // delete the last measurement objects
        if (key == GLFW_KEY_C && state == GLFW_PRESS){
            if(m_objects.size()>1)
                m_objects.pop_back();
        }

        if (key == GLFW_KEY_A && state == GLFW_PRESS){
            while(m_objects.size()>1)
                m_objects.pop_back();
        }
        
    }
    
    
    void Virtual3DLayer::scroll(double y){
        m_camera.Scroll(y);
    }
    
    void Virtual3DLayer::drag(double x, double y, int mods){
        
        glm::vec2 oldp(cx, cy);
        glm::vec2 newp(x, y);
        
        
        if(action_state==0 || (mods & GLFW_MOD_CONTROL)){
            
            
            
            m_camera.Track(oldp, newp);
            
            for(int i=0;i<m_objects.size();++i){
                m_objects[i]->drag(x, y, mods);
            }
            
            
           
            
        } else if(action_state==1){
            // picking business
            
            
            
        }

        
        
       
        cx = x;
        cy = y;
    }
    
    
    void Virtual3DLayer::release(int mods){
        
        if(action_state==0 || (mods & GLFW_MOD_CONTROL)){
            
            for(int i=0;i<m_objects.size();++i){
                m_objects[i]->release();
            }
            
            
        } else if(action_state==1){
            
            // picking business
            PickDataback pd;
            
            // compute the ray
            
            
           compute_ray(cx, cy, pd.origin, pd.dir);
            
            for(int i=0;i<m_objects.size();++i){
                m_objects[i]->render_pick_detail(pd);
            }
            
            RenderModel* vp = dynamic_cast<RenderModel*>(m_objects[0]);
            
        }
    
    }
    
    
    void Virtual3DLayer::init(int w, int h){
        resize(w, h);
        
        cout<<"W: "<<w<<" H: "<<h<<endl;
        glViewport(0,0,w, h);
        
        for(int i=0;i<m_objects.size();++i){
            m_objects[i]->init();
        }
        
    }

    
    void Virtual3DLayer::resize(int w, int h){
        
        m_width = w;
        m_height = h;
        
        m_camera.Resize(w, h);
        
       // cout<<"W: "<<w<<" H: "<<h<<endl;
        glViewport(0,0,w, h);
        
        for(int i=0;i<m_objects.size();++i){
            m_objects[i]->resize(w, h);
        }
        
    }
    
    void Virtual3DLayer::render(){
    
        is_error();
        
        glViewport(0,0,m_width, m_height);
        
        if(is_error(true)){
            cout<<"Err VP"<<endl;
        }
        
        //cout<<"W: "<<m_width<<" H: "<<m_height<<endl;
        
        m_camera.ApplyView();
        m_camera.setperspective(m_width, m_height, 0, 0, 0, 0, 0);
    
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        
        
        for(int i=0;i<m_objects.size();++i){
            
            m_objects[i]->m_ProjMatrix = m_camera.proj;
            m_objects[i]->m_ViewMatrix = m_camera.gview;
            
            m_objects[i]->m_wsize[0] = m_camera.tball.scrWidth;
            m_objects[i]->m_wsize[1] = m_camera.tball.scrHeight;
            
            m_objects[i]->render();
        }

    }
    
    
    
    
  
}
