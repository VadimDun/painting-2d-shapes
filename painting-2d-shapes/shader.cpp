#include "shader.h"
#include <iostream>
#include <vector>
#include <cmath>

Assignment currentAssignment = ASSIGNMENT_2;

GLuint ProgramQuad, ProgramFan, ProgramPentagon;
GLuint ProgramUniversal; 
GLuint ProgramGradient;  

GLuint Attrib_vertex_quad, Attrib_vertex_fan, Attrib_vertex_pentagon;
GLuint Attrib_vertex_universal;  
GLuint Attrib_vertex_gradient;   
GLuint Attrib_color_gradient;

GLuint Uniform_color;

GLuint VBO;
GLuint VBO_Gradient; 

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

const char* FragShaderSourceUniversal = R"(
#version 330 core
uniform vec4 objColor;  
out vec4 color;
void main() {
    color = objColor;  
}
)";

const char* VertexShaderSourceGradient = R"(
#version 330 core
in vec2 position;
in vec4 color;
out vec4 vertexColor;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertexColor = color;
}
)";

const char* FragShaderSourceGradient = R"(
#version 330 core
in vec4 vertexColor;
out vec4 color;
void main() {
    color = vertexColor;
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
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vShader);

    // Quad
    GLuint fShaderQuad = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderQuad, 1, &FragShaderSourceQuad, NULL);
    glCompileShader(fShaderQuad);

    ProgramQuad = glCreateProgram();
    glAttachShader(ProgramQuad, vShader);
    glAttachShader(ProgramQuad, fShaderQuad);
    glLinkProgram(ProgramQuad);

    Attrib_vertex_quad = glGetAttribLocation(ProgramQuad, "coord");
    if (Attrib_vertex_quad == -1) {
        std::cout << "Could not bind attrib " << Attrib_vertex_quad << std::endl;
        return;
    }

    // Fan
    GLuint fShaderFan = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderFan, 1, &FragShaderSourceFan, NULL);
    glCompileShader(fShaderFan);

    ProgramFan = glCreateProgram();
    glAttachShader(ProgramFan, vShader);
    glAttachShader(ProgramFan, fShaderFan);
    glLinkProgram(ProgramFan);

    Attrib_vertex_fan = glGetAttribLocation(ProgramFan, "coord");
    if (Attrib_vertex_fan == -1) {
        std::cout << "Could not bind attrib " << Attrib_vertex_fan << std::endl;
        return;
    }

    // Pentagon
    GLuint fShaderPentagon = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderPentagon, 1, &FragShaderSourcePentagon, NULL);
    glCompileShader(fShaderPentagon);

    ProgramPentagon = glCreateProgram();
    glAttachShader(ProgramPentagon, vShader);
    glAttachShader(ProgramPentagon, fShaderPentagon);
    glLinkProgram(ProgramPentagon);

    Attrib_vertex_pentagon = glGetAttribLocation(ProgramPentagon, "coord");
    if (Attrib_vertex_pentagon == -1) {
        std::cout << "Could not bind attrib " << Attrib_vertex_pentagon << std::endl;
        return;
    }

    // Universal program
    GLuint fShaderUniversal = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderUniversal, 1, &FragShaderSourceUniversal, NULL);
    glCompileShader(fShaderUniversal);

    ProgramUniversal = glCreateProgram();
    glAttachShader(ProgramUniversal, vShader);
    glAttachShader(ProgramUniversal, fShaderUniversal);
    glLinkProgram(ProgramUniversal);

    Attrib_vertex_universal = glGetAttribLocation(ProgramUniversal, "coord");
    if (Attrib_vertex_universal == -1) {
        std::cout << "Could not bind attrib coord in universal program" << std::endl;
        return;
    }

    Uniform_color = glGetUniformLocation(ProgramUniversal, "objColor");
    if (Uniform_color == -1) {
        std::cout << "Could not bind uniform objColor" << std::endl;
        return;
    }

    GLuint vShaderGradient = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShaderGradient, 1, &VertexShaderSourceGradient, NULL);
    glCompileShader(vShaderGradient);
    ShaderLog(vShaderGradient);

    GLuint fShaderGradient = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShaderGradient, 1, &FragShaderSourceGradient, NULL);
    glCompileShader(fShaderGradient);
    ShaderLog(fShaderGradient);

    ProgramGradient = glCreateProgram();
    glAttachShader(ProgramGradient, vShaderGradient);
    glAttachShader(ProgramGradient, fShaderGradient);
    glLinkProgram(ProgramGradient);

    Attrib_vertex_gradient = glGetAttribLocation(ProgramGradient, "position");
    Attrib_color_gradient = glGetAttribLocation(ProgramGradient, "color");
    
    if (Attrib_vertex_gradient == -1) {
        std::cout << "Could not bind attrib position in gradient program" << std::endl;
    }
    if (Attrib_color_gradient == -1) {
        std::cout << "Could not bind attrib color in gradient program" << std::endl;
    }
}

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

