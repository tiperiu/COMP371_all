#ifndef RUI_OBJECT_H
#define RUI_OBJECT_H


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

namespace TAPP {

    
    class PickDataback {
    public:
        PickDataback(){
            bHit = false;
        }
        
        
        // input
        
        glm::vec3 dir;// ray direction in world coordinate system
        glm::vec3 origin;// ray origin
        
        
        // output
        
        bool bHit;// false if we did not hit anything
        
        glm::vec3 P3d;
        int pid;// primitive ID
        
        int primitive; // 0 - triangle, 1 - point, 3 - line

        double d; // depth along the ray

        // some special cases
        glm::vec2 P2d;// texture space [0, 1]x[0,1]
        

        
        
    };
    
    class RUIObject {
    public:
        
    RUIObject(){
        
    }
    
    
        virtual void init()=0;
        virtual void release()=0;
        virtual void resize(double x, double y)=0;

        
        virtual void hover(double  x, double y, int mods) = 0;
        virtual void grab(double x, double y, int b, int mods /* 0 left, 1 right, 2 middle*/) =0;
        virtual void drag(double x, double y, int mods) = 0;
      
        
        virtual void render() = 0;
        virtual void render_pick_select(PickDataback& ) = 0; // to see if we selected this object
        virtual void render_pick_detail(PickDataback& )= 0; // to see if we selected a certain primitive
        
        
        glm::mat4 m_ProjMatrix, m_ViewMatrix;
        
        glm::vec2 m_wsize;
    
};


}
#endif // TOGLAPP_H
