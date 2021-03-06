#include "engine/core/log/Log.h"
#include "base/proxy/render_proxy.h"
#include "base/shader_program.h"
#include "base/renderer.h"
#include "base/pipeline/render_pipeline.h"
#include "base/material.h"
#include "base/mesh/mesh.h"
#include "engine/core/scene/render_node.h"

namespace Echo
{
	RenderProxy::RenderProxy(int identifier)
		: m_identifier(identifier)
	{
	}

	RenderProxy::~RenderProxy()
	{
	}

	void RenderProxy::subRefCount()
	{
		m_refCount--;
		if (m_refCount <= 0)
		{
			RenderProxy* ptr = this;
			Renderer::instance()->destroyRenderProxies(&ptr, 1);
		}
	}

	RenderProxy* RenderProxy::create(MeshPtr mesh, Material* material, Render* node)
	{
		// bind shader param
		RenderProxy* renderable = Renderer::instance()->createRenderProxy();
		renderable->setMaterial(material);
		renderable->setNode(node);
		renderable->setMesh(mesh);

		return renderable;
	}

	void RenderProxy::submitToRenderQueue()
	{
		if (m_mesh && m_mesh->isValid())
		{
			RenderPipeline::current()->addRenderable(m_material->getRenderStage(), getIdentifier());
		}
	}
}
