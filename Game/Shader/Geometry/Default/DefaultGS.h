#pragma once
#include <Game\Shader\Geometry\Interface\GSInterface.h>
#include <SimpleMath.h>

// エイリアス宣言
using namespace DirectX;
using namespace DirectX::SimpleMath;

// 基本となるジオメトリシェーダ(正方形に表示)
class DefaultGS final : public IGShader
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
	DefaultGS();
	// 定数バッファの更新
	void UpdateCBuffer() override;
	// シェーダーの取得
	ID3D11GeometryShader* GetShader() override;
	// 定数バッファ用のデータ取得
	DefaultGS::ConstBuffer& GetCBuffer();
private:
	// バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// 定数バッファ
	DefaultGS::ConstBuffer m_cbuff;
	// シェーダ
	static Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_shader;
};

