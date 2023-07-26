#include "pch.h"
#include "SceneManager.h"
#include <Game\Scene\Interface\IScene.h>
#include <Game\Locator\Locator.h>

// �R���X�g���N�^
SceneManager::SceneManager()
{
}

// �f�X�g���N�^
SceneManager::~SceneManager()
{
}

// �X�V����
void SceneManager::Update(DX::StepTimer const& timer)
{
	// �O�ɕ\������Ă���V�[��(back)���J�E���g�̐�������������
	if (m_count > 0)
	{
		for (int i = 0; i < m_count; i++)
		{
			m_scenes.back()->Finalize();
			m_scenes.pop_back();
		}
		m_count = 0;
	}

	// �V�[���ǉ�
	if (static_cast<int>(m_addScenes.size()) > 0)
	{
		AddScene();
	}

	// �V�[���̃A�b�v�f�[�g(back)
	assert(m_scenes.size() > 0 && "There is no active state."); // �V�[�����������������G���[
	for (const ScenePtr &scene : m_scenes)
	{
		scene->BackUpdate(timer);
	}

	m_scenes.back()->Update(timer);
}

// �`�揈��
void SceneManager::Render()
{
	// �V�[���̕`��(front����)
	assert(m_scenes.size() > 0 && "There is no active state."); // �V�[�����������������G���[
	for (const ScenePtr &scene : m_scenes)
	{
		scene->Render();
	}
}

// �V�[���̃��N�G�X�g(���Z�b�g)
void SceneManager::RequestScene_Reset(std::unique_ptr<IScene> scene)
{
	//�S�������Ă���V�[����ǉ����鏈��-----------------------
	// �����V�[���̐������݂̃V�[���S�Ăɂ���
	m_count = m_scenes.size();
	// �ǉ��V�[�������N�G�X�g����
	m_addScenes.push_back(std::move(scene));
	//-------------------------------------------------------------
}

// �V�[���̃��N�G�X�g(�ǉ�)
void SceneManager::RequestScene_Add(std::unique_ptr<IScene> scene)
{
	// �������ɃV�[����ǉ����鏈��-----------------------
	// �ǉ��V�[�������N�G�X�g����
	m_addScenes.push_back(std::move(scene));
	//-------------------------------------------------------------
}

// ���s���̃V�[���̈ꕔ���폜
void SceneManager::PopScene(int count)
{
	assert(count > 0 && "Count is invalid.");

	// �ǉ�����V�[���������������(�d�l)
	if (m_addScenes.size() > 0)
	{
		m_addScenes.clear();
		count--;
	}

	// �폜����V�[����ǉ�����(�ŏ����O�A�ő�� (������V�[���̐� - 1) �ɂ���)
	m_count = std::min(std::max(m_count + count, 0), static_cast<const int>(m_scenes.size() - 1));
}

// protected
// �V�[�����X�e�[�g�ɒǉ�
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
