#pragma once

#include "engine/core/geom/AABB.h"
#include "MeshVertexData.h"

namespace Echo
{
	/**
	* Mesh 2013-11-6
	*/
	class GPUBuffer;
	class Mesh
	{
	public:
		enum TopologyType
		{
			// A list of points, 1 vertex per point
			TT_POINTLIST,
			// A list of lines, 2 vertices per line
			TT_LINELIST,
			// A strip of connected lines, 1 vertex per line plus 1 start vertex
			TT_LINESTRIP,
			// A list of triangles, 3 vertices per triangle
			TT_TRIANGLELIST,
			// A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that 
			TT_TRIANGLESTRIP,
		};

	public:
		~Mesh();

		// create
		static Mesh* create(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);

		// name
		const String& getName() const { return m_name; }

		// topology type
		TopologyType getTopologyType() { return m_topologyType; }

		// vertex data
		MeshVertexData& getVertexData() { return m_vertData; }

		// vertex stride
		ui32 getVertexStride() const { return m_vertData.getVertexStride(); }

		// vertex count
		ui32 getVertexCount() const { return m_vertData.getVertexCount(); }

		// vertex data ptr
		const MeshVertexData& getVertices() const { return m_vertData; }

		// get buffer object
		GPUBuffer* getVertexBuffer() const;
		GPUBuffer* getIndexBuffer() const;

		// get face count
		ui32 getFaceCount() const;

		// get index count
		ui32 getIndexCount() const;

		// get index stride
		ui32 getIndexStride() const;

		// start vertex and index
		void setStartVertex(ui32 startVert) { m_startVert = startVert; }
		void setStartIndex(ui32 startIdx) { m_startIdx = startIdx; }
		ui32 getStartVertex() const { return m_startVert; }
		ui32 getStartIndex() const { return m_startIdx; }

		// 获取索引数据
		Word* getIndices() const;

		// is valid
		bool isValid() const { return getFaceCount() > 0; }

		// 是否为蒙皮
		bool isSkin() const { return isVertexUsage(VS_BLENDINDICES); }

		// 判断顶点格式中是否含有指定类型的数据
		bool isVertexUsage(VertexSemantic semantic) const { return m_vertData.isVertexUsage(semantic); }

		// 获取顶点格式
		const VertexElementList& getVertexElements() const;

		// 生成切线数据
		void generateTangentData(bool useNormalMap);

		// 获取本地包围盒
		const AABB& getLocalBox() const { return m_box; }

		// 获取受影响的骨骼数量
		ui32 getBoneNum() const { return static_cast<ui32>(m_boneIdxs.size()); }

		// 获取受影响的骨骼索引
		ui32 getBoneIdx(int idx) { return m_boneIdxs[idx]; }

		// set primitive type
		void setTopologyType(TopologyType type) { m_topologyType = type; }

		// update indices data
		void updateIndices(ui32 indicesCount, ui32 indicesStride, const void* indices);

		// update vertex data
		void updateVertexs(const MeshVertexFormat& format, ui32 vertCount, const Byte* vertices, const AABB& box);
		void updateVertexs(const MeshVertexData& vertexData, const AABB& box);

		// clear
		void clear();

	protected:
		Mesh(bool isDynamicVertexBuffer, bool isDynamicIndicesBuffer);

		// get memory usage
		ui32 getMemeoryUsage() const;

		// calc tangent data
		void buildTangentData();

		// build buffer
		bool buildBuffer();

		// build Vertex|Index buffer
		void buildVertexBuffer();
		void buildIndexBuffer();

	protected:
		String						m_name;
		TopologyType				m_topologyType;
		AABB						m_box;
		ui32						m_startVert = 0;
		ui32						m_startIdx = 0;
		ui32						m_idxCount = 0;
		ui32						m_idxStride = 0;
		vector<Byte>::type			m_indices;
		MeshVertexData				m_vertData;
		bool						m_isDynamicVertexBuffer;
		GPUBuffer*					m_vertexBuffer = nullptr;
		bool						m_isDynamicIndicesBuffer;
		GPUBuffer*					m_indexBuffer = nullptr;
		vector<ui32>::type			m_boneIdxs;
	};
}

