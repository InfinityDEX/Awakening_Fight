#include "pch.h"
#include "Spark.h"
#include <Game\3rdParty\BinaryFile.h>

Spark::Spark(Vector3 position, Vector3 direction, float range, int sparkNum, float speed, float length)
	: AnimationEffect(999, Vector2(1, 1), position, Matrix::Identity, L"Resources\\Sprite\\Particle.png")
	, m_sparkNum(sparkNum)
	, m_range(range)
	, m_direction(direction)
	, m_speed(speed)
	, m_length(length * 10)
	, m_interval(2)
{
	// 頂点インデックスを初期化
	m_vertex.clear();
	// パーティクルを一つ生成
	MakeSpark();
	// カウンターの値をセット
	m_counter = m_interval;
	// エフェクトの大きさを小さくする
	m_scale = Vector3(0.1f);
}

void Spark::Create(DX::DeviceResources * deviceResources)
{
	AnimationEffect::Create(deviceResources);

	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();
	
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/Spark/SparkGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/Spark/SparkPS.cso");

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
}

void Spark::Update(DX::StepTimer timer)
{
	// 生成カウンターが0でかつパーティクルの残量がまだ残っていたら
	if (m_counter == 0 && m_sparkNum > 0)
	{
		// パーティクルを一つ生成
		MakeSpark();
		// カウンターの値をセット
		m_counter = m_interval;
		// 粒子のカウンターを一つ減らす
		m_sparkNum--;
	}

	// 消す配列を記憶
	std::vector<int> m_deleteMemory;
	// 全てのバーテックスを回る
	for (int i = 0; i < m_vertex.size(); i++)
	{
		m_vertex[i].position = m_vertex[i].position + m_velocities[i];

		if (
			Vector3(m_vertex[i].position).Length() > m_length && // 発生した場所から指定した長さより離れていて、
			!(m_sparkNum > 0 && m_vertex.size() == 1)) // かつ生成カウンタが0で頂点インデックスが無くなりそうでなければ
		{
			// 削除する配列として記憶
			m_deleteMemory.push_back(i);
		}
	}

	// 削除する値があれば
	if (m_deleteMemory.size() > 0)
	{
		// 削除配列をソート(昇順)
		std::sort(m_deleteMemory.begin(), m_deleteMemory.end());
		// 順番に削除する(リバースなのは下のほうから消さなければいけない為)
		for (auto iter = m_deleteMemory.rbegin(); iter < m_deleteMemory.rend(); iter++)
		{
			// 生成するパーティクルが無く、描画していた頂点もなくなったら
			if (m_sparkNum == 0 && m_vertex.size() <= m_deleteMemory.size())
			{
				// 頂点インデックスは削除せずエフェクトのキルスイッチをtrueにして処理を終わる
				m_killSwitch = true;
				return;
			}
			// 頂点とそれに対応した移動ベクトルを削除
			m_vertex.erase(m_vertex.begin() + (*iter));
			m_velocities.erase(m_velocities.begin() + (*iter));
		}
	}

	// コンストバッファのタイマーの値を設定する
	m_cbuff.Time = Vector4(timer.GetTotalSeconds(), sin(timer.GetTotalSeconds()), timer.GetElapsedSeconds(), 0);

	// 移動限界距離を設定する
	m_cbuff.Length.x = m_length;

	// カウンターを一つ減らす
	m_counter--;
}

void Spark::Setting(DX::DeviceResources * deviceResourcese, Matrix view, Matrix proj)
{
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();
	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	ID3D11Buffer* cb[1] = { GetD3DBuffer() };
	auto device = deviceResourcese->GetD3DDevice();
	auto context = deviceResourcese->GetD3DDeviceContext();

	m_cbuff.matWorld = (Matrix::CreateScale(m_scale) * m_rotation * Matrix::CreateTranslation(m_position)).Transpose();
	m_cbuff.matView = view.Transpose();
	m_cbuff.matProj = proj.Transpose();

	//定数バッファの内容更新
	context->UpdateSubresource(GetD3DBuffer(), 0, NULL, &m_cbuff, 0, 0);
	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファは参照のみ
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	//// カリングは反時計回り
	//context->RSSetState(m_states->CullCounterClockwise());
	// 両面カリング
	context->RSSetState(m_states->CullNone());
	// テクスチャをセット
	context->PSSetShaderResources(0, 1, GetPPTexture());
	//定数バッファをシェーダに渡す（とりあえずPSは要らないのでコメントアウト）
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	// 両面カリング
	context->PSSetSamplers(0, 1, sampler);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}

void Spark::MakeSpark()
{
	m_vertex.push_back(VertexPositionColorTexture(Vector3::Zero, Vector4::Zero, Vector2::Zero));

	float radius = static_cast<float>(rand() % static_cast<int>(m_range * 100)) / 100;

	radius -= m_range / 2;
	Vector3 vec;

	vec.x = cos(radius) * m_direction.x - sin(radius) * m_direction.y;
	vec.y = sin(radius) * m_direction.x + cos(radius) * m_direction.y;
	m_velocities.push_back(vec * m_speed);
}
