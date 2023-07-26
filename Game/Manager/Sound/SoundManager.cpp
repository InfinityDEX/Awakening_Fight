#include "pch.h"
#include "SoundManager.h"

// オーディオエンジン
std::unique_ptr<AudioEngine> SoundManager::s_AudioEngine;

// サウンドデータの追加
void SoundManager::Register(int ID, wchar_t* path)
{
	// オーディオエンジンの生成
	if(!s_AudioEngine.get())
		s_AudioEngine = std::make_unique<AudioEngine>(AudioEngine_Default);
	// リソースのコピー
	m_soundResources[ID] = std::make_unique<SoundEffect>(s_AudioEngine.get(), path);
	// インスタンスのコピー
	m_soundInstances[ID] = m_soundResources[ID]->CreateInstance();
}

// 一回だけの効果音
void SoundManager::OneShot(int ID)
{
	// 音声を一度止める
	m_soundInstances[ID]->Stop();
	// 音声を再生する
	m_soundInstances[ID]->Play();
}

// 音声ループ
void SoundManager::Loop(int ID)
{
	// インスタンスをループ再生する
	m_soundInstances[ID]->Play(true);
}

// ループの停止
void SoundManager::LoopStop(int ID)
{
	// インスタンスを再生する
	m_soundInstances[ID]->Stop();
}

// 全ての音声の停止
void SoundManager::AllStop(int ID)
{
	// 全ての音源を停止する
	for (int i = 0; i < m_soundInstances.size(); i++)
	{
		m_soundInstances[i]->Stop();
	}
}

// ボリュームの調整
void SoundManager::Volume(int ID, float volume)
{
	m_soundInstances[ID]->SetVolume(volume);
}
