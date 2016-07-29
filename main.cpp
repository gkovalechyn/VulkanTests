/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: gkovalechyn
 *
 * Created on 18 de Julho de 2016, 14:18
 */
#define GLFW_DLL
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>
/*
 * 
 */
int main(int argc, char** argv) {
    GLFWwindow* window;
    uint32_t vkExtensionCount = 0;
    glm::vec4 vec;
    glm::mat4x4 mat;
    glm::vec4 res;
    
    glfwInit();
    
    if (glfwVulkanSupported() != GLFW_TRUE){
        glfwTerminate();
        //Send some message
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    window = glfwCreateWindow(800, 600, "Vulkan tests", nullptr, nullptr);
    
    vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
    
    std::cout << vkExtensionCount << " supported extensions." << std::endl;
    
    res = vec * mat;
    
    std::cout << "Resulting vector: (" << res.x << ", " << res.y << ", " << res.z << ")" << std::endl;
    
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    return 0;
}

