#include "first_app.hpp"
#include <array>
#include <stdexcept>
#include "Init.hpp"

namespace VulkanTest
{
	FirstApp::FirstApp()
	{
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	FirstApp::~FirstApp() { vkDestroyPipelineLayout(_appDevice.Device(), _pipelineLayout, nullptr); }

	void FirstApp::Run()
	{
		while (!_windowMain.ShouldClose())
		{
			glfwPollEvents();
			DrawFrame();
		}

		vkDeviceWaitIdle(_appDevice.Device());
	}

	void FirstApp::CreatePipelineLayout()
	{
		initializers::CreatePipelineLayoutCreateInfo(nullptr, 0);
		const auto layoutInfo = initializers::vulkanInfoStore.pipelineLayoutCreateInfo;
		if (vkCreatePipelineLayout(_appDevice.Device(), &layoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline layout!");
	}

	void FirstApp::CreatePipeline()
	{
		// auto pipelineConfig = AppPipeline::defaultPipelineConfigInfo(appSwapChain.width(), appSwapChain.height());
		PipelineConfigInfo pipelineConfig{};

		AppPipeline::DefaultPipelineConfigInfo(
			pipelineConfig,
			_appSwapChain.Width(),
			_appSwapChain.Height());

		pipelineConfig.renderPass = _appSwapChain.GetRenderPass();
		pipelineConfig.pipelineLayout = _pipelineLayout;

		_appPipeline = std::make_unique<AppPipeline>(_appDevice,
		                                            "Shaders/simple_shader.vert.spv",
		                                            "Shaders/simple_shader.frag.spv",
		                                            pipelineConfig);
	}

	void FirstApp::CreateCommandBuffers()
	{
		_commandBuffers.resize(_appSwapChain.ImageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = _appDevice.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

		if (vkAllocateCommandBuffers(_appDevice.Device(), &allocInfo, _commandBuffers.data()) !=
			VK_SUCCESS)
			throw std::runtime_error("failed to allocate command buffers!");

		for (size_t i = 0; i < _commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
				throw std::runtime_error("failed to begin recording command buffer!");

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = _appSwapChain.GetRenderPass();
			renderPassInfo.framebuffer = _appSwapChain.GetFrameBuffer(i);

			renderPassInfo.renderArea.offset = {0, 0};
			renderPassInfo.renderArea.extent = _appSwapChain.GetSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};

			// todo background color:
			clearValues[0].color = {0.2f, 0.2f, 0.2f, 1.0f}; // assigned color attachement
			clearValues[1].depthStencil = {1.0f, 0}; // assigned depth attachement

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();


			// For using secondary, pass below to vkCmdBeginRenderPass as third argument:
			//VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS
			vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			_appPipeline->Bind(_commandBuffers[i]);

			vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(_commandBuffers[i]);
			if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to record buffer");
		}
	}

	void FirstApp::DrawFrame()
	{
		uint32_t imageIndex;
		auto result = _appSwapChain.AcquireNextImage(&imageIndex);
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to aquire swap chain image");

		result = _appSwapChain.SubmitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to present swap chain image");
	}
}
