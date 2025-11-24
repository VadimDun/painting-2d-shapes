#include "shader.h"
#include <iostream>
#include <vector>
#include <cmath>

Assignment currentAssignment = ASSIGNMENT_2;

GLuint ProgramQuad, ProgramFan, ProgramPentagon;
GLuint Attrib_vertex_quad, Attrib_vertex_fan, Attrib_vertex_pentagon;
GLuint VBO;

const char* VertexShaderSource = R"(
#version 330 core
in vec2 coord;
void main() {
    gl_Position = vec4(coord, 0.0, 1.0);
}
)";

const char* FragShaderSourceQuad = R"(
#version 330 core
out vec4 color;
void main() {
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";

const char* FragShaderSourceFan = R"(
#version 330 core
out vec4 color;
void main() {
    color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
)";

const char* FragShaderSourcePentagon = R"(
#version 330 core
out vec4 color;
void main() {
    color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}
)";

void ShaderLog(unsigned int shader) {
int infologLen = 0;
glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
if (infologLen > 1) {
    int charsWritten = 0;
    std::vector<char> infoLog(infologLen);
    glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
    std::cout << "InfoLog: " << infoLog.data() << std::endl;
}
}

void InitShader() {
// Quad
GLuint vShaderQuad = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vShaderQuad, 1, &VertexShaderSource, NULL);
glCompileShader(vShaderQuad);

GLuint fShaderQuad = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fShaderQuad, 1, &FragShaderSourceQuad, NULL);
glCompileShader(fShaderQuad);

ProgramQuad = glCreateProgram();
glAttachShader(ProgramQuad, vShaderQuad);
glAttachShader(ProgramQuad, fShaderQuad);
glLinkProgram(ProgramQuad);

Attrib_vertex_quad = glGetAttribLocation(ProgramQuad, "coord");
if (Attrib_vertex_quad == -1) {
    std::cout << "Could not bind attrib " << Attrib_vertex_quad << std::endl;
    return;
}

// Fan
GLuint vShaderFan = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vShaderFan, 1, &VertexShaderSource, NULL);
glCompileShader(vShaderFan);

GLuint fShaderFan = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fShaderFan, 1, &FragShaderSourceFan, NULL);
glCompileShader(fShaderFan);

ProgramFan = glCreateProgram();
glAttachShader(ProgramFan, vShaderFan);
glAttachShader(ProgramFan, fShaderFan);
glLinkProgram(ProgramFan);

Attrib_vertex_fan = glGetAttribLocation(ProgramFan, "coord");
if (Attrib_vertex_fan == -1) {
    std::cout << "Could not bind attrib " << Attrib_vertex_fan << std::endl;
    return;
}

// Pentagon
GLuint vShaderPentagon = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vShaderPentagon, 1, &VertexShaderSource, NULL);
glCompileShader(vShaderPentagon);

GLuint fShaderPentagon = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fShaderPentagon, 1, &FragShaderSourcePentagon, NULL);
glCompileShader(fShaderPentagon);

ProgramPentagon = glCreateProgram();
glAttachShader(ProgramPentagon, vShaderPentagon);
glAttachShader(ProgramPentagon, fShaderPentagon);
glLinkProgram(ProgramPentagon);

Attrib_vertex_pentagon = glGetAttribLocation(ProgramPentagon, "coord");
if (Attrib_vertex_pentagon == -1) {
    std::cout << "Could not bind attrib " << Attrib_vertex_pentagon << std::endl;
    return;
}
}

// ������� ������� ����������� �������������
std::vector<Vertex> createPentagon(float centerX, float centerY, float radius) {
std::vector<Vertex> pentagon;
for (int i = 0; i < 5; ++i) {
    float angle = 2.0f * 3.14159f * i / 5.0f;

    Vertex v;
    v.x = centerX + radius * cos(angle);
    v.y = centerY + radius * sin(angle);
    pentagon.push_back(v);
}
return pentagon;
}

