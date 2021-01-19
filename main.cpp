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
#define pi 3.142857
using namespace std;

uint positionBufferObject;
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
    -0.5f, 0.6f, -0.5f,
     0.5f, 0.0f, -0.5f,
     0.0f, 0.0f, 0.5f
    };

    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int compileVertexShader() {
    int  success;
    char infoLog[512];
    ostringstream ss;
    string strVertexShader;
    ifstream vertexShaderFile("shaders/vertexShader.glsl");
    const char *vertexShaderSource;
    unsigned int vertexShader;

    ss << vertexShaderFile.rdbuf();
    strVertexShader = ss.str();
    vertexShaderSource =  strVertexShader.data();
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


    if(!success)
    {  
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        cout << "compiled successfully\n";
    }
    return vertexShader;

}

unsigned int compileFragmentShader() {
    int  success;
    char infoLog[512];
    ostringstream ss;
    string strFragmentShader;
    ifstream fragmentShaderFile("shaders/fragmentShader.glsl");
    const char *fragmentShaderSource;
    unsigned int fragmentShader;

    ss << fragmentShaderFile.rdbuf();
    strFragmentShader = ss.str();
    fragmentShaderSource =  strFragmentShader.data();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);


    if(!success)
    {  
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::fragment::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        cout << "compiled successfully\n";
    }

    return fragmentShader;

}

unsigned int linkShaders() {
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
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR: linking error\n";
    } else {
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

    initVertexBuffer();

    // glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glUseProgram(shaderProgram);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFlush();
}
int main(int argcp, char **argv)

{
    
    glutInit(&argcp, argv);

    initScreen();
    glewInit();
    linkShaders();

    shaderProgram = linkShaders();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}