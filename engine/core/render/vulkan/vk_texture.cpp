#include "vk_texture.h"
#include "vk_mapping.h"
#include "vk_renderer.h"

namespace Echo
{
    VKTexture2D::~VKTexture2D()
    {

    }

    VKTextureRender::VKTextureRender(const String& name)
        : TextureRender(name)
    {

    }

    VKTextureRender::~VKTextureRender()
    {

    }

    bool VKTextureRender::updateTexture2D(PixelFormat format, TexUsage usage, i32 width, i32 height, void* data, ui32 size)
    {
        m_width = width;
        m_height = height;
        m_usage = usage;
        m_isCompressed = false;
        m_compressType = Texture::CompressType_Unknown;
        m_depth = 1;
        m_pixFmt = format;
        m_numMipmaps = 1;

        createVkImage();

		ui32 pixelsSize = PixelUtil::CalcSurfaceSize(m_width, m_height, m_depth, m_numMipmaps, m_pixFmt);
		Buffer buff(pixelsSize, data, false);
        setVkImageSurfaceData(0, m_pixFmt, m_usage, m_width, m_height, buff);

        return true;
    }

    void VKTextureRender::createVkImage()
    {
		destroyVkImage();

		VkImageCreateInfo imageCreateInfo;
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.flags = 0;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = VKMapping::mapPixelFormat(m_pixFmt);
		imageCreateInfo.extent = { m_width, m_height, m_depth };
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = PixelUtil::IsDepth(m_pixFmt) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = 0;
		imageCreateInfo.pQueueFamilyIndices = nullptr;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		if (VK_SUCCESS == vkCreateImage(VKRenderer::instance()->getVkDevice(), &imageCreateInfo, nullptr, &m_vkImage))
		{
			createVkImageMemory();
			createVkImageView();
		}
    }

	void VKTextureRender::destroyVkImage()
	{
		if (m_vkImage)
		{
			vkDestroyImage(VKRenderer::instance()->getVkDevice(), m_vkImage, nullptr);
			m_vkImage = VK_NULL_HANDLE;
		}
	}

    void VKTextureRender::createVkImageMemory()
	{
		destroyVkImageMemory();

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(VKRenderer::instance()->getVkDevice(), m_vkImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo;
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VKRenderer::instance()->findVkMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VKDebug(vkAllocateMemory(VKRenderer::instance()->getVkDevice(), &allocInfo, nullptr, &m_vkImageMemory));
		VKDebug(vkBindImageMemory(VKRenderer::instance()->getVkDevice(), m_vkImage, m_vkImageMemory, 0));
    }

	void VKTextureRender::destroyVkImageMemory()
	{
		if (m_vkImageMemory)
		{
			vkFreeMemory(VKRenderer::instance()->getVkDevice(), m_vkImageMemory, nullptr);
			m_vkImageMemory = VK_NULL_HANDLE;
		}
	}

    void VKTextureRender::createVkImageView()
    {
		destroyVkImageView();

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.image = m_vkImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VKMapping::mapPixelFormat(m_pixFmt);
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = PixelUtil::IsDepth(m_pixFmt) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VKDebug(vkCreateImageView(VKRenderer::instance()->getVkDevice(), &createInfo, nullptr, &m_vkImageView));
    }

	void VKTextureRender::destroyVkImageView()
	{
		if (m_vkImageView)
		{
			vkDestroyImageView(VKRenderer::instance()->getVkDevice(), m_vkImageView, nullptr);
			m_vkImageView = VK_NULL_HANDLE;
		}
	}

    void VKTextureRender::setVkImageSurfaceData(int level, PixelFormat pixFmt, Dword usage, ui32 width, ui32 height, const Buffer& buff)
    {

    }
}
