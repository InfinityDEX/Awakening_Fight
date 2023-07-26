#include "pch.h"
#include "SpritePS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Game\Locator\Locator.h>

// シェーダ
Microsoft::WRL::ComPtr<ID3D11PixelShader> SpritePS::m_shader;

// コンストラクタ
SpritePS::SpritePS()
{
	auto device = Locator::GetDeviceResources()->GetD3DDevice();
	///　定数バッファを作成する
	// バッファディスク
	D3D11_BUFFER_DESC bd;
	// バッファディスクをクリア
	ZeroMemory(&bd, sizeof(bd));
	// GPUを用いて読み書きする設定
	bd.Usage = D3D11_USAGE_DEFAULT;
	// ConstBuffer構造体のメモリサイズ(Byte単位)でバッファサイズを指定する設定
	bd.ByteWidth = sizeof(ConstBuffer);
	// 定数バッファとしてパイプラインに送る設定
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	// CPUは使用しない設定
	bd.CPUAccessFlags = 0;
	// ディスクをもとにバッファを作成
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	// もしすでにシェーダが作成されていたらこの先は飛ばす
	if (m_shader.Get()) return;

	/// ピクセルシェーダを作成する
	// ピクセルシェーダのファイル読み込み
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Sprite/SpritePS.cso");
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
}

// 定数バッファの更新
void SpritePS::UpdateCBuffer()
{
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// 定数バッファの配列取得
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//定数バッファをシェーダに渡す
	context->PSSetConstantBuffers(0, 1, cb);
	// ピクセルシェーダのセット
	context->PSSetShader(m_shader.Get(), nullptr, 0);
}

// シェーダの取得
ID3D11PixelShader * SpritePS::GetShader()
{
	return m_shader.Get();
}

// 定数バッファの取得
SpritePS::ConstBuffer & SpritePS::GetCBuffer()
{
	return m_cbuff;
}