std::vector<Vertex> createFan(float centerX, float centerY, float radius, int segments, float startAngle, float endAngle) {
    std::vector<Vertex> fan;

    // Центр веера
    fan.push_back({ centerX, centerY });

    // Вершины по окружности
    float angleStep = (endAngle - startAngle) / (segments - 1);
    for (int i = 0; i < segments; ++i) {
        float angle = startAngle + i * angleStep;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        fan.push_back({ x, y });
    }

    return fan;
}

std::vector<ColorVertex> createGradientVertices() {
    std::vector<ColorVertex> gradientVertices;
    
    // Квадрат
    gradientVertices.push_back({-0.8f, -0.8f, 1.0f, 0.0f, 0.0f, 1.0f}); // красный
    gradientVertices.push_back({-0.3f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f}); // зеленый
    gradientVertices.push_back({-0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 1.0f}); // синий
    
    gradientVertices.push_back({-0.8f, -0.8f, 1.0f, 0.0f, 0.0f, 1.0f}); // красный
    gradientVertices.push_back({-0.3f, -0.3f, 0.0f, 0.0f, 1.0f, 1.0f}); // синий
    gradientVertices.push_back({-0.8f, -0.3f, 1.0f, 1.0f, 0.0f, 1.0f}); // желтый

    // Веер
    std::vector<Vertex> fan = createFan(0.0f, 0.0f, 0.5f, 10, 3.14159 / 8, 3.14159 * 7 / 8);

    std::vector<std::vector<float>> fanColors = {
        {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},
        {0.5f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.5f, 1.0f}
    };

    gradientVertices.push_back({ fan[0].x, fan[0].y, 1.0f, 1.0f, 1.0f, 1.0f });
    for (int i = 1; i < fan.size(); ++i) {
        gradientVertices.push_back({ fan[i].x, fan[i].y,
                                  fanColors[i - 1][0], fanColors[i - 1][1],
                                  fanColors[i - 1][2], fanColors[i - 1][3] });
    }

    // Пятиугольник
    Vertex center = {0.5f, -0.5f};
    std::vector<Vertex> pentagon = createPentagon(center.x, center.y, 0.3f);
    
    std::vector<std::vector<float>> pentagonColors = {
        {1.0f, 0.0f, 0.0f, 1.0f},  // красный
        {1.0f, 0.5f, 0.0f, 1.0f},  // оранжевый
        {1.0f, 1.0f, 0.0f, 1.0f},  // желтый
        {0.5f, 1.0f, 0.0f, 1.0f},  // желто-зеленый
        {0.0f, 1.0f, 0.0f, 1.0f}   // зеленый
    };
    
    for (int i = 0; i < 5; ++i) {
        // Центр - белый
        gradientVertices.push_back({center.x, center.y, 1.0f, 1.0f, 1.0f, 1.0f});
        gradientVertices.push_back({pentagon[i].x, pentagon[i].y, 
                                  pentagonColors[i][0], pentagonColors[i][1], 
                                  pentagonColors[i][2], pentagonColors[i][3]});
        gradientVertices.push_back({pentagon[(i + 1) % 5].x, pentagon[(i + 1) % 5].y,
                                  pentagonColors[(i + 1) % 5][0], pentagonColors[(i + 1) % 5][1],
                                  pentagonColors[(i + 1) % 5][2], pentagonColors[(i + 1) % 5][3]});
    }
    
    return gradientVertices;
}

