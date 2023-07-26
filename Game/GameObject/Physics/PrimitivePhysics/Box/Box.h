#ifndef BOX
#define BOX

#include <Game\GameObject\Interface\IObject.h>

class Box : public IObject
{
	public:
		Box()
		{ 
			m_heightRadio = 0.2f;
		};
		// 当たり判定
		bool isHitting(const Box* collision);
		bool isHittingTop(const Box* collision);
		bool isHittingBottom(const Box* collision);
		bool isHittingRight(const Box* collision);
		bool isHittingLeft(const Box* collision);
		// サイズ取得
		DirectX::SimpleMath::Vector3 GetSize() { return m_size; };
	protected:
		DirectX::SimpleMath::Vector3 m_size = DirectX::SimpleMath::Vector3::One;
		// 「全体のYの大きさ」に対する「上部と下部の当たり判定のYの大きさ」の割合
		float m_heightRadio;
};

#endif