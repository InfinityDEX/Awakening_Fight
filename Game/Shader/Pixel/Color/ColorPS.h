#pragma once
#include <Game\Shader\Pixel\Interface\PSInterface.h>
#include <SimpleMath.h>

// エイリアス宣言
using namespace DirectX;
using namespace DirectX::SimpleMath;

// 基本となるジオメトリシェーダ(正方形に表示)
class ColorPS final : public IPShader
{
public:
	// 定数バッファの構造体
	struct ConstBuffer
	{
		Matrix		matWorld;
		Matrix		matView;
		Matrix		matProj;
		Vector4		Time;
		Vector4		Color;
	};
	// コンストラクタ
	ColorPS();
	// 定数バッファの更新
	void UpdateCBuffer() override;
	// シェーダーの取得
	ID3D11PixelShader* GetShader() override;
	// 定数バッファ用のデータ取得
	ColorPS::ConstBuffer& GetCBuffer();
private:
	// バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// 定数バッファ
	ColorPS::ConstBuffer m_cbuff;
	// シェーダ
	static Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;
};

