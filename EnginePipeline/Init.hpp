#pragma once

#include <vector>
#include "vulkan/vulkan.hpp"

#include <cstring>
#include <glfw3.h>
#include <iostream>
#include <set>
#include <unordered_set>
// Responsible for all initialization for the engine.
// Hands off vulkanInfoStore to VoxelateEngine implementation
// This will be refactored into multiple classes, and not inline
namespace initializers
{
#ifdef NDEBUG
	inline const bool enableValidationLayers = false;
#else
	inline constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif
	inline VkInstance vkInstance;
	inline const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

	// local callback functions
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

	inline std::vector<const char*> GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (ENABLE_VALIDATION_LAYERS)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	inline void PopulateDebugMessengerCreateInfo(
		VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
		createInfo.pUserData = nullptr;
	}

	inline VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance,
			"vkCreateDebugUtilsMessengerEXT");

		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	// Temporary struct to refactor app_device, app_swap_chain and app_pipline into
	// the "initializers" namespace
	struct VulkanInfoStore
	{
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkMemoryAllocateInfo memAllocInfo;
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;
		VkMappedMemoryRange mappedMemoryRange;
		VkCommandBufferAllocateInfo commandBufferAllocateInfo;
		VkCommandPoolCreateInfo cmdPoolCreateInfo;
		VkCommandBufferBeginInfo cmdBufferBeginInfo;
		VkCommandBufferInheritanceInfo cmdBufferInheritanceInfo;
		VkRenderPassCreateInfo renderPassCreateInfo;
		VkImageViewCreateInfo imageViewCreateInfo;
		VkBindSparseInfo bindSparseInfo;
		VkFramebufferCreateInfo framebufferCreateInfo;
		VkApplicationInfo appInfo;
		VkWriteDescriptorSetAccelerationStructureKHR writeDescriptorSetAccelerationStructureKHR;
		VkRayTracingPipelineCreateInfoKHR rayTracingPipelineCreateInfoKHR;
		VkRayTracingShaderGroupCreateInfoKHR rayTracingShaderGroupCreateInfoKHR;
		VkAccelerationStructureBuildSizesInfoKHR accelerationStructureBuildSizesInfoKHR;
		VkAccelerationStructureBuildGeometryInfoKHR accelerationStructureBuildGeometryInfoKHR;
		VkAccelerationStructureGeometryKHR accelerationStructureGeometryKHR;
		VkSpecializationInfo specializationInfo;
		VkSpecializationMapEntry specializationMapEntry;
		VkPushConstantRange pushConstantRange;
		VkComputePipelineCreateInfo computePipelineCreateInfo;
		VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo;
		VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo;
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo;
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState;
		VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo;
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		VkVertexInputAttributeDescription vInputAttribDescription;
		VkVertexInputBindingDescription vInputBindDescription;
		VkWriteDescriptorSet writeDescriptorSet;
		VkDescriptorImageInfo descriptorImageInfo;
		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
		VkDescriptorSetLayoutBinding setLayoutBinding;
		VkDescriptorPoolSize descriptorPoolSize;
		VkDescriptorPoolCreateInfo descriptorPoolInfo;
		VkBufferCreateInfo bufCreateInfo;
		VkViewport viewport;
		VkSubmitInfo submitInfo;
		VkEventCreateInfo eventCreateInfo;
		VkFenceCreateInfo fenceCreateInfo;
		VkSemaphoreCreateInfo semaphoreCreateInfo;
		VkSamplerCreateInfo samplerCreateInfo;
		VkImageCreateInfo imageCreateInfo;
		VkMemoryBarrier memoryBarrier;
		VkBufferMemoryBarrier bufferMemoryBarrier;
		VkImageMemoryBarrier imageMemoryBarrier;
		VkRenderPassBeginInfo renderPassBeginInfo;
		VkInstanceCreateInfo createInstanceInfo;
		std::vector<const char*> extensions;
	};

	inline VulkanInfoStore vulkanInfoStore{};

	inline void SetupDebugMessenger()
	{
		if (!ENABLE_VALIDATION_LAYERS) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};

		PopulateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo,
		                                 nullptr, &vulkanInfoStore.debugMessenger) != VK_SUCCESS)
			throw std::runtime_error("failed to set up debug messenger!");
	}


	inline void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			instance,
			"vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}

	inline void DestroyMessenger()
	{
		DestroyDebugUtilsMessengerEXT(vkInstance, vulkanInfoStore.debugMessenger, nullptr);
	}

	inline VkMemoryAllocateInfo CreateMemoryAllocateInfo()
	{
		vulkanInfoStore.memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		return vulkanInfoStore.memAllocInfo;
	}

	inline VkMappedMemoryRange CreateMappedMemoryRange()
	{
		vulkanInfoStore.mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		return vulkanInfoStore.mappedMemoryRange;
	}

	inline VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo(
		VkCommandPool commandPool,
		VkCommandBufferLevel level,
		uint32_t bufferCount)
	{
		vulkanInfoStore.commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vulkanInfoStore.commandBufferAllocateInfo.commandPool = commandPool;
		vulkanInfoStore.commandBufferAllocateInfo.level = level;
		vulkanInfoStore.commandBufferAllocateInfo.commandBufferCount = bufferCount;
		return vulkanInfoStore.commandBufferAllocateInfo;
	}

	inline VkCommandPoolCreateInfo CreateCommandPoolCreateInfo()
	{
		vulkanInfoStore.cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		return vulkanInfoStore.cmdPoolCreateInfo;
	}

	inline VkCommandBufferBeginInfo CreateCommandBufferBeginInfo()
	{
		vulkanInfoStore.cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		return vulkanInfoStore.cmdBufferBeginInfo;
	}

	inline VkCommandBufferInheritanceInfo CreateCommandBufferInheritanceInfo()
	{
		vulkanInfoStore.cmdBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		return vulkanInfoStore.cmdBufferInheritanceInfo;
	}

	inline VkRenderPassBeginInfo CreateRenderPassBeginInfo()
	{
		vulkanInfoStore.renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		return vulkanInfoStore.renderPassBeginInfo;
	}

	inline VkRenderPassCreateInfo CreateRenderPassCreateInfo()
	{
		vulkanInfoStore.renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		return vulkanInfoStore.renderPassCreateInfo;
	}

	/** @brief Initialize an image memory barrier with no image transfer ownership */
	inline VkImageMemoryBarrier CreateImageMemoryBarrier()
	{
		vulkanInfoStore.imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		vulkanInfoStore.imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vulkanInfoStore.imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		return vulkanInfoStore.imageMemoryBarrier;
	}

	/** @brief Initialize a buffer memory barrier with no image transfer ownership */
	inline VkBufferMemoryBarrier CreateBufferMemoryBarrier()
	{
		vulkanInfoStore.bufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		vulkanInfoStore.bufferMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		vulkanInfoStore.bufferMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		return vulkanInfoStore.bufferMemoryBarrier;
	}

	inline VkMemoryBarrier CreateMemoryBarrier()
	{
		vulkanInfoStore.memoryBarrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
		return vulkanInfoStore.memoryBarrier;
	}

	inline VkImageCreateInfo CreateImageCreateInfo()
	{
		vulkanInfoStore.imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		return vulkanInfoStore.imageCreateInfo;
	}

	inline VkSamplerCreateInfo CreateSamplerCreateInfo()
	{
		vulkanInfoStore.samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		vulkanInfoStore.samplerCreateInfo.maxAnisotropy = 1.0f;
		return vulkanInfoStore.samplerCreateInfo;
	}

	inline VkImageViewCreateInfo CreateImageViewCreateInfo()
	{
		vulkanInfoStore.imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		return vulkanInfoStore.imageViewCreateInfo;
	}

	inline VkFramebufferCreateInfo CreateFramebufferCreateInfo()
	{
		vulkanInfoStore.framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		return vulkanInfoStore.framebufferCreateInfo;
	}

	inline VkSemaphoreCreateInfo CreateSemaphoreCreateInfo()
	{
		vulkanInfoStore.semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		return vulkanInfoStore.semaphoreCreateInfo;
	}

	inline VkFenceCreateInfo CreateFenceCreateInfo(VkFenceCreateFlags flags = 0)
	{
		vulkanInfoStore.fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vulkanInfoStore.fenceCreateInfo.flags = flags;
		return vulkanInfoStore.fenceCreateInfo;
	}

	inline VkEventCreateInfo CreateEventCreateInfo()
	{
		vulkanInfoStore.eventCreateInfo.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
		return vulkanInfoStore.eventCreateInfo;
	}

	inline VkSubmitInfo CreateSubmitInfo()
	{
		vulkanInfoStore.submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		return vulkanInfoStore.submitInfo;
	}

	inline VkViewport CreateViewport(
		float width,
		float height,
		float minDepth,
		float maxDepth)
	{
		vulkanInfoStore.viewport.width = width;
		vulkanInfoStore.viewport.height = height;
		vulkanInfoStore.viewport.minDepth = minDepth;
		vulkanInfoStore.viewport.maxDepth = maxDepth;
		return vulkanInfoStore.viewport;
	}

	inline VkRect2D CreateRect2D(
		int32_t width,
		int32_t height,
		int32_t offsetX,
		int32_t offsetY)
	{
		VkRect2D rect2D{};
		rect2D.extent.width = width;
		rect2D.extent.height = height;
		rect2D.offset.x = offsetX;
		rect2D.offset.y = offsetY;
		return rect2D;
	}

	inline VkBufferCreateInfo CreateBufferCreateInfo()
	{
		vulkanInfoStore.bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		return vulkanInfoStore.bufCreateInfo;
	}

	inline VkBufferCreateInfo CreateBufferCreateInfo(
		VkBufferUsageFlags usage,
		VkDeviceSize size)
	{
		vulkanInfoStore.bufCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vulkanInfoStore.bufCreateInfo.usage = usage;
		vulkanInfoStore.bufCreateInfo.size = size;
		return vulkanInfoStore.bufCreateInfo;
	}

	inline VkDescriptorPoolCreateInfo CreateDescriptorPoolCreateInfo(
		uint32_t poolSizeCount,
		VkDescriptorPoolSize* pPoolSizes,
		uint32_t maxSets)
	{
		vulkanInfoStore.descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		vulkanInfoStore.descriptorPoolInfo.poolSizeCount = poolSizeCount;
		vulkanInfoStore.descriptorPoolInfo.pPoolSizes = pPoolSizes;
		vulkanInfoStore.descriptorPoolInfo.maxSets = maxSets;
		return vulkanInfoStore.descriptorPoolInfo;
	}

	inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(
		const std::vector<VkDescriptorPoolSize>& poolSizes,
		uint32_t maxSets)
	{
		vulkanInfoStore.descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		vulkanInfoStore.descriptorPoolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		vulkanInfoStore.descriptorPoolInfo.pPoolSizes = poolSizes.data();
		vulkanInfoStore.descriptorPoolInfo.maxSets = maxSets;
		return vulkanInfoStore.descriptorPoolInfo;
	}

	inline VkDescriptorPoolSize CreateDescriptorPoolSize(
		VkDescriptorType type,
		uint32_t descriptorCount)
	{
		vulkanInfoStore.descriptorPoolSize.type = type;
		vulkanInfoStore.descriptorPoolSize.descriptorCount = descriptorCount;
		return vulkanInfoStore.descriptorPoolSize;
	}

	inline VkDescriptorSetLayoutBinding CreateDescriptorSetLayoutBinding(
		VkDescriptorType type,
		VkShaderStageFlags stageFlags,
		uint32_t binding,
		uint32_t descriptorCount = 1)
	{
		vulkanInfoStore.setLayoutBinding.descriptorType = type;
		vulkanInfoStore.setLayoutBinding.stageFlags = stageFlags;
		vulkanInfoStore.setLayoutBinding.binding = binding;
		vulkanInfoStore.setLayoutBinding.descriptorCount = descriptorCount;
		return vulkanInfoStore.setLayoutBinding;
	}

	inline VkDescriptorSetLayoutCreateInfo CreateDescriptorSetLayoutCreateInfo(
		const VkDescriptorSetLayoutBinding* pBindings,
		uint32_t bindingCount)
	{
		vulkanInfoStore.descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		vulkanInfoStore.descriptorSetLayoutCreateInfo.pBindings = pBindings;
		vulkanInfoStore.descriptorSetLayoutCreateInfo.bindingCount = bindingCount;
		return vulkanInfoStore.descriptorSetLayoutCreateInfo;
	}

	inline VkDescriptorSetLayoutCreateInfo CreateDescriptorSetLayoutCreateInfo(
		const std::vector<VkDescriptorSetLayoutBinding>& bindings)
	{
		vulkanInfoStore.descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		vulkanInfoStore.descriptorSetLayoutCreateInfo.pBindings = bindings.data();
		vulkanInfoStore.descriptorSetLayoutCreateInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		return vulkanInfoStore.descriptorSetLayoutCreateInfo;
	}

	inline VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo(
		const VkDescriptorSetLayout* pSetLayouts,
		uint32_t setLayoutCount = 1)
	{
		vulkanInfoStore.pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		vulkanInfoStore.pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
		vulkanInfoStore.pipelineLayoutCreateInfo.pSetLayouts = pSetLayouts;
		return vulkanInfoStore.pipelineLayoutCreateInfo;
	}

	inline VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo(
		uint32_t setLayoutCount = 1)
	{
		vulkanInfoStore.pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		vulkanInfoStore.pipelineLayoutCreateInfo.setLayoutCount = setLayoutCount;
		return vulkanInfoStore.pipelineLayoutCreateInfo;
	}

	inline VkDescriptorSetAllocateInfo CreateDescriptorSetAllocateInfo(
		VkDescriptorPool descriptorPool,
		const VkDescriptorSetLayout* pSetLayouts,
		uint32_t descriptorSetCount)
	{
		vulkanInfoStore.descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		vulkanInfoStore.descriptorSetAllocateInfo.descriptorPool = descriptorPool;
		vulkanInfoStore.descriptorSetAllocateInfo.pSetLayouts = pSetLayouts;
		vulkanInfoStore.descriptorSetAllocateInfo.descriptorSetCount = descriptorSetCount;
		return vulkanInfoStore.descriptorSetAllocateInfo;
	}

	inline VkDescriptorImageInfo CreateDescriptorImageInfo(VkSampler sampler, VkImageView imageView,
	                                                       VkImageLayout imageLayout)
	{
		vulkanInfoStore.descriptorImageInfo.sampler = sampler;
		vulkanInfoStore.descriptorImageInfo.imageView = imageView;
		vulkanInfoStore.descriptorImageInfo.imageLayout = imageLayout;
		return vulkanInfoStore.descriptorImageInfo;
	}

	inline VkWriteDescriptorSet writeDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorBufferInfo* bufferInfo,
		uint32_t descriptorCount = 1)
	{
		vulkanInfoStore.writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vulkanInfoStore.writeDescriptorSet.dstSet = dstSet;
		vulkanInfoStore.writeDescriptorSet.descriptorType = type;
		vulkanInfoStore.writeDescriptorSet.dstBinding = binding;
		vulkanInfoStore.writeDescriptorSet.pBufferInfo = bufferInfo;
		vulkanInfoStore.writeDescriptorSet.descriptorCount = descriptorCount;
		return vulkanInfoStore.writeDescriptorSet;
	}

	inline VkWriteDescriptorSet CreateWriteDescriptorSet(
		VkDescriptorSet dstSet,
		VkDescriptorType type,
		uint32_t binding,
		VkDescriptorImageInfo* imageInfo,
		uint32_t descriptorCount = 1)
	{
		vulkanInfoStore.writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		vulkanInfoStore.writeDescriptorSet.dstSet = dstSet;
		vulkanInfoStore.writeDescriptorSet.descriptorType = type;
		vulkanInfoStore.writeDescriptorSet.dstBinding = binding;
		vulkanInfoStore.writeDescriptorSet.pImageInfo = imageInfo;
		vulkanInfoStore.writeDescriptorSet.descriptorCount = descriptorCount;
		return vulkanInfoStore.writeDescriptorSet;
	}

	// inputs should be mapped to an input class. TODO afterinit
	inline VkVertexInputBindingDescription CreateVertexInputBindingDescription(
		uint32_t binding,
		uint32_t stride,
		VkVertexInputRate inputRate)
	{
		vulkanInfoStore.vInputBindDescription.binding = binding;
		vulkanInfoStore.vInputBindDescription.stride = stride;
		vulkanInfoStore.vInputBindDescription.inputRate = inputRate;
		return vulkanInfoStore.vInputBindDescription;
	}

	inline VkVertexInputAttributeDescription VertexInputAttributeDescription(
		uint32_t binding,
		uint32_t location,
		VkFormat format,
		uint32_t offset)
	{
		vulkanInfoStore.vInputAttribDescription.location = location;
		vulkanInfoStore.vInputAttribDescription.binding = binding;
		vulkanInfoStore.vInputAttribDescription.format = format;
		vulkanInfoStore.vInputAttribDescription.offset = offset;
		return vulkanInfoStore.vInputAttribDescription;
	}

	inline VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo()
	{
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		return vulkanInfoStore.pipelineVertexInputStateCreateInfo;
	}

	inline VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo(
		const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
		const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions
	)
	{
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(
			vertexBindingDescriptions.size());
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexBindingDescriptions.
			data();
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(
			vertexAttributeDescriptions.size());
		vulkanInfoStore.pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.
			data();
		return vulkanInfoStore.pipelineVertexInputStateCreateInfo;
	}

	inline VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology topology,
		VkPipelineInputAssemblyStateCreateFlags flags,
		VkBool32 primitiveRestartEnable)
	{
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
		pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineInputAssemblyStateCreateInfo.topology = topology;
		pipelineInputAssemblyStateCreateInfo.flags = flags;
		pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
		return pipelineInputAssemblyStateCreateInfo;
	}

	inline VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace,
		VkPipelineRasterizationStateCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.cullMode = cullMode;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.frontFace = frontFace;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.flags = flags;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
		vulkanInfoStore.pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
		return vulkanInfoStore.pipelineRasterizationStateCreateInfo;
	}

	inline VkPipelineColorBlendAttachmentState CreatePipelineColorBlendAttachmentState(
		VkColorComponentFlags colorWriteMask,
		VkBool32 blendEnable)
	{
		vulkanInfoStore.pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
		vulkanInfoStore.pipelineColorBlendAttachmentState.blendEnable = blendEnable;
		return vulkanInfoStore.pipelineColorBlendAttachmentState;
	}

	inline VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo(
		uint32_t attachmentCount,
		const VkPipelineColorBlendAttachmentState* pAttachments)
	{
		vulkanInfoStore.pipelineColorBlendStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineColorBlendStateCreateInfo.attachmentCount = attachmentCount;
		vulkanInfoStore.pipelineColorBlendStateCreateInfo.pAttachments = pAttachments;
		return vulkanInfoStore.pipelineColorBlendStateCreateInfo;
	}

	inline VkPipelineDepthStencilStateCreateInfo CreatePipelineDepthStencilStateCreateInfo(
		VkBool32 depthTestEnable,
		VkBool32 depthWriteEnable,
		VkCompareOp depthCompareOp)
	{
		vulkanInfoStore.pipelineDepthStencilStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
		vulkanInfoStore.pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
		vulkanInfoStore.pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
		vulkanInfoStore.pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
		return vulkanInfoStore.pipelineDepthStencilStateCreateInfo;
	}

	inline VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo(
		uint32_t viewportCount,
		uint32_t scissorCount,
		VkPipelineViewportStateCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineViewportStateCreateInfo.viewportCount = viewportCount;
		vulkanInfoStore.pipelineViewportStateCreateInfo.scissorCount = scissorCount;
		vulkanInfoStore.pipelineViewportStateCreateInfo.flags = flags;
		return vulkanInfoStore.pipelineViewportStateCreateInfo;
	}

	inline VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits rasterizationSamples,
		VkPipelineMultisampleStateCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineMultisampleStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
		vulkanInfoStore.pipelineMultisampleStateCreateInfo.flags = flags;
		return vulkanInfoStore.pipelineMultisampleStateCreateInfo;
	}

	inline VkPipelineDynamicStateCreateInfo CreatePipelineDynamicStateCreateInfo(
		const VkDynamicState* pDynamicStates,
		uint32_t dynamicStateCount,
		VkPipelineDynamicStateCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates;
		vulkanInfoStore.pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
		vulkanInfoStore.pipelineDynamicStateCreateInfo.flags = flags;
		return vulkanInfoStore.pipelineDynamicStateCreateInfo;
	}

	inline VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
		const std::vector<VkDynamicState>& pDynamicStates,
		VkPipelineDynamicStateCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineDynamicStateCreateInfo.pDynamicStates = pDynamicStates.data();
		vulkanInfoStore.pipelineDynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(pDynamicStates.size());
		vulkanInfoStore.pipelineDynamicStateCreateInfo.flags = flags;
		return vulkanInfoStore.pipelineDynamicStateCreateInfo;
	}

	inline VkPipelineTessellationStateCreateInfo CreatePipelineTessellationStateCreateInfo(uint32_t patchControlPoints)
	{
		vulkanInfoStore.pipelineTessellationStateCreateInfo.sType =
			VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		vulkanInfoStore.pipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
		return vulkanInfoStore.pipelineTessellationStateCreateInfo;
	}

	inline VkGraphicsPipelineCreateInfo CreatePipelineCreateInfo(
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		VkPipelineCreateFlags flags = 0)
	{
		vulkanInfoStore.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		vulkanInfoStore.pipelineCreateInfo.layout = layout;
		vulkanInfoStore.pipelineCreateInfo.renderPass = renderPass;
		vulkanInfoStore.pipelineCreateInfo.flags = flags;
		vulkanInfoStore.pipelineCreateInfo.basePipelineIndex = -1;
		vulkanInfoStore.pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		return vulkanInfoStore.pipelineCreateInfo;
	}

	inline VkGraphicsPipelineCreateInfo CreatePipelineCreateInfo()
	{
		vulkanInfoStore.pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		vulkanInfoStore.pipelineCreateInfo.basePipelineIndex = -1;
		vulkanInfoStore.pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		return vulkanInfoStore.pipelineCreateInfo;
	}

	inline VkComputePipelineCreateInfo CreateComputePipelineCreateInfo(
		VkPipelineLayout layout,
		VkPipelineCreateFlags flags = 0)
	{
		vulkanInfoStore.computePipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		vulkanInfoStore.computePipelineCreateInfo.layout = layout;
		vulkanInfoStore.computePipelineCreateInfo.flags = flags;
		return vulkanInfoStore.computePipelineCreateInfo;
	}

	inline VkPushConstantRange CreatePushConstantRange(
		VkShaderStageFlags stageFlags,
		uint32_t size,
		uint32_t offset)
	{
		vulkanInfoStore.pushConstantRange.stageFlags = stageFlags;
		vulkanInfoStore.pushConstantRange.offset = offset;
		vulkanInfoStore.pushConstantRange.size = size;
		return vulkanInfoStore.pushConstantRange;
	}

	inline VkBindSparseInfo CreateBindSparseInfo()
	{
		vulkanInfoStore.bindSparseInfo.sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO;
		return vulkanInfoStore.bindSparseInfo;
	}

	/** @brief Initialize a map entry for a shader specialization constant */
	inline VkSpecializationMapEntry CreateSpecializationMapEntry(uint32_t constantID, uint32_t offset, size_t size)
	{
		vulkanInfoStore.specializationMapEntry.constantID = constantID;
		vulkanInfoStore.specializationMapEntry.offset = offset;
		vulkanInfoStore.specializationMapEntry.size = size;
		return vulkanInfoStore.specializationMapEntry;
	}

	/** @brief Initialize a specialization constant info structure to pass to a shader stage */
	inline VkSpecializationInfo SpecializationInfo(uint32_t mapEntryCount, const VkSpecializationMapEntry* mapEntries,
	                                               size_t dataSize, const void* data)
	{
		vulkanInfoStore.specializationInfo.mapEntryCount = mapEntryCount;
		vulkanInfoStore.specializationInfo.pMapEntries = mapEntries;
		vulkanInfoStore.specializationInfo.dataSize = dataSize;
		vulkanInfoStore.specializationInfo.pData = data;
		return vulkanInfoStore.specializationInfo;
	}

	/** @brief Initialize a specialization constant info structure to pass to a shader stage */
	inline VkSpecializationInfo specializationInfo(const std::vector<VkSpecializationMapEntry>& mapEntries,
	                                               size_t dataSize, const void* data)
	{
		vulkanInfoStore.specializationInfo.mapEntryCount = static_cast<uint32_t>(mapEntries.size());
		vulkanInfoStore.specializationInfo.pMapEntries = mapEntries.data();
		vulkanInfoStore.specializationInfo.dataSize = dataSize;
		vulkanInfoStore.specializationInfo.pData = data;
		return vulkanInfoStore.specializationInfo;
	}

	// Ray tracing related
	inline VkAccelerationStructureGeometryKHR CreateAccelerationStructureGeometryKHR()
	{
		vulkanInfoStore.accelerationStructureGeometryKHR.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		return vulkanInfoStore.accelerationStructureGeometryKHR;
	}

	inline VkAccelerationStructureBuildGeometryInfoKHR CreateAccelerationStructureBuildGeometryInfoKHR()
	{
		vulkanInfoStore.accelerationStructureBuildGeometryInfoKHR.sType =
			VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		return vulkanInfoStore.accelerationStructureBuildGeometryInfoKHR;
	}

	inline VkAccelerationStructureBuildSizesInfoKHR CreateAccelerationStructureBuildSizesInfoKHR()
	{
		vulkanInfoStore.accelerationStructureBuildSizesInfoKHR.sType =
			VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
		return vulkanInfoStore.accelerationStructureBuildSizesInfoKHR;
	}

	inline VkRayTracingShaderGroupCreateInfoKHR CreateRayTracingShaderGroupCreateInfoKHR()
	{
		vulkanInfoStore.rayTracingShaderGroupCreateInfoKHR.sType =
			VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		return vulkanInfoStore.rayTracingShaderGroupCreateInfoKHR;
	}

	inline VkRayTracingPipelineCreateInfoKHR CreateRayTracingPipelineCreateInfoKHR()
	{
		vulkanInfoStore.rayTracingPipelineCreateInfoKHR.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
		return vulkanInfoStore.rayTracingPipelineCreateInfoKHR;
	}

	inline VkWriteDescriptorSetAccelerationStructureKHR CreateWriteDescriptorSetAccelerationStructureKHR()
	{
		vulkanInfoStore.writeDescriptorSetAccelerationStructureKHR.sType =
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
		return vulkanInfoStore.writeDescriptorSetAccelerationStructureKHR;
	}

	inline VkApplicationInfo CreateAppInfo()
	{
		vulkanInfoStore.appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vulkanInfoStore.appInfo.pApplicationName = "LittleVulkanEngine App";
		vulkanInfoStore.appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		vulkanInfoStore.appInfo.pEngineName = "No Engine";
		vulkanInfoStore.appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		vulkanInfoStore.appInfo.apiVersion = VK_API_VERSION_1_0;
		return vulkanInfoStore.appInfo;
	}

	// TODO hacking instance fix
	inline void CreateInstance(VkInstance* instance)
	{
		vulkanInfoStore.createInstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vulkanInfoStore.createInstanceInfo.pApplicationInfo = &vulkanInfoStore.appInfo;

		vulkanInfoStore.extensions = GetRequiredExtensions();
		vulkanInfoStore.createInstanceInfo.enabledExtensionCount = static_cast<uint32_t>(vulkanInfoStore.extensions.
			size());
		vulkanInfoStore.createInstanceInfo.ppEnabledExtensionNames = vulkanInfoStore.extensions.data();


		if (ENABLE_VALIDATION_LAYERS)
		{
			vulkanInfoStore.createInstanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			vulkanInfoStore.createInstanceInfo.ppEnabledLayerNames = validationLayers.data();

			PopulateDebugMessengerCreateInfo(vulkanInfoStore.debugCreateInfo);

			vulkanInfoStore.createInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&vulkanInfoStore.
				debugCreateInfo;
		}
		else
		{
			vulkanInfoStore.createInstanceInfo.enabledLayerCount = 0;
			vulkanInfoStore.createInstanceInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&vulkanInfoStore.createInstanceInfo, nullptr, instance) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create instance!");
		}
		vkInstance = *instance;
	}
}
