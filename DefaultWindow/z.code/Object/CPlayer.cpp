#include "../etc/pch.h"
#include "CPlayer.h"
#include "CMouse.h"
#include "../Manager/ObjectManager.h"
#include "../Manager/KeyManager.h"
#include "../Manager/CSoundMgr.h"

CPlayer::CPlayer()
    : PlayerIndex(0), PlayerJob(PLAYER_JOB_NONE)
    , bPlayerDead(false), bGameStarted(false)
{
}

CPlayer::~CPlayer()
{
    Release();
}

void CPlayer::Initialize()
{
    m_tInfo = { float(WINCX >> 1), float(WINCY >> 1), 100.f, 100.f };
    m_fSpeed = 10.f;

    // get commandline
    WCHAR* commandline = GetCommandLineW();

    const char* ip = "127.0.0.1";
    const char* port = "9091";

    // socket Initialize
    clientSocket.Socket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;

    if (inet_pton(AF_INET, (PCSTR)ip, &servAdr.sin_addr) <= 0)
    {
        perror("inet_pton error");
        exit(1);
    }

    servAdr.sin_port = htons(atoi(port));
}

void CPlayer::Update()
{
}

void CPlayer::Render(HDC hDC)
{

}

void CPlayer::Release()
{
    AllClientInfo_Vec.clear();
}

