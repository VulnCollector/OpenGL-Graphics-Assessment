#include "glad.h"
#include "Application.h"
#include "imgui_glfw3.h"
#include <gtc/matrix_transform.hpp>


Application* Application::s_instance = nullptr;

Application::Application()
{
    
}

Application::~Application()
{
}

bool Application::startup()
{
    if (glfwInit() == false)
        return false;

    m_window = glfwCreateWindow(windowWidth, windowHeight, "Computer Graphics", nullptr, nullptr);

    if (m_window == nullptr) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL()) {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        return false;
    }

    printf("GL: %i.%i\n", GLVersion.major, GLVersion.minor);

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST); // enables the depth buffer

    Gizmos::create(10000, 10000, 10000, 10000);

    aie::ImGui_Init(m_window, true);

    s_instance = this;
    glfwSetCursorPosCallback(m_window, &Application::SetMousePosition);

    m_camera.setPosition(glm::vec3(-10, 2, 0));
    m_camera.setPhi(0.0f);
    m_camera.setTheta(0.0f);

    // create simple camera transforms
    

    //m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
   // m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

   m_phongShader.loadShader(aie::eShaderStage::VERTEX, "shaders/phong.vert");
   m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "shaders/phong.frag");

   if(!m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalmap.vert"))
       printf("Shader Error: %s\n", m_normalMapShader.getLastError());
   if(!m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalmap.frag"))
       printf("Shader Error: %s\n", m_normalMapShader.getLastError());


    if (m_normalMapShader.link() == false) {
        printf("Shader Error: %s\n", m_normalMapShader.getLastError());
        return false;
    }


    if (m_phongShader.link() == false) {
        printf("Shader Error: %s\n", m_phongShader.getLastError());
        return false;
    }

    m_light.direction = glm::normalize(glm::vec3(1, 1, 1));
    m_light.colour = { 1, 1, 1 };
    m_ambientLight = { 0.25f, 0.25f, 0.25f };

    


    /*m_bunnyMesh.initialiseFromFile("./stanford/bunny.obj");
    m_bunnyMesh.loadMaterial("./stanford/bunny.mtl");
    m_bunnyTransform = glm::mat4(1.0f);*/

    m_soulspearMesh.initialiseFromFile("./soulspear/soulspear.obj");
    m_soulspearMesh.loadMaterial("./soulspear/soulspear.mtl");
    m_soulspearTransform = glm::mat4(1.0f);

    m_quadMesh.initialiseQuad();
    m_quadTransform = glm::mat4(1.0f);
    m_quadTransform = glm::scale(m_quadTransform, glm::vec3(3.5f));

    if (!m_gridTexture.load("textures/numbered_grid.tga"))
        std::cerr << "failed to load m_gridtex\n";


    // make the quad 10 units wide
  /*  m_quadTransform = {
          1,0,0,0,
          0,1,0,0,
          0,0,1,0,
          0,0,0,1 };*/

    return true;
}

bool Application::update()
{
    aie::ImGui_NewFrame();

    m_camera.update(0.1f, m_window);

    m_lastMousePosition = m_mousePosition;

    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -1.0f, 1.0f);
    ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
    ImGui::End();

    float time = (float)glfwGetTime();
    

    //m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2.0f), glm::sin(time * 2.0f), -1.0f));

    return glfwWindowShouldClose(m_window) == false &&
        glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
    
}

void Application::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Gizmos::clear();

    Gizmos::addTransform(glm::mat4(1));

    glm::mat4 pv = m_camera.getProjectionMatrix(windowWidth, windowHeight) * m_camera.getViewMatrix();

    // bind shader
    m_normalMapShader.bind();

    // bind light
    m_normalMapShader.bindUniform("AmbientColour", m_ambientLight);
    m_normalMapShader.bindUniform("LightColour", m_light.colour);
    m_normalMapShader.bindUniform("LightDirection", m_light.direction);
    m_normalMapShader.bindUniform("cameraPosition", vec3(glm::inverse(m_camera.getViewMatrix())[3]));

    // bind transform
    auto pvm = pv * m_soulspearTransform;
    m_normalMapShader.bindUniform("ProjectionViewModel", pvm);

    // bind transforms for lighting
    m_normalMapShader.bindUniform("ModelMatrix", m_soulspearTransform);

    m_soulspearMesh.applyMaterial(&m_normalMapShader);

    m_soulspearMesh.draw();

    
    //auto quadPVM = pv * m_quadTransform;
    //m_normalMapShader.bindUniform("ProjectionViewModel", quadPVM);
    //m_normalMapShader.bindUniform("ModelMatrix", m_quadTransform);

    //m_normalMapShader.bindUniform("Ka", vec3(0.25f, 0.25f, 0));
    //m_normalMapShader.bindUniform("Ks", vec3(1.0f));
    //m_normalMapShader.bindUniform("Kd", vec3(0.5f, 0.5f, 0));
    //m_normalMapShader.bindUniform("specularPower", 32.0f);

    //m_gridTexture.bind(3);
    //m_normalMapShader.bindUniform("diffuseTex", 3);


    //// draw quad
    //m_quadMesh.draw();
    
    
    

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i < 21; ++i) {
        Gizmos::addLine(vec3(-10 + i, 0, 10),
            vec3(-10 + i, 0, -10),
            i == 10 ? white : black);

        Gizmos::addLine(vec3(10, 0, -10 + i),
            vec3(-10, 0, -10 + i),
            i == 10 ? white : black);
    }

    Gizmos::draw(pv);

    ImGui::Render();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Application::shutdown()
{
    aie::ImGui_Shutdown();
    Gizmos::destroy();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    s_instance->m_mousePosition.x = (float)x;
    s_instance->m_mousePosition.y = (float)y;
}
