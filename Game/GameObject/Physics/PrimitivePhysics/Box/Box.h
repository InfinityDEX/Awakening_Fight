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
		// �����蔻��
		bool isHitting(const Box* collision);
		bool isHittingTop(const Box* collision);
		bool isHittingBottom(const Box* collision);
		bool isHittingRight(const Box* collision);
		bool isHittingLeft(const Box* collision);
		// �T�C�Y�擾
		DirectX::SimpleMath::Vector3 GetSize() { return m_size; };
	protected:
		DirectX::SimpleMath::Vector3 m_size = DirectX::SimpleMath::Vector3::One;
		// �u�S�̂�Y�̑傫���v�ɑ΂���u�㕔�Ɖ����̓����蔻���Y�̑傫���v�̊���
		float m_heightRadio;
};

#endif