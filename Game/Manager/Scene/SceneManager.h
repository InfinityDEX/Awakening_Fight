#pragma once
#ifndef SCENEMANAGER
#define SCENEMANAGER
#include <Game\3rdParty\Timer\StepTimer.h>
#include <memory>
#include <vector>
#include <deque>

// �V�[���C���^�[�t�F�[�X
class IScene;
// Scene�̃��j�[�N�|�C���^
using ScenePtr = std::unique_ptr<IScene>;
// ScenePtr �̑o�����R���e�i
using SceneStack = std::deque<ScenePtr>;

// �V�[���}�l�[�W��
class SceneManager final
{
// �֐�
public:
	// �R���X�g���N�^
	SceneManager();
	// �f�X�g���N�^
	~SceneManager();
	// �X�V����
	void Update(DX::StepTimer const& timer);
	// �`�揈��
	void Render();
	// ���o�^�̃V�[���̃��N�G�X�g(���Z�b�g)
	void RequestScene_Reset(std::unique_ptr<IScene> scene);
	// ���o�^�̃V�[���̃��N�G�X�g(�ǉ�)
	void RequestScene_Add(std::unique_ptr<IScene> scene);
	// ���s���̃V�[���̈ꕔ���폜
	void PopScene(int count);
private:
	// �V�[�����X�e�[�g�ɒǉ�
	void AddScene();
	std::vector<std::unique_ptr<IScene>> m_addScenes;
	SceneStack m_scenes;
	// �폜����V�[���̐�
	int	m_count;
};

#endif

