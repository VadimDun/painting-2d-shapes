#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <string>

GLuint Program;
GLuint Attrib_vertex;
GLuint VBO;

struct Vertex {
    GLfloat x;
    GLfloat y;
};

const char* VertexShaderSource = R"(
    #version 330 core
    in vec2 coord;
    void main() {
        gl_Position = vec4(coord, 0.0, 1.0);
    }
)";

const char* FragShaderSource = R"(
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
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vShader);
    std::cout << "Vertex shader log:" << std::endl;
    ShaderLog(vShader);

    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &FragShaderSource, NULL);
    glCompileShader(fShader);
    std::cout << "Fragment shader log:" << std::endl;
    ShaderLog(fShader);

    Program = glCreateProgram();
    glAttachShader(Program, vShader);
    glAttachShader(Program, fShader);
    glLinkProgram(Program);

    const char* attr_name = "coord";
    Attrib_vertex = glGetAttribLocation(Program, attr_name);
    if (Attrib_vertex == -1) {
        std::cout << "Could not bind attrib " << attr_name << std::endl;
        return;
    }
}

// создает вершины правильного пятиугольника
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
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void Init() {
    InitShader();
    InitVBO();
}

void Draw() {
    glUseProgram(Program);
    glEnableVertexAttribArray(Attrib_vertex);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(Attrib_vertex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // quad
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // fan
    glDrawArrays(GL_TRIANGLE_FAN, 6, 11);

    // pentagon
    glDrawArrays(GL_TRIANGLES, 17, 15);

    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glDisableVertexAttribArray(Attrib_vertex);
    glUseProgram(0);
}

void Release() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(Program);
}

int main() {
    setlocale(LC_ALL, "ru");

    // чтобы фигуры не расстягивались, нужно квадратное окно
    sf::Window window(sf::VideoMode(900, 900), "Shapes", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "GLEW initialization error: " << glewGetErrorString(glewInitResult) << std::endl;
        return -1;
    }

    if (!GLEW_VERSION_3_3) {
        std::cerr << "OpenGL 3.3 not supported!" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Draw();

        window.display();
    }

    Release();

    return 0;
}