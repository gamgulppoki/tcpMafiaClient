#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <map>
#pragma comment(lib, "winmm.lib")

#include "../etc/Define.h"

class CSoundMgr {
private:
    std::map<std::wstring, std::wstring> sounds; // alias → filepath
    static CSoundMgr* m_pInstance;

public:
    void LoadAllSound();

    bool Load(const std::wstring& alias, const std::wstring& filepath) {
        if (sounds.count(alias)) Close(alias); // 중복 방지
        std::wstring cmd = L"open \"" + filepath + L"\" type mpegvideo alias " + alias;
       
        if (mciSendString(cmd.c_str(), NULL, 0, NULL) == 0) {
            sounds[alias] = filepath;
            return true;
        }

        return false;
    }

    void Play(const std::wstring& alias, bool loop = false) {
        if (!sounds.count(alias)) return;
        if (!bSoundOn) return;
        std::wstring cmd = L"seek " + alias + L" to start";
        mciSendString(cmd.c_str(), NULL, 0, NULL);
        cmd = L"play " + alias;
        if (loop) cmd += L" repeat";
        mciSendString(cmd.c_str(), NULL, 0, NULL);
    }

    void PlayBgm(EBgmID BgmID) { Play(BgmDirections[BgmID], true); CurBgmID = BgmID; }
    void PlayCurBgm() { PlayBgm(CurBgmID); }
    void PlaySoundEffect(ESoundEffectID SEID) { Play(SoundEffectDirections[SEID]); }

    void Stop(const std::wstring& alias) {
        if (!sounds.count(alias)) return;
        std::wstring cmd = L"stop " + alias;
        mciSendString(cmd.c_str(), NULL, 0, NULL);
    }

    void StopBgm(EBgmID id) { Stop(BgmDirections[id]); }
    void StopCurBgm() { StopBgm(CurBgmID); }
    void StopSoundEffect(ESoundEffectID id) { Stop(SoundEffectDirections[id]); }

    void Pause(const std::wstring& alias) {
        if (!sounds.count(alias)) return;
        std::wstring cmd = L"pause " + alias;
        mciSendString(cmd.c_str(), NULL, 0, NULL);
    }

    bool IsPaused(const std::wstring& alias) {
        if (!sounds.count(alias)) return false;
        std::wstring cmd = L"status " + alias + L" mode";
        wchar_t buffer[128] = {};
        if (mciSendString(cmd.c_str(), buffer, sizeof(buffer) / sizeof(wchar_t), NULL) == 0) {
            return std::wstring(buffer) == L"paused";
        }
        return false;
    }

    void Resume(const std::wstring& alias) {
        if (!sounds.count(alias)) return;
        std::wstring cmd = L"resume " + alias;
        mciSendString(cmd.c_str(), NULL, 0, NULL);
    }

    void Close(const std::wstring& alias) {
        if (!sounds.count(alias)) return;
        std::wstring cmd = L"close " + alias;
        mciSendString(cmd.c_str(), NULL, 0, NULL);
        sounds.erase(alias);
    }

    void CloseAll() {
        for (const auto& pair : sounds) {
            const std::wstring& alias = pair.first;
            std::wstring cmd = L"close " + alias;
            DWORD err = mciSendString(cmd.c_str(), NULL, 0, NULL);
            if (err != 0) {
                wchar_t msg[256];
                mciGetErrorString(err, msg, 256);
                MessageBox(NULL, msg, L"MCI Error", MB_OK);
            }
        }
        sounds.clear();
    }
public:
    static CSoundMgr* Get_Instance()
    {
        if (!m_pInstance)
        {
            m_pInstance = new CSoundMgr;
        }

        return m_pInstance;
    }



    static void	Destroy_Instance()
    {

        if (m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = nullptr;
        }
    }



private:
    CSoundMgr();
    CSoundMgr(const CSoundMgr& rhs) = delete;		// 삭제
    CSoundMgr& operator=(CSoundMgr& rObj) = delete;	// 삭제
    ~CSoundMgr();

public:
    bool bSoundOn = true;
    EBgmID CurBgmID;
};