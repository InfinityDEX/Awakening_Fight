#pragma once
#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\Effect\PrimitiveEffect\Interface\EffectInterface.h>
#include <SimpleMath.h>
#include <Model.h>
#include <list>
#include <map>

// 描画順
enum DrawingOrder
{
	Front,
	Back
};

class EffectManager final
{
	public:
		// マネージャーの更新処理
		void Update(const  DX::StepTimer& timer) ;
		// 描画処理
		void Render(DrawingOrder side, Matrix world, Matrix view, Matrix proj) ;
		// Effectの追加
		void Register(DrawingOrder side, EffectInterface* effect) ;
		// すべてのオブジェクトの削除
		void Reset();
	private:
		// エフェクトの消去
		void DeleteEffect();
		// エフェクトの更新
		void EffectUpdate(const DX::StepTimer& timer);
		// 保留状態のエフェクトの追加
		void AddEffect();
		// 管理しているエフェクト
		std::map<DrawingOrder, std::list<EffectInterface*>>	m_effectList;
		// まだ追加していない保留中のエフェクト
		std::list<std::pair<DrawingOrder, EffectInterface*>> m_addEffectList;
};