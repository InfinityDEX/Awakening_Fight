#pragma once
#include <Audio.h>
#include <map>

using namespace DirectX;
class SoundManager final
{
	public:
		// デストラクタ
		~SoundManager() = default;
		// サウンドデータの追加
		void Register(int ID, wchar_t* path);
		// 一回だけの効果音
		void OneShot(int ID);
		// 音声ループ
		void Loop(int ID);
		// ループの停止
		void LoopStop(int ID);
		// 全ての音声の停止
		void AllStop(int ID);
		// ボリュームの調整
		void Volume(int ID, float volume);
		// オーディオエンジンの取得
		static const AudioEngine* Engine() { return s_AudioEngine.get(); };
	private:
		// オーディオエンジン
		static std::unique_ptr<AudioEngine> s_AudioEngine;
		// 音声データのリソース
		std::map<int,std::unique_ptr<SoundEffect>> m_soundResources;
		// 音声インスタンス
		std::map<int, std::unique_ptr<SoundEffectInstance>> m_soundInstances;
};