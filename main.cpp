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
GLuint theProgram;
void initScreen()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Hello World");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    gluOrtho2D(-780, 780, -420, 420);
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);
    
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
        const char *strShaderType = NULL;
        switch(eShaderType)
        {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        
        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();
    
    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
    	glAttachShader(program, shaderList[iLoop]);
    
    glLinkProgram(program);
    
    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
    
    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

void InitializeProgram()
{
    vector<GLuint> shaderList;
    string strVertexShader;
    string strFragmentShader;
    ostringstream ss;

    ifstream vertexShaderFile("shaders/vertexShader.glsl");
    ifstream fragmentShaderFile("shaders/fragmentShader.glsl");

    
    ss << vertexShaderFile.rdbuf();
    strVertexShader = ss.str();

    ss << fragmentShaderFile.rdbuf();
    strFragmentShader = ss.str();
    
    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));
    
    theProgram = CreateProgram(shaderList);

    for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void initVertexBuffer()
{
    const float vertexPositions[] = {
        0.0f,
        0.0f,

        0.0f,
        200.0f,

        600.0f,
        0.0f,

        600.0f,
        200.0f,

        600.0f,
        0.0f,

        0.0f,
        200.0f,

    };

    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void display(void)
{

    glUseProgram(theProgram);
    glClear(GL_COLOR_BUFFER_BIT);
    // glColor3f(1.0, 0.0, 1.0);

    initVertexBuffer();

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glFlush();
}
int main(int argcp, char **argv)

{
    glutInit(&argcp, argv);

    initScreen();
    glewInit();
    InitializeProgram();

    glUseProgram(theProgram);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
}