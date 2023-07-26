#include "pch.h"
#include "GagePS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\Locator\Locator.h>

// シェーダ
Microsoft::WRL::ComPtr<ID3D11PixelShader> GagePS::m_shader;

// コンストラクタ
GagePS::GagePS()
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

	/// ジオメトリシェーダを作成する
	// ジオメトリシェーダのファイル読み込み
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Gage/GagePS.cso");
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
}

// 定数バッファの更新処理
void GagePS::UpdateCBuffer()
{
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// 定数バッファの配列取得
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//定数バッファをシェーダに渡す
	context->PSSetConstantBuffers(0, 1, cb);
	// ジオメトリシェーダのセット
	context->PSSetShader(m_shader.Get(), nullptr, 0);
}

// シェーダーの取得
ID3D11PixelShader * GagePS::GetShader()
{
	return m_shader.Get();
}

// 定数バッファの取得
GagePS::ConstBuffer & GagePS::GetCBuffer()
{
	return m_cbuff;
}
