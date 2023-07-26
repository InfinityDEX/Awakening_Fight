#pragma once
#ifndef SCENEMANAGER
#define SCENEMANAGER
#include <Game\3rdParty\Timer\StepTimer.h>
#include <memory>
#include <vector>
#include <deque>

// シーンインターフェース
class IScene;
// Sceneのユニークポインタ
using ScenePtr = std::unique_ptr<IScene>;
// ScenePtr の双方向コンテナ
using SceneStack = std::deque<ScenePtr>;

// シーンマネージャ
class SceneManager final
{
// 関数
public:
	// コンストラクタ
	SceneManager();
	// デストラクタ
	~SceneManager();
	// 更新処理
	void Update(DX::StepTimer const& timer);
	// 描画処理
	void Render();
	// 未登録のシーンのリクエスト(リセット)
	void RequestScene_Reset(std::unique_ptr<IScene> scene);
	// 未登録のシーンのリクエスト(追加)
	void RequestScene_Add(std::unique_ptr<IScene> scene);
	// 実行中のシーンの一部を削除
	void PopScene(int count);
private:
	// シーンをステートに追加
	void AddScene();
	std::vector<std::unique_ptr<IScene>> m_addScenes;
	SceneStack m_scenes;
	// 削除するシーンの数
	int	m_count;
};

#endif

