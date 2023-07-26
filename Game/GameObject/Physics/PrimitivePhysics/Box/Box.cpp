#include "pch.h"
#include "Box.h"

//
//   -----------------------------
//   |                           |
//   |                           |
//   |                           |
//   |            A	(this)		 |
//	 |				------------------------------
//	 |				|			 |  			 |
//	 |				|      C     |  			 |
//	 |				| (�d������) |               |
//	 |				|            |               |
//   --------------------------------------------|
//				    |							 |
//				    |			   B(collision)  |
//  			    |							 |
//  			    |							 |
//   			    ------------------------------
//

bool Box::isHitting(const Box * collision)
{
	return
		isHittingTop(collision) ||
		isHittingBottom(collision) ||
		isHittingRight(collision) ||
		isHittingLeft(collision);
}

bool Box::isHittingTop(const Box* collision)
{
	// �u�������Ă���I�u�W�F�N�g��x�̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔����x�̑傫���v�̊���
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// �u�������Ă���I�u�W�F�N�g�̂��̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔���̂��̑傫���v�̊���
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);
	
	// ���������������ĂȂ�
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// �d�Ȃ��Ă��镔�������E�̔��蕔���ɁA�͂ݏo���Ă���
	if (!(c_yRadio	<= m_heightRadio))
	{
		return false;
	}


	//�u���I�u�W�F�N�g�̂����W�v���u�������Ă���I�u�W�F�N�g�̂����W�v��艺�ɂ���
	if (this->GetTransform()->position.y < collision->GetTransform()->position.y)
	{
		return false;
	}

	// �I�u�W�F�N�g�̏㕔�ɓ������Ă�
 	return true;
}

bool Box::isHittingBottom(const Box* collision)
{
	// �u�������Ă���I�u�W�F�N�g��x�̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔����x�̑傫���v�̊���
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// �u�������Ă���I�u�W�F�N�g�̂��̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔���̂��̑傫���v�̊���
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// ���������������ĂȂ�
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// �d�Ȃ��Ă��镔�������E�̔��蕔���ɁA�͂ݏo���Ă���
	if (!(c_yRadio <= m_heightRadio))
	{
		return false;
	}

	//�u���I�u�W�F�N�g�̂����W�v���u�������Ă���I�u�W�F�N�g�̂����W�v��蓯������ɂ���
	if (this->GetTransform()->position.y >= collision->GetTransform()->position.y)
	{
		return false;
	}

	// �I�u�W�F�N�g�̉����ɓ������Ă�
	return true;
}

bool Box::isHittingRight(const Box* collision)
{
	// �u�������Ă���I�u�W�F�N�g��x�̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔����x�̑傫���v�̊���
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// �u�������Ă���I�u�W�F�N�g�̂��̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔���̂��̑傫���v�̊���
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// ���������������ĂȂ�

	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}


	// �d�Ȃ��Ă��镔�������E�̔��蕔���ɁA�͂ݏo���Ă��Ȃ�
	if (!(c_yRadio > m_heightRadio))
	{
		return false;
	}

	//�u���I�u�W�F�N�g��x���W�v���u�������Ă���I�u�W�F�N�g��x���W�v��荶�ɂ���
	if (this->GetTransform()->position.x < collision->GetTransform()->position.x)
	{
		return false;
	}

	
	// �I�u�W�F�N�g�̉E���ɓ������Ă�
	return true;
}

bool Box::isHittingLeft(const Box* collision)
{
	// �u�������Ă���I�u�W�F�N�g��x�̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔����x�̑傫���v�̊���
	float c_xSize = -abs(this->GetTransform()->position.x - collision->GetTransform()->position.x) + (this->m_size.x + collision->m_size.x) / 2;
	float c_xRadio = c_xSize / (collision->m_size.x * 2);

	// �u�������Ă���I�u�W�F�N�g�̂��̑傫���v�ɑ΂���u�d�Ȃ��Ă��镔���̂��̑傫���v�̊���
	float c_ySize = -abs(this->GetTransform()->position.y - collision->GetTransform()->position.y) + (this->m_size.y + collision->m_size.y) / 2;
	float c_yRadio = c_ySize / (collision->m_size.y * 2);

	// ���������������ĂȂ�
	if ((c_xRadio <= 0 || c_yRadio <= 0))
	{
		return false;
	}

	// �d�Ȃ��Ă��镔�������E�̔��蕔���ɁA�͂ݏo���Ă��Ȃ�
	if (!(c_yRadio > m_heightRadio))
	{
		return false;
	}

	//�u���I�u�W�F�N�g��x���W�v���u�������Ă���I�u�W�F�N�g��x���W�v��蓯�����E�ɂ���
	if (this->GetTransform()->position.x >= collision->GetTransform()->position.x)
	{
		return false;
	}

	// �I�u�W�F�N�g�̍����ɓ������Ă�
	return true;
}