void InitVBO() {
std::vector<Vertex> quad = {
    { -0.8f, -0.8f },  // LD
    { -0.3f, -0.8f },  // RD
    { -0.3f, -0.3f },  // RU

    { -0.8f, -0.8f },  // LD
    { -0.3f, -0.3f },  // RU
    { -0.8f, -0.3f }   // LU
};

std::vector<Vertex> fan = {
    { 0.0f, 0.0f },    // center

    { 0.47f, 0.17f },
    { 0.41f, 0.32f },
    { 0.32f, 0.44f },
    { 0.21f, 0.53f },
    { 0.07f, 0.57f },
    { -0.07f, 0.57f },
    { -0.21f, 0.53f },
    { -0.32f, 0.44f },
    { -0.41f, 0.32f },
    { -0.47f, 0.17f },
};

std::vector<Vertex> pentagonVertices;
Vertex center = { 0.5f, -0.5f };
std::vector<Vertex> pentagon = createPentagon(center.x, center.y, 0.3f);

// 5 fans from center
for (int i = 0; i < 5; ++i) {
    pentagonVertices.push_back(center);
    pentagonVertices.push_back(pentagon[i]);
    pentagonVertices.push_back(pentagon[(i + 1) % 5]);
}

std::vector<Vertex> allVertices;
allVertices.insert(allVertices.end(), quad.begin(), quad.end());
allVertices.insert(allVertices.end(), fan.begin(), fan.end());
allVertices.insert(allVertices.end(), pentagonVertices.begin(), pentagonVertices.end());

glGenBuffers(1, &VBO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex), allVertices.data(), GL_STATIC_DRAW);
glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Init() {
InitShader();
InitVBO();
}

void Draw() {
glBindBuffer(GL_ARRAY_BUFFER, VBO);

switch (currentAssignment) {
    case ASSIGNMENT_2: {
        // Quad
        glUseProgram(ProgramQuad);
        glEnableVertexAttribArray(Attrib_vertex_quad);
        glVertexAttribPointer(Attrib_vertex_quad, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(Attrib_vertex_quad);

        // Fan
        glUseProgram(ProgramFan);
        glEnableVertexAttribArray(Attrib_vertex_fan);
        glVertexAttribPointer(Attrib_vertex_fan, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLE_FAN, 6, 11);
        glDisableVertexAttribArray(Attrib_vertex_fan);

        // Pentagon
        glUseProgram(ProgramPentagon);
        glEnableVertexAttribArray(Attrib_vertex_pentagon);
        glVertexAttribPointer(Attrib_vertex_pentagon, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 17, 15);
        glDisableVertexAttribArray(Attrib_vertex_pentagon);
        break;
    }
    case ASSIGNMENT_3: {
        // TODO TASK3
        glUseProgram(ProgramQuad);
        glEnableVertexAttribArray(Attrib_vertex_quad);
        glVertexAttribPointer(Attrib_vertex_quad, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLE_FAN, 6, 11);
        glDrawArrays(GL_TRIANGLES, 17, 15);
        glDisableVertexAttribArray(Attrib_vertex_quad);
        break;
    }
    case ASSIGNMENT_4: {
        // TODO TASK4
        glUseProgram(ProgramQuad);
        glEnableVertexAttribArray(Attrib_vertex_quad);
        glVertexAttribPointer(Attrib_vertex_quad, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawArrays(GL_TRIANGLE_FAN, 6, 11);
        glDrawArrays(GL_TRIANGLES, 17, 15);
        glDisableVertexAttribArray(Attrib_vertex_quad);
        break;
    }
}
glBindBuffer(GL_ARRAY_BUFFER, 0);
glUseProgram(0);
}

void Release() {
glBindBuffer(GL_ARRAY_BUFFER, 0);
glDeleteBuffers(1, &VBO);
glDeleteProgram(ProgramQuad);
glDeleteProgram(ProgramFan);
glDeleteProgram(ProgramPentagon);
}