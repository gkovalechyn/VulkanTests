/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Test1.cpp
 * Author: gkovalechyn
 * 
 * Created on July 30, 2016, 1:13 AM
 */

#include <intrin.h>

#include "Test1.h"

Test1::Test1() {
}

Test1::Test1(const Test1& orig) {
}

Test1::~Test1() {
}

void Test1::initVulkan() {
    VkApplicationInfo info = {};
    VkInstanceCreateInfo createInfo = {};
    std::vector<const char*> glfwExtensions;
    
    if (this->enableValidationLayers && !this->checkValidationLayers()){
        throw std::runtime_error("Validation layers not available");
    }
    
    info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName = "Vulkan test 1";
    info.pEngineName = "No engine";
    info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    info.apiVersion = VK_API_VERSION_1_0;
    info.pNext = nullptr;
    
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &info;
    
#if defined(DEBUG)
    createInfo.enabledLayerCount = this->validationLayers.size();
    createInfo.ppEnabledLayerNames = this->validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif
    
    glfwExtensions = this->getGLFWRequiredExtensions();
    
    createInfo.enabledExtensionCount = glfwExtensions.size();
    createInfo.ppEnabledExtensionNames = glfwExtensions.data();
    
    if (vkCreateInstance(&createInfo, nullptr, this->vkInstance) != VK_SUCCESS){
        throw std::runtime_error("Could not create Vulkan instance.");
    }
    
    std::cout << "Created the vulkan instance. " << *this->vkInstance << std::endl;
    std::cout << "Setting up the debug callback." << std::endl;
    
    if (this->setupDebugCallback(nullptr) != VK_SUCCESS){
        std::cout << "Could not setup the debug callback." << std::endl;
    }
}

void Test1::mainLoop() {
    while(!glfwWindowShouldClose(this->window)){
        glfwPollEvents();
    }
}

void Test1::run() {
    this->initVulkan();
    
    this->window = glfwCreateWindow(800, 600, "Vulkan - Test 1", nullptr, nullptr);
    
    this->mainLoop();
    
    this->destroyDebugCallbak();
    vkDestroyInstance(*this->vkInstance, nullptr);
    glfwDestroyWindow(this->window);
}

bool Test1::checkValidationLayers() {
    uint32_t layerCount;
    
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data());
    
#if defined(DEBUG)
    for(VkLayerProperties property : layerProperties){
        std::cout << "Found VkLayerProperty: " << property.layerName << " version " << property.implementationVersion << std::endl;
    }
#endif
    
    for(const char* layer : this->validationLayers){
        bool foundLayer = false;
        
        for (const VkLayerProperties property : layerProperties){
            if (strcmp(layer, property.layerName) == 0){
                foundLayer = true;
                break;
            }
        }
        
        if (!foundLayer){
            return false;
        }
    }
    
    return true;
}

std::vector<const char*> Test1::getGLFWRequiredExtensions() {
    uint32_t extensionCount;
    std::vector<const char*> required;
    const char** extensions;
    
    extensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    
    for(uint32_t i = 0; i < extensionCount; i++){
        std::cout << "GLFW required extension: " << extensions[i] << std::endl;
        required.push_back(extensions[i]);
    }
    
#if defined(DEBUG)
    required.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
    
    return required;
}

VkResult Test1::setupDebugCallback(const VkAllocationCallbacks* allocator) {
    VkDebugReportCallbackCreateInfoEXT info = {};
    PFN_vkCreateDebugReportCallbackEXT function = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(*this->vkInstance, "vkCreateDebugReportCallbackEXT");
    
    info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    info.pfnCallback = (PFN_vkDebugReportCallbackEXT) Test1::vkDebugCallback;
    
    if (function != nullptr){
        std::cout << "Creating debug callback." << std::endl;
        function(*this->vkInstance, &info, allocator, &this->callbackHandle);
        return VK_SUCCESS;
    }else{
        std::cout << "Failed to get the address of vkCreateDebugReportCallbackEXT" << std::endl;
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkBool32 Test1::vkDebugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* message, void* userData) {
    std::cerr << "Validation layer \""  << layerPrefix << "\": " << message << std::endl;
    
    //No idea why this has to return anything, returning true or false
    //made no difference.
    return VK_FALSE;
}

void Test1::destroyDebugCallbak() {
    PFN_vkDestroyDebugReportCallbackEXT func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(*this->vkInstance, "vkDestroyDebugReportCallbackEXT");
    
    if (func != nullptr){
        func(*this->vkInstance, this->callbackHandle, nullptr);
    }else{
        std::cout << "Could not find the address for \"vkDestroyDebugReportCallbackEXT\"" << std::endl;
    }
}

void Test1::findPhysicalDevice() {
    uint32_t deviceCount = 0;
    
    vkEnumeratePhysicalDevices(*this->vkInstance, &deviceCount, nullptr);
    
    if (deviceCount == 0){
        throw std::runtime_error("No devices with vulkan support found!");
    }
    std::vector<VkPhysicalDevice*> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(*this->vkInstance, &deviceCount, physicalDevices.data());
    
    for(VkPhysicalDevice device : physicalDevices){
        if (this->isDeviceSuitable(device)){
            this->physicalDevice = device;
            break;
        }
    }
    
    if (this->physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("Could not find suitable GPU");
    }
}

bool Test1::isDeviceSuitable(VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);
    
    return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            features.geometryShader;
}









