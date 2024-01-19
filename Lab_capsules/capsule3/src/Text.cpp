#include <iostream>

#include "Text.h"

#include "GLUtils.h"

#include "ObjModel.h"
#include "ObjParser.h"

extern std::string global_path;

using namespace std;

namespace TAPP {

   
    void Text::hover(double  x, double y, int mods){
        
        
    }
    void Text::grab(double x, double y, int b, int mods /* 0 left, 1 right, 2 middle*/){
        
        
    }
    
    
    void Text::drag(double x, double y, int mods) {
        
        
    }
    
    void Text::resize(double x, double y){
        
        
        
        m_wwidth = x;
        m_wheight = y;
        
      //  cout<<"TExt debug: W: "<<m_wwidth<<" H: "<<m_wheight<<endl;
    }
    
    void Text::init(){
        
       // cout<<"Do I get here Text!!!"<<endl;
        
        is_error();
        
        glGenVertexArrays(1, &VertexArrayID);
        
        glBindVertexArray(VertexArrayID);
        
        // Initialize texture
        int txt_width, txt_height;
        //Text2DTextureID = loadBMP_custom("Holstein.bmp", txt_width, txt_height);
        std::string s = global_path + "Holstein.DDS";
        Text2DTextureID = loadDDS(s.c_str());
        
        
        // Initialize VBO
        glGenBuffers(1, &Text2DVertexBufferID);
        glGenBuffers(1, &Text2DUVBufferID);
        
        if(is_error(true)){
            cout<<"Error TExt I1"<<endl;
        }
        
        // Initialize Shader
        string vsh1 = global_path + "TextVertexShader.vertexshader";
        string fsh1 = global_path + "TextVertexShader.fragmentshader";
        
        Text2DShaderID = LoadShaders( vsh1.c_str(), fsh1.c_str());
        
        
        if(is_error(true)){
            cout<<"Error TExt I2"<<endl;
        }
        
        // Initialize uniforms' IDs
        Text2DUniformID = glGetUniformLocation( Text2DShaderID, "myTextureSampler" );
        
        // passing the screen size
        SC = glGetUniformLocation(Text2DShaderID, "SC");
        
        if(is_error(true)){
            cout<<"Error TExt I3"<<endl;
        }
    }
    
    
    void Text::release(){
        
     //   cout<<"Do I ger hre! 34"<<endl;
        
        /*
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &uvbuffer);
        glDeleteBuffers(1, &normalbuffer);
        glDeleteProgram(programID);
        glDeleteTextures(1, &Texture);
        glDeleteVertexArrays(1, &VertexArrayID);
         */
    }
    
    void Text::render() {
        
       // cout<<"Do I rener text!"<<endl;
        
        is_error();
        
         glBindVertexArray(VertexArrayID);
        
       // cout<<"Text: "<<m_text<<endl;
        
        unsigned int length = m_text.size();
        
        // Fill buffers
        std::vector<glm::vec2> vertices;
        std::vector<glm::vec2> UVs;
        for ( unsigned int i=0 ; i<length ; i++ ){
            
            glm::vec2 vertex_up_left    = glm::vec2( m_x+i* m_size     , m_y+ m_size );
            glm::vec2 vertex_up_right   = glm::vec2( m_x+i* m_size+ m_size, m_y+ m_size );
            glm::vec2 vertex_down_right = glm::vec2( m_x+i* m_size+ m_size, m_y      );
            glm::vec2 vertex_down_left  = glm::vec2( m_x+i* m_size     , m_y      );
            
            vertices.push_back(vertex_up_left   );
            vertices.push_back(vertex_down_left );
            vertices.push_back(vertex_up_right  );
            
            vertices.push_back(vertex_down_right);
            vertices.push_back(vertex_up_right);
            vertices.push_back(vertex_down_left);
            
            char character = m_text[i];
            float uv_x = (character%16)/16.0f;
            float uv_y = (character/16)/16.0f;
            
            glm::vec2 uv_up_left    = glm::vec2( uv_x           , uv_y );
            glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, uv_y );
            glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
            glm::vec2 uv_down_left  = glm::vec2( uv_x           , (uv_y + 1.0f/16.0f) );
            UVs.push_back(uv_up_left   );
            UVs.push_back(uv_down_left );
            UVs.push_back(uv_up_right  );
            
            UVs.push_back(uv_down_right);
            UVs.push_back(uv_up_right);
            UVs.push_back(uv_down_left);
        }
        
        
        glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
        glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
        
        if(is_error(true)){
            cout<<"Error TExt 0"<<endl;
        }
        
        
        // Bind shader
        glUseProgram(Text2DShaderID);
        
        
        if(is_error(true)){
            cout<<"Error TExt 0.1"<<endl;
        }

        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(Text2DUniformID, 0);
        
        // the screen size
      //  cout<<"Uniforms: "<<m_wwidth<<" "<<m_wheight<<endl;
        glUniform2f(SC, m_wwidth, m_wheight);
        
        if(is_error(true)){
            cout<<"Errro TExt 0.2"<<endl;
        }
        
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        
        if(is_error(true)){
            cout<<"Error TExt 0.3"<<endl;
        }
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
        
        
        if(is_error(true)){
            cout<<"Error TExt 0.4"<<endl;
        }
        
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        
        if(is_error(true)){
            cout<<"Error TExt 1"<<endl;
        }
        
        // Draw call
        glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
        
        if(is_error(true)){
            cout<<"Error TExt 2"<<endl;
        }

        
        glDisable(GL_BLEND);
        
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        
        if(is_error(true)){
            cout<<"Error TExt 3"<<endl;
        }

        
        
    }
    void Text::render_pick_select(PickDataback& ) { // to see if we selected this object
        
        
        
        
    }
    
    
    
    void Text::render_pick_detail(PickDataback& pd){ // to see if we selected a certain primitive
        
        

    }// end of picking function
    
}