void InitVBO() {
    std::vector<Vertex> quad = {
        {-0.8f, -0.8f}, {-0.3f, -0.8f}, {-0.3f, -0.3f},
        {-0.8f, -0.8f}, {-0.3f, -0.3f}, {-0.8f, -0.3f}
    };

    //std::vector<Vertex> fan = {
    //    {0.0f, 0.0f}, {0.47f, 0.17f}, {0.41f, 0.32f}, {0.32f, 0.44f},
    //    {0.21f, 0.53f}, {0.07f, 0.57f}, {-0.07f, 0.57f}, {-0.21f, 0.53f},
    //    {-0.32f, 0.44f}, {-0.41f, 0.32f}, {-0.47f, 0.17f}
    //};
    std::vector<Vertex> fan = createFan(0.0f, 0.0f, 0.5f, 10, 3.14159 / 8, 3.14159 * 7 / 8);

    std::vector<Vertex> pentagonVertices;
    Vertex center = {0.5f, -0.5f};
    std::vector<Vertex> pentagon = createPentagon(center.x, center.y, 0.3f);
    
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

    std::vector<ColorVertex> gradientVertices = createGradientVertices();
    
    glGenBuffers(1, &VBO_Gradient);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Gradient);
    glBufferData(GL_ARRAY_BUFFER, gradientVertices.size() * sizeof(ColorVertex), 
                 gradientVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Init() {
    InitShader();
    InitVBO();
}

void Draw() {
    switch (currentAssignment) {
        case ASSIGNMENT_2: {
            // Quad
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glUseProgram(ProgramUniversal);
            glEnableVertexAttribArray(Attrib_vertex_universal);
            glVertexAttribPointer(Attrib_vertex_universal, 2, GL_FLOAT, GL_FALSE, 0, 0);
                
            glUniform4f(Uniform_color, 1.0f, 0.0f, 1.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 6);
                
            glUniform4f(Uniform_color, 1.0f, 0.5f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLE_FAN, 6, 11);
                
            glUniform4f(Uniform_color, 0.0f, 1.0f, 1.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 17, 15);
                
            glDisableVertexAttribArray(Attrib_vertex_universal);
            break;
        }
        case ASSIGNMENT_4: {
            
            glBindBuffer(GL_ARRAY_BUFFER, VBO_Gradient);

            glUseProgram(ProgramGradient);

            glEnableVertexAttribArray(Attrib_vertex_gradient);
            glVertexAttribPointer(Attrib_vertex_gradient, 2, GL_FLOAT, GL_FALSE, 
                                 sizeof(ColorVertex), (void*)0);
            
            glEnableVertexAttribArray(Attrib_color_gradient);
            glVertexAttribPointer(Attrib_color_gradient, 4, GL_FLOAT, GL_FALSE, 
                                 sizeof(ColorVertex), (void*)offsetof(ColorVertex, r));
            
            glDrawArrays(GL_TRIANGLES, 0, 6);        
            glDrawArrays(GL_TRIANGLE_FAN, 6, 11);    
            glDrawArrays(GL_TRIANGLES, 17, 15);      
            
            glDisableVertexAttribArray(Attrib_vertex_gradient);
            glDisableVertexAttribArray(Attrib_color_gradient);
            break;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Release() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO_Gradient);  
    glDeleteProgram(ProgramQuad);
    glDeleteProgram(ProgramFan);
    glDeleteProgram(ProgramPentagon);
    glDeleteProgram(ProgramUniversal);
    glDeleteProgram(ProgramGradient);
}