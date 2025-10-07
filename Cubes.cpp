#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

// write vertext shader
const char* vs_src =R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec2 TexCoord;
void main()
{
    TexCoord = aTex;
    gl_Position = projection * view * model  * vec4(aPos,1.0);
}
)";

// frag shader
const char* fg_src=R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform int mode;
uniform float mixRatio;
void main()
{
    if(mode == 0)
    {
        FragColor = texture(myTexture1, TexCoord);
    }
    else
    if(mode == 1)
    {
        FragColor = texture(myTexture2, TexCoord);
    }
    else
        FragColor = mix(texture(myTexture1, TexCoord), texture(myTexture2, TexCoord), mixRatio);
}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

GLuint compileShader(GLenum type, const char* sharderSrc)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader,1, &sharderSrc, nullptr);
    glCompileShader(shader);

    GLint ok;
    glGetShaderiv(shader,GL_COMPILE_STATUS, &ok);
    if(!ok)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, ' ');
        glGetShaderInfoLog(shader, len, nullptr, &log[0]);
        std::cerr << "Error Compiling Shader: " << log << std::endl;

    }
    return shader;

}

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window)
{
    float cameraSpeed = 2.5f * deltaTime; // movement speed

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

int main()
{
    if(!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(800,800, "Textured Cube", nullptr, nullptr);

    if(!window)
    {

        std::cerr << "create window failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(glewInit() != GLEW_OK)
    {
        std::cerr << "glew Init failed" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float vertices[] = {
        // positions          // texcoords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VAO, VBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT,GL_FALSE, 5* sizeof(float),(void *) (3 * sizeof(float)) );
    glEnableVertexAttribArray(1);

    // compile shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vs_src);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fg_src);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    stbi_set_flip_vertically_on_load(true);


    GLuint texture1, texture2;
    // Texture 1
    glGenTextures(1,&texture1);
    glBindTexture(GL_TEXTURE_2D,texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width,height,nr;
    unsigned char* data = stbi_load("container.jpg",&width,&height,&nr,0);
    if(data) {
    GLenum fmt = (nr== 4)? GL_RGBA: GL_RGB;
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,fmt ,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { std::cout<<"Failed to load container.jpg\n"; }
    stbi_image_free(data);

// Texture 2
    glGenTextures(1,&texture2);
    glBindTexture(GL_TEXTURE_2D,texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("mossyRock.jpg",&width,&height,&nr,0);

    if(data) {

    GLenum fmt = (nr== 4)? GL_RGBA: GL_RGB;
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,fmt ,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else { std::cout<<"Failed to load mossyRock.jpg\n"; }
    stbi_image_free(data);


    



    
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program,"myTexture1"),0);
    glUniform1i(glGetUniformLocation(program,"myTexture2"),1);

    while(!glfwWindowShouldClose(window))   
    {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        
        glClearColor(0.1, 0.2, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)width / (float)height,
                                                0.1f, 100.0f);
        // view: move camera back along Z
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // model: rotate over time
        float t = (float)glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, t * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // send matrices to shader
        glUseProgram(program);
        GLint locModel = glGetUniformLocation(program, "model");
        GLint locView = glGetUniformLocation(program, "view");
        GLint locProj = glGetUniformLocation(program, "projection");
        glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(glGetUniformLocation(program,"mode"), 2);
        glUniform1f(glGetUniformLocation(program,"mixRatio"), 0.4f);
        // bind texture and draw cube (36 vertices)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // lets add another cube with only container texture to right
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(1.5f,0.0f, 0.0f));
        model2 = glm::rotate(model2, (float) glfwGetTime() * glm::radians(120.0f), glm::vec3(1.0f, 0.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model2));

        glUniform1i(glGetUniformLocation(program,"mode"), 0);
        
        glDrawArrays(GL_TRIANGLES, 0,  36);



        // lets add another cube with only moss texture to left
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, glm::vec3(-1.5f,0.0f, 0.0f));
        model3 = glm::rotate(model3, (float) glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model3));

        glUniform1i(glGetUniformLocation(program,"mode"), 1);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawArrays(GL_TRIANGLES, 0,  36);


        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glDeleteTextures(1, &texture1);
    glDeleteProgram(program);
    glDeleteBuffers(1, &VBO);


    return 0;
}