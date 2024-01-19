#ifndef TEXT_H
#define TEXT_H


#include "RUIObject.h"
#include <vector>
#include <GL/glew.h>
#include <string>


namespace TAPP {

    
   
    
    class Text :public RUIObject {
    public:
        
    Text()
        :RUIObject(){
    }
    
        unsigned int Text2DTextureID;
        unsigned int Text2DVertexBufferID;
        unsigned int Text2DUVBufferID;
        unsigned int Text2DShaderID;
        unsigned int Text2DUniformID;
        
        // STore the size of the window
        GLuint SC;
        
        GLuint VertexArrayID;
        
        std::string m_text;
        int m_x, m_y, m_size;
        
        float m_wwidth, m_wheight;
        
        virtual void init();
        virtual void release();
        virtual void hover(double  x, double y, int mods);
        virtual void grab(double x, double y, int b, int mods /* 0 left, 1 right, 2 middle*/);
        virtual void drag(double x, double y, int mods) ;
        
        virtual void resize(double x, double y);
        virtual void render() ;
        virtual void render_pick_select(PickDataback& ) ; // to see if we selected this object
        virtual void render_pick_detail(PickDataback& ); // to see if we selected a certain primitive
    
};


}
#endif // TOGLAPP_H
