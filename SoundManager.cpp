#include"SoundManager.h"

void SoundManager::init()
{
	r = System_Create(&pEngine);
	errorCheck(r);

	r = pEngine->init(100, FMOD_INIT_NORMAL, NULL);
	errorCheck(r);
}

void SoundManager::loading()
{
	r = pEngine->createSound("forest01.mp3", FMOD_LOOP_NORMAL, NULL, &pSound[SND_BGM]);
	errorCheck(r);
	r = pEngine->createSound("walk.wav", FMOD_DEFAULT, NULL, &pSound[SND_WALK]);
	errorCheck(r);
	r = pEngine->createSound("jump.wav", FMOD_DEFAULT, NULL, &pSound[SND_JUMP]);
	errorCheck(r);
	r = pEngine->createSound("land.wav", FMOD_DEFAULT, NULL, &pSound[SND_LAND]);
	errorCheck(r);
	r = pEngine->createSound("attackgun01.wav", FMOD_DEFAULT, NULL, &pSound[SND_FIRE]);
	errorCheck(r);
	r = pEngine->createSound("attack1.wav", FMOD_DEFAULT, NULL, &pSound[SND_HIT]);
	errorCheck(r);
	r = pEngine->createSound("mach_hit.ogg", FMOD_DEFAULT, NULL, &pSound[SND_HIT2]);
	errorCheck(r);
	r = pEngine->createSound("kick.wav", FMOD_DEFAULT, NULL, &pSound[SND_KICK]);
	errorCheck(r);
}

void SoundManager::play(int _type)
{
	pEngine->update();
	pEngine->playSound(FMOD_CHANNEL_FREE, pSound[_type], false, &pChannel[_type]);
}

void SoundManager::stop(int _ch)
{
	pChannel[_ch]->stop();
}

void SoundManager::errorCheck(FMOD_RESULT _r)
{
	if(_r != FMOD_OK)
	{
		TCHAR szStr[512] = {0};
		const char* errorMessage = FMOD_ErrorString(_r);
		MultiByteToWideChar(CP_ACP, NULL, errorMessage, 1, szStr,512);
		MessageBox(NULL, szStr, L"FMOD error", MB_OK);
	}
}
