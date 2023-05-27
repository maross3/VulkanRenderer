#pragma once
#include "MainWindow.hpp"
#include "app_pipline.hpp"
#include "app_device.hpp"
#include "app_swap_chain.hpp"

#include <memory>
#include <vector>

namespace VulkanTest
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void Run();

	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();
		MainWindow _windowMain{WIDTH, HEIGHT, "Hello world"};

		AppDevice _appDevice{_windowMain};

		AppSwapChain _appSwapChain{_appDevice, _windowMain.GetExtent()};

		std::unique_ptr<AppPipeline> _appPipeline;
		VkPipelineLayout _pipelineLayout{};

		std::vector<VkCommandBuffer> _commandBuffers;
	};
}
