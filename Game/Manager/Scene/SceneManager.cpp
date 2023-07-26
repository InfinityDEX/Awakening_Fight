#include "pch.h"
#include "SceneManager.h"
#include <Game\Scene\Interface\IScene.h>
#include <Game\Locator\Locator.h>

// コンストラクタ
SceneManager::SceneManager()
{
}

// デストラクタ
SceneManager::~SceneManager()
{
}

// 更新処理
void SceneManager::Update(DX::StepTimer const& timer)
{
	// 前に表示されているシーン(back)をカウントの数だけ消去する
	if (m_count > 0)
	{
		for (int i = 0; i < m_count; i++)
		{
			m_scenes.back()->Finalize();
			m_scenes.pop_back();
		}
		m_count = 0;
	}

	// シーン追加
	if (static_cast<int>(m_addScenes.size()) > 0)
	{
		AddScene();
	}

	// シーンのアップデート(back)
	assert(m_scenes.size() > 0 && "There is no active state."); // シーンが一つも無かったらエラー
	for (const ScenePtr &scene : m_scenes)
	{
		scene->BackUpdate(timer);
	}

	m_scenes.back()->Update(timer);
}

// 描画処理
void SceneManager::Render()
{
	// シーンの描画(frontから)
	assert(m_scenes.size() > 0 && "There is no active state."); // シーンが一つも無かったらエラー
	for (const ScenePtr &scene : m_scenes)
	{
		scene->Render();
	}
}

// シーンのリクエスト(リセット)
void SceneManager::RequestScene_Reset(std::unique_ptr<IScene> scene)
{
	//全部消してからシーンを追加する処理-----------------------
	// 消すシーンの数を現在のシーン全てにする
	m_count = m_scenes.size();
	// 追加シーンをリクエストする
	m_addScenes.push_back(std::move(scene));
	//-------------------------------------------------------------
}

// シーンのリクエスト(追加)
void SceneManager::RequestScene_Add(std::unique_ptr<IScene> scene)
{
	// 消さずにシーンを追加する処理-----------------------
	// 追加シーンをリクエストする
	m_addScenes.push_back(std::move(scene));
	//-------------------------------------------------------------
}

// 実行中のシーンの一部を削除
void SceneManager::PopScene(int count)
{
	assert(count > 0 && "Count is invalid.");

	// 追加するシーンがあったら消す(仕様)
	if (m_addScenes.size() > 0)
	{
		m_addScenes.clear();
		count--;
	}

	// 削除するシーンを追加する(最小を０、最大を (今あるシーンの数 - 1) にする)
	m_count = std::min(std::max(m_count + count, 0), static_cast<const int>(m_scenes.size() - 1));
}

// protected
// シーンをステートに追加
void SceneManager::AddScene()
{
	for (std::unique_ptr<IScene> &scene : m_addScenes)
	{
		m_scenes.push_back(std::move(scene));
		m_scenes.back()->Create();
		m_scenes.back()->initialize();
	}

	m_addScenes.erase(m_addScenes.begin(), m_addScenes.end());
}
