#pragma once
#ifndef FIGHTERIDENTITY
#define FIGHTERIDENTITY
#include <SpriteBatch.h>
#include <SimpleMath.h>

#include <Game\3rdParty\Timer\StepTimer.h>
#include <Game\DataStructure\Fighter\Loader\Skill\SkillDataLoader.h>
#include <Game\DataStructure\Fighter\Loader\Status\StatusDataLoader.h>
#include <Game\Controller\CommandData\CommandData.h>
#include <Model.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;
class FighterFrame;
struct FighterShareData;

// �t�@�C�^�[�̌�
class FighterIdentity
{
	public:
		// �R���X�g���N�^
		FighterIdentity() = default;
		// �f�X�g���N�^
		~FighterIdentity() = default;
		// ������
		virtual void Initialize(FighterShareData* plug);
		// �ʏ�X�V����
		virtual void Update(const DX::StepTimer& timer) = 0;
		// ��ԕʍX�V����
		void StateUpdata(const DX::StepTimer& timer);
		// ���ݔ����ł���Z�f�[�^���Z�o����(���݂��Ȃ����nullptr��Ԃ�)
		const SkillData* Find_Matched_Skill();
		////////////////////////////////////////////
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		//  �R�}���h����@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		////////////////////////////////////////////
		// �{�^������
		bool PushJudgment(int buttom);
		// �A�Ŕ���(�{�^���P��)
		bool RepeatedlyJudgment(KeyData buttom, int num);
		// ���o�[�R�}���h����
		bool LeverCommandJudgment(const SkillData& data);
		bool LeverCommandJudgment(const std::vector<int>& command, const int& limit);
		////////////////////////////////////////////
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		//  Getter�@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		////////////////////////////////////////////
		// �v���O�̎擾
		FighterShareData& Plug() { return *m_plug; };
		// �Z�f�[�^�̎擾
		virtual const SkillRepository& Skills() { return *m_skillDatas; };
		// �X�e�[�^�X�f�[�^�̎擾
		const StatusData& Status() { return *m_statusData; };
		////////////////////////////////////////////
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		//  Setter�@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		////////////////////////////////////////////
		// �Z�f�[�^�t�@�C���̓ǂݍ���
		void LoadSkill(char* path) { m_skillDatas = LoadSkillDataFile(path); };
		// �X�e�[�^�X�t�@�C���̓ǂݍ���
		void LoadStatus(char* path) { m_statusData = LoadStatusDataFile(path); };
		////////////////////////////////////////////
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		//  ��Ԋ֐��@�@�@�@�@�@�@�@�@�@�@ �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@  
		//�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@
		////////////////////////////////////////////
		// ����
		virtual void Stand(const DX::StepTimer& timer) = 0;
		// ���Ⴊ��
		virtual void Squat(const DX::StepTimer& timer) = 0;
		// ��
		virtual void Fly(const DX::StepTimer& timer) = 0;
	private:
		// �R���g���[���[�C���^�[�t�F�[�X
		FighterShareData* m_plug;
		// �Z�f�[�^
		std::unique_ptr<SkillRepository> m_skillDatas;
		// �X�e�[�^�X�f�[�^
		std::unique_ptr<StatusData> m_statusData;

		// �A�ŗL������
		static const int Repeatedly_coefficient;
};

#endif // !FIGHTERIDENTITY
