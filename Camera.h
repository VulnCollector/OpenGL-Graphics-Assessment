#pragma once
#include <vec3.hpp>
#include<mat4x4.hpp>
#include<GLFW/glfw3.h>

class Camera
{
public:
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float w, float h);
    void update(float deltaTime, GLFWwindow* window);
    void setPosition(glm::vec3 pos);
    void setTheta(float theta);
    void setPhi(float phi);
private:
    float m_theta;
    float m_phi;
    glm::vec3 m_position;
};

