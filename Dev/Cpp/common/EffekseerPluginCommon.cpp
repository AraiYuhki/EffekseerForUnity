#include "EffekseerPluginCommon.h"
#include "EffekseerPluginTexture.h"
#include "EffekseerPluginModel.h"
#include "EffekseerPluginSound.h"
#include "Effekseer.h"

using namespace Effekseer;
using namespace EffekseerPlugin;

namespace EffekseerPlugin
{
	RenderSettings renderSettings[MAX_RENDER_PATH] = {{}};

	void Array2Matrix(Matrix44& matrix, float matrixArray[])
	{
		matrix.Values[0][0] = matrixArray[ 0];
		matrix.Values[1][0] = matrixArray[ 1];
		matrix.Values[2][0] = matrixArray[ 2];
		matrix.Values[3][0] = matrixArray[ 3];
		matrix.Values[0][1] = matrixArray[ 4];
		matrix.Values[1][1] = matrixArray[ 5];
		matrix.Values[2][1] = matrixArray[ 6];
		matrix.Values[3][1] = matrixArray[ 7];
		matrix.Values[0][2] = matrixArray[ 8];
		matrix.Values[1][2] = matrixArray[ 9];
		matrix.Values[2][2] = matrixArray[10];
		matrix.Values[3][2] = matrixArray[11];
		matrix.Values[0][3] = matrixArray[12];
		matrix.Values[1][3] = matrixArray[13];
		matrix.Values[2][3] = matrixArray[14];
		matrix.Values[3][3] = matrixArray[15];
	}
}

