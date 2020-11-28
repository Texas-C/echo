#pragma once

#include "base/frame_buffer.h"
#include "gles_render_base.h"

namespace Echo
{
	class GLESFrameBufferOffScreen : public FrameBufferOffScreen
	{
	public:
        GLESFrameBufferOffScreen(ui32 width, ui32 height);
		virtual ~GLESFrameBufferOffScreen();

        // attach render view
        virtual void attach(Attachment attachment, TextureRender* renderView) override;

        // begin render
        virtual bool begin(bool isClearColor, const Color& bgColor, bool isClearDepth, float depthValue, bool isClearStencil, ui8 stencilValue) override;
        virtual bool end() override;

        // on resize
        virtual void onSize(ui32 width, ui32 height) override;

    public:
		// clear render target
		static void clear(bool clear_color, const Color& color, bool clear_depth, float depth_value, bool clear_stencil, ui8 stencil_value);

	private:
		GLuint m_fbo;
	};

	class GLESFramebufferWindow : public FrameBufferWindow
	{
	public:
		GLESFramebufferWindow();
		virtual ~GLESFramebufferWindow();

		// begin render
		virtual bool begin(bool clearColor, const Color& backgroundColor, bool clearDepth, float depthValue, bool clearStencil, ui8 stencilValue) override;
		virtual bool end() override;

		// on resize
		virtual void onSize(ui32 width, ui32 height) override;
	};
}
