#include "engine/core/util/PathUtil.h"
#include "base/Renderer.h"
#include "gles_render_base.h"
#include "gles_mapping.h"
#include "gles_frame_buffer.h"
#include "gles_texture_2d.h"
#include "gles_texture_render.h"

namespace Echo
{
    GLESFrameBufferOffScreen::GLESFrameBufferOffScreen(ui32 width, ui32 height)
		: FrameBufferOffScreen(width, height)
        , m_fbo(0)
	{
        OGLESDebug(glGenFramebuffers(1, &m_fbo));
	}

    GLESFrameBufferOffScreen::~GLESFrameBufferOffScreen()
	{
		OGLESDebug(glDeleteFramebuffers(1, &m_fbo));
	}

    // attach render view
    void GLESFrameBufferOffScreen::attach(Attachment attachment, TextureRender* renderView)
    {
        GLESTextureRender* texture = dynamic_cast<GLESTextureRender*>(renderView);
        GLenum esAttachment = attachment == Attachment::DepthStencil ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0;

        OGLESDebug(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
        OGLESDebug(glFramebufferTexture2D(GL_FRAMEBUFFER, esAttachment, GL_TEXTURE_2D, texture->m_glesTexture, 0));
        OGLESDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        m_views[(ui8)attachment] = renderView;
    }

	bool GLESFrameBufferOffScreen::begin(bool isClearColor, const Color& bgColor, bool isClearDepth, float depthValue, bool isClearStencil, ui8 stencilValue)
	{
		// bind frame buffer
		OGLESDebug(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
		OGLESDebug(glViewport(0, 0, m_width, m_height));

		// clear
		clear( isClearColor, bgColor, isClearDepth, depthValue, isClearStencil, stencilValue );

		return true;
	}

	bool GLESFrameBufferOffScreen::end()
	{
		return true;
	}

	void GLESFrameBufferOffScreen::clear(bool clear_color, const Color& color, bool clear_depth, float depth_value, bool clear_stencil, ui8 stencil_value)
	{
		GLbitfield mask = 0;
		if (clear_color)
		{
			OGLESDebug(glClearColor(color.r, color.g, color.b, color.a));
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if (clear_depth)
		{
			OGLESDebug(glClearDepthf(depth_value));
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		OGLESDebug(glDepthMask(clear_depth));

		if (clear_stencil)
		{
			OGLESDebug(glClearStencil(stencil_value));
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		OGLESDebug(glStencilMask(clear_stencil));

		if (mask != 0)
		{
			OGLESDebug(glClear(mask));
		}
	}

	void GLESFrameBufferOffScreen::onSize( ui32 width, ui32 height )
	{
        m_width = width;
        m_height = height;

        for (TextureRender* colorView : m_views)
        {
            if (colorView)
                colorView->onSize(width, height);
        }
	}

	GLESFramebufferWindow::GLESFramebufferWindow()
		: FrameBufferWindow()
	{
	}

	GLESFramebufferWindow::~GLESFramebufferWindow()
	{
	}

	bool GLESFramebufferWindow::begin(bool clearColor, const Color& backgroundColor, bool clearDepth, float depthValue, bool clearStencil, ui8 stencilValue)
	{
		// bind frame buffer
#if defined(ECHO_PLATFORM_WINDOWS) || defined(ECHO_PLATFORM_ANDROID)
		OGLESDebug(glBindFramebuffer(GL_FRAMEBUFFER, 0));
#endif

		ui32 width = Renderer::instance()->getWindowWidth();
		ui32 height = Renderer::instance()->getWindowHeight();
		OGLESDebug(glViewport(0, 0, width, height));

		// clear
		GLESFrameBufferOffScreen::clear(clearColor, backgroundColor, clearDepth, depthValue, clearStencil, stencilValue);

		return true;
	}

	bool GLESFramebufferWindow::end()
	{
		return true;
	}

	void GLESFramebufferWindow::onSize(ui32 width, ui32 height)
	{
	}
}
