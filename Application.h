#pragma once

#include <iostream>
#include "Shader.h"
#include "Mesh.h"
#include "Gizmos.h"
#include "Camera.h"
#include "Texture.h"




using glm::vec3;
using glm::vec4;
using glm::mat4;

using aie::Gizmos;

struct Light
{
    glm::vec3 direction;
    glm::vec3 colour;
};



class Application
{
public:
    Application();
    ~Application();
    bool startup();
    bool update();
    void draw();
    void shutdown();
    float getWindowWidth() { return windowWidth; }
    float getWindowHeight() { return windowHeight; }

    static Application* get() { return s_instance; }
    glm::vec2 getMousePosition() { return m_mousePosition; }
    glm::vec2 getMouseDelta() { return m_mousePosition - m_lastMousePosition; }

    // set up mouse input
    static void SetMousePosition(GLFWwindow* window, double x, double y);

protected:
    static Application* s_instance;
  

    GLFWwindow* m_window;
    Camera m_camera;

    aie::ShaderProgram      m_shader;
    aie::ShaderProgram      m_phongShader;
    aie::ShaderProgram      m_normalMapShader;
    Mesh                    m_quadMesh;
    glm::mat4               m_quadTransform;
    glm::vec2               m_mousePosition;
    glm::vec2               m_lastMousePosition;
    aie::Texture            m_gridTexture;
    Mesh                    m_bunnyMesh;
    glm::mat4               m_bunnyTransform;
    Mesh                    m_soulspearMesh;
    glm::mat4               m_soulspearTransform;


    Light m_light;
    glm::vec3 m_ambientLight;

    const int windowWidth = 1280;
    const int windowHeight = 720;
};