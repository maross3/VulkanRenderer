#pragma once

#include "MainWindow.hpp"

#include <vector>
#include <vulkan/vulkan_core.h>

namespace VulkanTest
{
	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices
	{
		uint32_t graphicsFamily{};
		uint32_t presentFamily{};
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		bool IsComplete() const { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class AppDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		explicit AppDevice(MainWindow& window);
		~AppDevice();


		// Not copyable or movable
		AppDevice(const AppDevice&) = delete;
		void operator=(const AppDevice&) = delete;
		AppDevice(AppDevice&&) = delete;
		AppDevice& operator=(AppDevice&&) = delete;

		[[nodiscard]] VkCommandPool GetCommandPool() const { return commandPool; }
		[[nodiscard]] VkDevice Device() const { return device_; }
		[[nodiscard]] VkSurfaceKHR Surface() const { return surface_; }
		[[nodiscard]] VkQueue GraphicsQueue() const { return graphicsQueue_; }
		[[nodiscard]] VkQueue PresentQueue() const { return presentQueue_; }

		// ADDED
		VkInstance GetInstance() { return instance; }
		VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
		// END ADDED

		SwapChainSupportDetails getSwapChainSupport() { return QuerySwapChainSupport(physicalDevice); }
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		QueueFamilyIndices findPhysicalQueueFamilies() { return FindQueueFamilies(physicalDevice); }
		VkFormat FindSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

		// Buffer Helper Functions
		void CreateBuffer(
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory) const;
		VkCommandBuffer BeginSingleTimeCommands() const;
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
		void CopyBufferToImage(
			VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

		void CreateImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory) const;

		VkPhysicalDeviceProperties properties;

	private:
		void CreateInstance();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device) const;
		std::vector<const char*> GetRequiredExtensions() const;
		bool CheckValidationLayerSupport() const;
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const;
		void HasGflwRequiredInstanceExtensions() const;
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const;

		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		MainWindow& window;
		VkCommandPool commandPool;

		VkDevice device_;
		VkSurfaceKHR surface_;
		VkQueue graphicsQueue_;
		VkQueue presentQueue_;

		const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	};
} 