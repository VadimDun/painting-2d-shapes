#pragma once
#include <GL/glew.h>
#include <vector>

enum Assignment {
    ASSIGNMENT_2 = 2,
    ASSIGNMENT_3 = 3,
    ASSIGNMENT_4 = 4  
};

extern Assignment currentAssignment;

extern GLuint ProgramQuad, ProgramFan, ProgramPentagon;
extern GLuint ProgramUniversal; 
extern GLuint ProgramGradient; 

extern GLuint Attrib_vertex_quad, Attrib_vertex_fan, Attrib_vertex_pentagon;
extern GLuint Attrib_vertex_universal;  
extern GLuint Attrib_vertex_gradient;   
extern GLuint Attrib_color_gradient;    

extern GLuint Uniform_color;

extern GLuint VBO;
extern GLuint VBO_Gradient;  

extern const char* VertexShaderSource;
extern const char* FragShaderSourceQuad;
extern const char* FragShaderSourceFan;
extern const char* FragShaderSourcePentagon;
extern const char* FragShaderSourceUniversal;

extern const char* VertexShaderSourceGradient;
extern const char* FragShaderSourceGradient;

struct Vertex {
    GLfloat x;
    GLfloat y;
};

struct ColorVertex {
    GLfloat x;
    GLfloat y;
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};


void ShaderLog(unsigned int shader);
void InitShader();
std::vector<ColorVertex> createGradientVertices();
void InitVBO();
void Init();
void Draw();
void Release();

