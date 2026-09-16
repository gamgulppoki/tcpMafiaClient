#pragma once
class CAsset
{
public:
	CAsset();
	~CAsset();

public:
	HDC Get_MemDC() { return m_hMemDC; }

	void Load_Bmp(const TCHAR* pFilePath);
	void Release();

private:
	HDC	m_hMemDC;

	HBITMAP		m_hBitmap;
	HBITMAP		m_hOldBmp;
};

