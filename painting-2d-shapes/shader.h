#pragma once
#include <GL/glew.h>
#include <vector>

enum Assignment {
    ASSIGNMENT_2 = 2,
    ASSIGNMENT_3 = 3,
    ASSIGNMENT_4 = 4
};

extern Assignment currentAssignment;

// Shader programs
extern GLuint ProgramQuad, ProgramFan, ProgramPentagon;
extern GLuint Attrib_vertex_quad, Attrib_vertex_fan, Attrib_vertex_pentagon;

// VBO
extern GLuint VBO;

struct Vertex {
    GLfloat x;
    GLfloat y;
};

extern const char* VertexShaderSource;
extern const char* FragShaderSourceQuad;
extern const char* FragShaderSourceFan;
extern const char* FragShaderSourcePentagon;

void ShaderLog(unsigned int shader);
void InitShader();

// VBO (может быть в другой файл?)
std::vector<Vertex> createPentagon(float centerX, float centerY, float radius);
void InitVBO();
void Init();
void Draw();
void Release();