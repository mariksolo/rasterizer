#include <GL/glew.h>
#include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>

#define pi 3.142857
using namespace std;

unsigned int positionBufferObject;
unsigned int positionVertexArrayObject;
unsigned int elementBufferObject;
unsigned int shaderProgram;
void initScreen()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Hello World");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    gluOrtho2D(-780, 780, -420, 420);
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void initVertexBuffer()
{
    float vertexPositions[] = {
       0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };

    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initElementBuffer()
{

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenBuffers(1, &elementBufferObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initVertexArray()
{
    glGenVertexArrays(1, &positionVertexArrayObject);
    glBindVertexArray(positionVertexArrayObject);
}

unsigned int compileVertexShader()
{
    int success;
    char infoLog[512];
    ostringstream ss;
    string strVertexShader;
    ifstream vertexShaderFile("shaders/vertexShader.glsl");
    const char *vertexShaderSource;
    unsigned int vertexShader;

    ss << vertexShaderFile.rdbuf();
    strVertexShader = ss.str();
    vertexShaderSource = strVertexShader.data();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    else
    {
        cout << "compiled successfully\n";
    }
    return vertexShader;
}

unsigned int compileFragmentShader()
{
    int success;
    char infoLog[512];
    ostringstream ss;
    string strFragmentShader;
    ifstream fragmentShaderFile("shaders/fragmentShader.glsl");
    const char *fragmentShaderSource;
    unsigned int fragmentShader;

    ss << fragmentShaderFile.rdbuf();
    strFragmentShader = ss.str();
    fragmentShaderSource = strFragmentShader.data();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    else
    {
        cout << "compiled successfully\n";
    }

    return fragmentShader;
}

unsigned int linkShaders()
{
    unsigned int vertexShader = compileVertexShader();
    unsigned int fragmentShader = compileFragmentShader();
    unsigned int shaderProgram;

    int success;
    char infoLog[512];

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR: linking error\n";
    }
    else
    {
        cout << "linked successfully\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    // glColor3f(1.0, 0.0, 1.0);

    glBindBuffer(GL_ARRAY_BUFFER, positionVertexArrayObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);


    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    glUseProgram(shaderProgram);

    // time_t timeValue = std::time(0);
    
    // float redValue = abs(sin(rand() % 5000));
    // float greenValue = abs(sin(rand() % 5000));
    // float blueValue = abs(sin( rand() % 5000));
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");

    // glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glFlush();
}

void idle()
{
    glutPostRedisplay();
}
int main(int argcp, char **argv)

{
    srand(time(0));
    glutInit(&argcp, argv);

    initScreen();
    glewInit();
    linkShaders();

    shaderProgram = linkShaders();
    initVertexBuffer();
    initVertexArray();
    // initElementBuffer();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMainLoop();
}