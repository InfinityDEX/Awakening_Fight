#include "pch.h"
#include "FighterAnimation.h"

FighterAnimation::FighterAnimation(const Vector3* const pos, const int* const dir, Vector3 scale)
	: AnimationEffect(0, Vector2::Zero, Vector3::Zero, Matrix::Identity, nullptr, true)
	, m_position(pos)
	, m_scale(scale)
	, m_dir(dir)
{
	m_velocity = Vector3::Zero;
	m_nonStop = false;
}

FighterAnimation::~FighterAnimation()
{}

void FighterAnimation::NowAnimation(int motionID, int cycle, bool nonStop)
{
	assert(m_textures.count(motionID) > 0 && "���̃A�j���[�V�����͐ݒ肳��Ă��܂���");

	if (m_nonStop)
		return;
	m_currentTextureData = &m_textures[motionID];
	GetLife() = cycle;
	GetMaxLife() = cycle;
	m_nonStop = nonStop;
}

void FighterAnimation::Register(AnimationDataRepository animationRepository)
{
	auto device = GameContext<DX::DeviceResources>::Get()->GetD3DDevice();
	for (auto iter = animationRepository.begin(); iter != animationRepository.end(); iter++)
	{
		CreateWICTextureFromFile(device, iter->second.path, nullptr, m_textures[iter->first].texture.GetAddressOf());
		m_textures[iter->first].split = iter->second.split;
	}
}

void FighterAnimation::Update(DX::StepTimer timer)
{
	m_endOfAnimation = false;
	if (GetLife() <= 0)
	{
		m_nonStop = false;
		m_endOfAnimation = true;
	}

	// ���W���X�V����
	GetPosition() = *m_position;
	// �傫�����X�V����
	GetScale() = Vector3(m_scale.x * (*m_dir), m_scale.y, m_scale.z) / 2;
	// �e�N�X�`�����X�V����
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& tex = GetRenderTexture();
	tex = m_currentTextureData->texture;
	// �o�b�t�@���X�V����
	m_cbuff.Split = Vector4(m_currentTextureData->split);
	SetConstBuffer(&m_cbuff);

	AnimationEffect::Update(timer);
}
