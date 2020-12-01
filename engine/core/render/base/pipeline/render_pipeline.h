#pragma once

#include "engine/core/scene/node.h"
#include "../frame_buffer.h"
#include "../renderer.h"

namespace Echo
{
	class RenderPass;
	class RenderPipeline : public Res
	{
		ECHO_RES(RenderPipeline, Res, ".pipeline", Res::create<RenderPipeline>, RenderPipeline::load);

	public:
		// template
		enum Template
		{
			Empty,
			Default,
		};

	public:
		RenderPipeline();
		RenderPipeline(const ResourcePath& path);
		virtual ~RenderPipeline();

		// add render able
		void addRenderable(const String& name, RenderableID id);

		// on Resize
		void onSize(ui32 width, ui32 height);

		// process
		void render();

	public:
		// current
		static ResRef<RenderPipeline> current();
		static void setCurrent(const ResourcePath& path);

		// set src
		void setSrc(Template type);
		void setSrc(const String& src);

	public:
		// stages
		vector<RenderPass*>::type& getRenderStages() { return m_stages; }

		// stage operate
		void addStage(RenderPass* stage, ui32 position=-1);
		void deleteStage(RenderPass* stage);

	public:
		// load and save
		static Res* load(const ResourcePath& path);
		virtual void save() override;

	private:
		// parse
		void parseXml();

	private:
		String						m_srcData;
		bool						m_isParsed = false;
		vector<RenderPass*>::type	m_stages;
	};
	typedef ResRef<RenderPipeline> RenderPipelinePtr;
}