extern "C"
{
	// �t���[���̍X�V
	void UNITY_API EffekseerUpdate(float deltaFrame)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}
		
		g_EffekseerManager->Update(deltaFrame);
	}
	
	// �G�t�F�N�g�̃��[�h
	Effect* UNITY_API EffekseerLoadEffect(const EFK_CHAR* path)
	{
		if (g_EffekseerManager == NULL) {
			return NULL;
		}
		
		return Effect::Create(g_EffekseerManager, path);
	}
	
	// �G�t�F�N�g�̃��[�h�i�������w��j
	Effect* UNITY_API EffekseerLoadEffectOnMemory(void* data, int32_t size)
	{
		if (g_EffekseerManager == NULL) {
			return NULL;
		}
		
		return Effect::Create(g_EffekseerManager, data, size);
	}
	
	// �G�t�F�N�g�̃A�����[�h
	void UNITY_API EffekseerReleaseEffect(Effect* effect)
	{
		if (effect != NULL) {
			effect->Release();
		}
	}

	// �G�t�F�N�g�Đ�
	int UNITY_API EffekseerPlayEffect(Effect* effect, float x, float y, float z)
	{
		if (g_EffekseerManager == NULL) {
			return -1;
		}

		if (effect != NULL) {
			return g_EffekseerManager->Play(effect, x, y, z);
		}
		return -1;
	}
	
	// �t���[���̍X�V(�n���h���P��)
	void UNITY_API EffekseerUpdateHandle(int handle, float deltaFrame)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}
		
		g_EffekseerManager->BeginUpdate();
		g_EffekseerManager->UpdateHandle(handle, deltaFrame);
		g_EffekseerManager->EndUpdate();
	}

	// �G�t�F�N�g��~
	void UNITY_API EffekseerStopEffect(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->StopEffect(handle);
	}
	
	// �G�t�F�N�g�̃��[�g�������~
	void UNITY_API EffekseerStopRoot(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->StopRoot(handle);
	}
	
	// �S�ẴG�t�F�N�g�Đ�
	void UNITY_API EffekseerStopAllEffects()
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->StopAllEffects();
	}

	void UNITY_API EffekseerSetPausedToAllEffects(int paused)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetPausedToAllEffects(paused != 0);
	}
	
	int UNITY_API EffekseerGetShown(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return 0;
		}

		return g_EffekseerManager->GetShown(handle);
	}

	// �G�t�F�N�g���ݒ�
	void UNITY_API EffekseerSetShown(int handle, int shown)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetShown(handle, shown != 0);
	}

	int UNITY_API EffekseerGetPaused(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return 0;
		}

		return g_EffekseerManager->GetPaused(handle);
	}

	// �G�t�F�N�g�ꎞ��~
	void UNITY_API EffekseerSetPaused(int handle, int paused)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetPaused(handle, paused != 0);
	}

	float UNITY_API EffekseerGetSpeed(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return 0;
		}

		return g_EffekseerManager->GetSpeed(handle);
	}

	void UNITY_API EffekseerSetSpeed(int handle, float speed)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetSpeed(handle, speed);
	}
	
	// �G�t�F�N�g���ݏ��
	int UNITY_API EffekseerExists(int handle)
	{
		if (g_EffekseerManager == NULL) {
			return false;
		}

		return g_EffekseerManager->Exists(handle);
	}
	
	// �G�t�F�N�g�ʒu�ݒ�
	void UNITY_API EffekseerSetLocation(int handle, float x, float y, float z)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetLocation(handle, x, y, z);
	}
	
	// �G�t�F�N�g��]�ݒ�
	void UNITY_API EffekseerSetRotation(int handle, float x, float y, float z, float angle)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		Vector3D axis(x, y, z);
		g_EffekseerManager->SetRotation(handle, axis, angle);
	}
	
	// �G�t�F�N�g�g�k�ݒ�
	void UNITY_API EffekseerSetScale(int handle, float x, float y, float z)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetScale(handle, x, y, z);
	}
	
	// �G�t�F�N�g�̃^�[�Q�b�g�ʒu�ݒ�
	void UNITY_API EffekseerSetTargetLocation(int handle, float x, float y, float z)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetTargetLocation(handle, x, y, z);
	}
	
	// �v���W�F�N�V�����s��ݒ�
	void UNITY_API EffekseerSetProjectionMatrix(int renderId, float matrixArray[])
	{
		if (renderId >= 0 && renderId < MAX_RENDER_PATH) {
			Array2Matrix(renderSettings[renderId].projectionMatrix, matrixArray);
		}
	}
	
	// �r���[�s��ݒ�
	void UNITY_API EffekseerSetCameraMatrix(int renderId, float matrixArray[])
	{
		if (renderId >= 0 && renderId < MAX_RENDER_PATH) {
			Array2Matrix(renderSettings[renderId].cameraMatrix, matrixArray);
		}
	}
	
	// �`��ݒ�
	void UNITY_API EffekseerSetRenderSettings(int renderId, bool renderIntoTexture)
	{
		if (renderId >= 0 && renderId < MAX_RENDER_PATH) {
			renderSettings[renderId].renderIntoTexture = renderIntoTexture;
		}
	}

	void UNITY_API EffekseerSetTextureLoaderEvent(
		TextureLoaderLoad load,
		TextureLoaderUnload unload)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetTextureLoader(EffekseerPlugin::TextureLoader::Create(load, unload));
	}

	void UNITY_API EffekseerSetModelLoaderEvent(
		ModelLoaderLoad load,
		ModelLoaderUnload unload)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetModelLoader(EffekseerPlugin::ModelLoader::Create(load, unload));
	}

	void UNITY_API EffekseerSetSoundLoaderEvent(
		SoundLoaderLoad load,
		SoundLoaderUnload unload)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetSoundLoader(EffekseerPlugin::SoundLoader::Create(load, unload));
	}

	void UNITY_API EffekseerSetSoundPlayerEvent(
		SoundPlayerPlay play,
		SoundPlayerStopTag stopTag,
		SoundPlayerPauseTag pauseTag,
		SoundPlayerCheckPlayingTag checkPlayingTag,
		SoundPlayerStopAll stopAll)
	{
		if (g_EffekseerManager == NULL) {
			return;
		}

		g_EffekseerManager->SetSoundPlayer(EffekseerPlugin::SoundPlayer::Create(play, stopTag, pauseTag, checkPlayingTag, stopAll ));
	}
}
