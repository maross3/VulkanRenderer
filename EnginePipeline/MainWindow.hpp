#pragma once

#define GLFW_INCLUDE_VULKAN

#include "glfw3.h"
#include<string>

namespace VulkanTest
{
	class MainWindow
	{
	public:
		MainWindow(int w, int h, std::string name);
		~MainWindow();

		MainWindow(const MainWindow&);
		MainWindow& operator=(MainWindow);

		[[nodiscard]] bool ShouldClose() const { return glfwWindowShouldClose(window); }

		[[nodiscard]] VkExtent2D GetExtent() const
		{
			return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
		}

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;
		void InitWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window{};
	};
}
