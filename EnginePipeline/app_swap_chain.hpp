#pragma once

#include "app_device.hpp"

#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanTest
{
	class AppSwapChain
	{
	public:
		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		AppSwapChain(AppDevice& deviceRef, VkExtent2D windowExtent);
		~AppSwapChain();

		AppSwapChain(const AppSwapChain&) = delete;
		void operator=(const AppSwapChain&) = delete;

		[[nodiscard]] VkFramebuffer GetFrameBuffer(const size_t index) const { return _swapChainFramebuffers[index]; }
		[[nodiscard]] VkRenderPass GetRenderPass() const { return _renderPass; }
		[[nodiscard]] VkImageView GetImageView(const int index) const { return _swapChainImageViews[index]; }
		[[nodiscard]] size_t ImageCount() const { return _swapChainImages.size(); }
		[[nodiscard]] VkFormat GetSwapChainImageFormat() const { return _swapChainImageFormat; }
		[[nodiscard]] VkExtent2D GetSwapChainExtent() const { return _swapChainExtent; }
		[[nodiscard]] uint32_t Width() const { return _swapChainExtent.width; }
		[[nodiscard]] uint32_t Height() const { return _swapChainExtent.height; }

		[[nodiscard]] float ExtentAspectRatio() const
		{
			return static_cast<float>(_swapChainExtent.width) / static_cast<float>(_swapChainExtent.height);
		}

		[[nodiscard]] VkFormat FindDepthFormat() const;

		VkResult AcquireNextImage(uint32_t* imageIndex) const;
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, const uint32_t* imageIndex);

	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateSyncObjects();

		// Helper functions
		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(
			const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(
			const std::vector<VkPresentModeKHR>& availablePresentModes);
		[[nodiscard]] VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;

		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;

		std::vector<VkFramebuffer> _swapChainFramebuffers;
		VkRenderPass _renderPass;

		std::vector<VkImage> _depthImages;
		std::vector<VkDeviceMemory> _depthImageMemorys;
		std::vector<VkImageView> _depthImageViews;
		std::vector<VkImage> _swapChainImages;
		std::vector<VkImageView> _swapChainImageViews;

		AppDevice& _device;
		VkExtent2D _windowExtent;

		VkSwapchainKHR _swapChain;

		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;
		std::vector<VkFence> _imagesInFlight;
		size_t _currentFrame = 0;
	};
}
