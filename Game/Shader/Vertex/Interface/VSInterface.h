#pragma once
#include <wrl\client.h>
#include <d3d11.h>

// 頂点シェーダのインターフェース	
class IVShader
{
	public:
		// 定数バッファの更新
		virtual void UpdateCBuffer() = 0;
		// シェーダーの取得
		virtual ID3D11VertexShader* GetShader() = 0;
		// インプットレイアウトを入力アセンブラーステージにセット
		virtual void IASetInputLayout() = 0;
};