#pragma once
#include <Game\Shader\Vertex\Interface\VSInterface.h>
#include <SimpleMath.h>
#include <vector>

// エイリアス宣言
using namespace DirectX;
using namespace DirectX::SimpleMath;

class DefaultVS final : public IVShader
{
	public:
		// 定数バッファの構造体
		struct ConstBuffer
		{
			Matrix		matWorld;
			Matrix		matView;
			Matrix		matProj;
			Vector4		Time;
		};
		// コンストラクタ
		DefaultVS();
		// 定数バッファの更新
		void UpdateCBuffer() override;
		// シェーダーの取得
		ID3D11VertexShader* GetShader() override;
		// インプットレイアウトを入力アセンブラーステージにセット
		void IASetInputLayout() override;
		// 定数バッファ用のデータ取得
		DefaultVS::ConstBuffer& GetCBuffer();
	private:
		// バッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
		// 定数バッファ
		DefaultVS::ConstBuffer m_cbuff;
		// 入力レイアウトディスク
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
		// 入力レイアウト
		static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
		// シェーダ
		static Microsoft::WRL::ComPtr<ID3D11VertexShader> m_shader;
};