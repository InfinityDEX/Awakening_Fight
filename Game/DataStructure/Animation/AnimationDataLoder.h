#pragma once
#include <SimpleMath.h>
#include <map>

using namespace DirectX;
using namespace DirectX::SimpleMath;

// アニメーションデータ
struct AnimationData
{
	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	// スプリット
	Vector2 split;
};

// アニメーションデータのかたまり
using AnimationDataRepository = std::map<int, AnimationData>;

// アニメーションデータの読み込み
AnimationDataRepository LoadAnimationData(char * AnimationDataPath);