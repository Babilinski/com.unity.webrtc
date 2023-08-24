#include "pch.h"

#include "GraphicsDevice/Vulkan/VulkanUtility.h"
#include "VulkanTexture2D.h"

namespace unity
{
namespace webrtc
{

    //---------------------------------------------------------------------------------------------------------------------

    VulkanTexture2D::VulkanTexture2D(const uint32_t w, const uint32_t h)
        : ITexture2D(w, h)
        , m_textureImage(nullptr)
        , m_textureImageMemory(nullptr)
        , m_textureImageMemorySize(0)
        , m_device(nullptr)
        , m_textureFormat(VK_FORMAT_B8G8R8A8_UNORM)
    {
    }

    VulkanTexture2D::~VulkanTexture2D() { Shutdown(); }

    void VulkanTexture2D::Shutdown()
    {
        if (m_textureImage)
            vkDestroyImage(m_device, m_textureImage, m_allocator);
        if (m_textureImageMemory)
            vkFreeMemory(m_device, m_textureImageMemory, m_allocator);

        m_textureImage = nullptr;
        m_textureImageMemory = nullptr;
        m_textureImageMemorySize = 0;
        m_device = nullptr;
    }

    bool
    VulkanTexture2D::Init(const VkPhysicalDevice physicalDevice, const VkDevice device)
    {
        m_physicalDevice = physicalDevice;
        m_device = device;

        const bool EXPORT_HANDLE = true;
        VkResult result = VulkanUtility::CreateImage(
            physicalDevice,
            device,
            m_allocator,
            m_width,
            m_height,
            VK_IMAGE_TILING_OPTIMAL,
            VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            m_textureFormat,
            &m_unityVulkanImage,
            EXPORT_HANDLE);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        m_textureImage = m_unityVulkanImage.image;
        m_textureImageMemory = m_unityVulkanImage.memory.memory;
        m_textureImageMemorySize = m_unityVulkanImage.memory.size;

        return true;
    }

    //---------------------------------------------------------------------------------------------------------------------
    bool VulkanTexture2D::InitCpuRead(
        const VkPhysicalDevice physicalDevice, const VkDevice device)
    {
        m_physicalDevice = physicalDevice;
        m_device = device;

        const bool EXPORT_HANDLE = false;
        VkResult result = VulkanUtility::CreateImage(
            physicalDevice,
            device,
            m_allocator,
            m_width,
            m_height,
            VK_IMAGE_TILING_LINEAR,
            VK_IMAGE_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_textureFormat,
            &m_unityVulkanImage,
            EXPORT_HANDLE);

        if (result != VK_SUCCESS)
        {
            return false;
        }

        m_textureImage = m_unityVulkanImage.image;
        m_textureImageMemory = m_unityVulkanImage.memory.memory;
        m_textureImageMemorySize = m_unityVulkanImage.memory.size;
        return true;
    }
} // end namespace webrtc
} // end namespace unity
