#include "TSoundMgr.h"
#include "TStd.h"
#include <tchar.h>
#include "TWriter.h"
bool  TSound::Init()
{	
	return true;
}
bool  TSound::Load(std::wstring filename)
{
    std::string file= wtm(filename);
	FMOD_RESULT hr = m_pSystem->createSound(file.c_str(),
		FMOD_DEFAULT, 0, &m_pSound);
	return true;
}
bool  TSound::Frame()
{
	TCHAR m_szBuffer[256] = { 0, };
	unsigned int ms = 0;
	unsigned int total = 0;
	m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
	m_pSound->getLength(&total, FMOD_TIMEUNIT_MS);
	_stprintf_s(m_szBuffer, _T(" 경과시간[%02d:%02d:%02d], 파일크기[%02d:%02d:%02d]"),
		ms / 1000 / 60,
		ms / 1000 % 60,
		ms / 10 % 60,
		total / 1000 / 60,
		total / 1000 % 60,
		total / 10 % 60);
	TWriter::GetInstance().AddText(m_szBuffer,0,100);
	return true;
}
bool  TSound::Release()
{
	if (m_pSound)m_pSound->release();

	return true;
}

void  TSound::Play(bool bLoop)
{
	if (m_pChannel == nullptr)
	{
		if (bLoop)m_pSound->setMode(FMOD_LOOP_NORMAL);
		else m_pSound->setMode(FMOD_LOOP_OFF);
		m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
	}
	else
	{
		bool play;
		m_pChannel->isPlaying(&play);
		if (play == false)
		{
			if (bLoop)m_pSound->setMode(FMOD_LOOP_NORMAL);
			else m_pSound->setMode(FMOD_LOOP_OFF);
			m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
		}
	}
}
void  TSound::PlayEffect()
{
	m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
}
void  TSound::Stop()
{
	m_pChannel->stop();
}
void  TSound::Paused()
{
	bool paused;
	m_pChannel->getPaused(&paused);
	m_pChannel->setPaused(!paused);
}
void  TSound::VolumeUp()
{
	float fVolume;
	m_pChannel->getVolume(&fVolume);
	fVolume += 0.33f * g_fSecondPerFrame;
	fVolume = min(1.0f, fVolume);
	m_pChannel->setVolume(fVolume);
}
void  TSound::VolumeDown()
{
	float fVolume;
	m_pChannel->getVolume(&fVolume);
	fVolume -= 0.33f * g_fSecondPerFrame;
	fVolume = max(0.0f, fVolume);
	m_pChannel->setVolume(fVolume);
}





TSound* TSoundMgr::Load(std::wstring szPullfilePath)
{
    std::size_t found = szPullfilePath.find_last_of(L"/");
    std::wstring path = szPullfilePath.substr(0, found + 1);
    std::wstring key = szPullfilePath.substr(found + 1);
    TSound* data = GetPtr(key);
    if (data != nullptr)
    {
        return data;
    }
	std::unique_ptr<TSound> newData = std::make_unique<TSound>();
	newData->Set(m_pSystem);
	newData->SetName(key);
	//std::unique_ptr<TSound> copyData = std::move(newData);
    if (newData->Load(szPullfilePath))
    {
        m_list.insert(std::make_pair(key, std::move(newData)));
        return GetPtr(key);
    }	
    return nullptr;
}
TSound* TSoundMgr::GetPtr(std::wstring key)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return nullptr;
    }
    return iter->second.get();
}
bool TSoundMgr::Get(std::wstring key, TSound& ret)
{
    auto iter = m_list.find(key);
    if (m_list.end() == iter)
    {
        return false;
    }
    ret = *(iter->second);
    return true;
}
bool TSoundMgr::Release()
{
    for (auto& data : m_list)
    {
        data.second->Release();
    }
    m_list.clear();

	m_pSystem->close();
	m_pSystem->release();
    return true;
}
bool TSoundMgr::Frame()
{
	m_pSystem->update();
	for (auto& data : m_list)
	{
		data.second->Frame();
	}
	return true;
}
TSoundMgr::TSoundMgr()
{
    FMOD::System_Create(&m_pSystem);
    m_pSystem->init(32, FMOD_INIT_NORMAL, 0);
}
TSoundMgr::~TSoundMgr()
{
    Release();
}