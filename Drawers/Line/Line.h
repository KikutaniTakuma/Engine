#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Mat4x4.h"
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Engine/ConstBuffer/ConstBuffer.h"
#include "Engine/ShaderResource/ShaderResourceHeap.h"
#include "Engine/PipelineManager/PipelineManager.h"
#include "Engine/ShaderManager/ShaderManager.h"

class Line {
private:
	struct VertexData {
		Vector4 pos;
		Vector4 color;
	};
private:
	static constexpr uint16_t kVertexNum = 2u;

public:
	Line();
	~Line();

public:
	void Draw(const Mat4x4& viewProjection, const Vector2& start, const Vector2& end, uint32_t color);
	void Draw(const Mat4x4& viewProjection, const Vector3& start, const Vector3& end, uint32_t color);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexView;
	// 頂点バッファマップ
	VertexData* vertexMap;

	ShaderResourceHeap heap;
	
	Shader shader;

	Pipeline* pipline;

	ConstBuffer<Mat4x4> wvpMat;
};