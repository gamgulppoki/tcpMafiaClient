#include "../etc/pch.h"
#include "CAsset.h"
#include "../etc/Define.h"

CAsset::CAsset()
{
}

CAsset::~CAsset()
{
	Release();
}

void CAsset::Load_Bmp(const TCHAR* pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd, hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,			// ���α׷� �ν��Ͻ� �ڵ�(���Ϸκ��� �о���� ���̱� ������ NULL)
		pFilePath,		// ������ �̸��� ������ ���
		IMAGE_BITMAP,	// � Ÿ�� ����
		0, 0,			// ����, ���� ũ��(���Ϸκ��� �о���� ���̱� ������ ������ ����� ������ ������ ����)
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ϸκ��� �̹����� �ҷ�����

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}

void CAsset::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
