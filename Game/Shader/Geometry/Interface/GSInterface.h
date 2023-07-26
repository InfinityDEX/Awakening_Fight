#pragma once
#include <wrl\client.h>
#include <d3d11.h>

// ジオメトリシェーダのインターフェース	
class IGShader
{
public:
	// 定数バッファの更新
	virtual void UpdateCBuffer() = 0;
	// シェーダーの取得
	virtual ID3D11GeometryShader* GetShader() = 0;
};