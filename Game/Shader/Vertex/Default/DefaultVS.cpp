#include "pch.h"
#include "DefaultVS.h"
#include <Game\3rdParty\BinaryFile\BinaryFile.h>
#include <Game\3rdParty\DeviceResources\DeviceResources.h>
#include <Game\Locator\Locator.h>

// 入力レイアウト
Microsoft::WRL::ComPtr<ID3D11InputLayout> DefaultVS::m_inputLayout;
// シェーダ
Microsoft::WRL::ComPtr<ID3D11VertexShader> DefaultVS::m_shader;
// インプットレイアウト
const std::vector<D3D11_INPUT_ELEMENT_DESC> DefaultVS::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
DefaultVS::DefaultVS()
{
	// デバイスを取得する
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
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/Default/DefaultVS.cso");
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_shader.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// インプットレイアウト作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
}

// 定数バッファの更新
void DefaultVS::UpdateCBuffer()
{
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();
	// 定数バッファの配列取得
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &m_cbuff, 0, 0);
	//定数バッファをシェーダに渡す
	context->VSSetConstantBuffers(0, 1, cb);
	// 頂点シェーダのセット
	context->VSSetShader(m_shader.Get(), nullptr, 0);
}

// シェーダーの取得
ID3D11VertexShader * DefaultVS::GetShader()
{
	return m_shader.Get();
}

// インプットレイアウトを入力アセンブラーステージにセット
void DefaultVS::IASetInputLayout()
{
	// デバイスコンテキスト取得
	auto context = Locator::GetDeviceResources()->GetD3DDeviceContext();

	// 入力レイアウトを反映
	context->IASetInputLayout(m_inputLayout.Get());
}

DefaultVS::ConstBuffer& DefaultVS::GetCBuffer()
{
	return m_cbuff;
}
