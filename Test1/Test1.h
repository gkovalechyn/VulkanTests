/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Test1.h
 * Author: gkovalechyn
 *
 * Created on July 30, 2016, 1:13 AM
 */

/*
 * I'm just following the tutorial for now. 
 */

#ifndef TEST1_H
#define TEST1_H

#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <GLFW/glfw3.h>
#include <vector>

class Test1 {
public:
    Test1();
    Test1(const Test1& orig);
    virtual ~Test1();
    
    void run();
    
    static VkBool32 vkDebugCallback(
        VkDebugReportFlagsEXT flags, 
        VkDebugReportObjectTypeEXT objectType, 
        uint64_t obj, 
        size_t location, 
        int32_t code, 
        const char* layerPrefix, 
        const char* message, 
        void* userData
    );
    
private:
    GLFWwindow* window;
    VkInstance* vkInstance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT callbackHandle;
    
    std::vector<const char*> validationLayers = {
        "VK_LAYER_LUNARG_standard_validation"
    };
    
    bool enableValidationLayers = true;
    
    void initVulkan();
    bool checkValidationLayers();
    VkResult setupDebugCallback(const VkAllocationCallbacks* allocator);
    void destroyDebugCallbak();
    void findPhysicalDevice();
    bool isDeviceSuitable(VkPhysicalDevice device);
    
    void mainLoop();
    
    std::vector<const char*> getGLFWRequiredExtensions();
    
};

#endif /* TEST1_H */

