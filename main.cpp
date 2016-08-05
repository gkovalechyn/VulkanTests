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

#include <stdexcept>
#include <iostream>

#include "Test1/Test1.h"
/*
 * 
 */
int main(int argc, char** argv) {
    
    uint32_t vkExtensionCount = 0;
    Test1 test1;
    
    glfwInit();
    
    if (glfwVulkanSupported() != GLFW_TRUE){
        glfwTerminate();
        //Send some message
        return 1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    try{
        test1.run();
    }catch(const std::runtime_error& e){
        std::cout << e.what() << std::endl;
        
        glfwTerminate();
        return 1;
    }
    //vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);
    //std::cout << vkExtensionCount << " supported extensions." << std::endl;
    
    glfwTerminate();
    return 0;
}

