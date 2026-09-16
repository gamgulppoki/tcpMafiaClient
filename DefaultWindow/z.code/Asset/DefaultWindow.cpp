// DefaultWindow.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "../etc/pch.h"
#include "../Scene/CMainGame.h"
#include "../Manager/SceneManager.h"
#include "../framework.h"
#include "DefaultWindow.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND    g_hWnd;
HWND    g_Dlg;
HWND    g_hEdit;
float g_fVolume = 1.0f;

WSADATA wsaData;
SOCKADDR_IN servAdr;

bool bMafiaWin = false;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void         CreateTextBox(HWND hWnd);

// 함수 호출 규약 : 함수의 실행이 종료되면 할당되었던 스택을 정리해야 하는데 함수를 호출한 호출원(caller)가 정리할 것인지
// 아니면 호출 받은 피호출자(callee)가 정리할 것인지 정해놓은 규칙 또는 규약

// __cdecl : c / c++ 기본 호출 규약, 호출원이 stack을 정리, 가변인자 함수에 적용, 함수 본인이 스택 정리 불가
// __stdcall : winapi 기본 호출 규약, 피호출자가 stack을 정리, 고정인자 함수에 적용
// __fastcall : ecx, edx라는 레지스터에 코드를 저장, 함수 호출을 빠르게 할 수 있음
// __thiscall : this 포인터를 이용한 매개 변수 동작 시, 사용하는 규약


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // wsa start
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        cout << ("WSAStartup() error!") << endl;

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
    msg.message = WM_NULL;

    CMainGame       MainGame;
    MainGame.Initialize();

    DWORD   dwTime = GetTickCount();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            if (dwTime + 10 < GetTickCount())
            {
                MainGame.Update();
                MainGame.Render();

                dwTime = GetTickCount();
            }           
        }       
    }

    //GDI 

    //UpdateWindow(g_hWnd);

    //while (GetMessage(&msg, NULL, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);
    //}

    //GdiplusShutdown(gpToken);


    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT  rc = { 0, 0, WINCX, WINCY };

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   // rc = rc + 기본 창 설정 옵션 + 메뉴 바 크기 고려

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 
      rc.right - rc.left,
      rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// KERNEL : 메모리를 관리하고 프로그램을 실행과 관련한 명령어를 갖고 있는 라이브러리
// USER   : 유저 인터페이스와 윈도우 창 스타일을 관리
// 
// GDI    : 화면 처리와 그래픽 담당 명령어 라이브러리

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (SceneManager::Get_Instance()->IsInGameScene())
    {

    }

    switch (message)
    {
   // case WM_CREATE:
   //     SetTimer(hWnd, 0, 0, 0);
   //
   //     // 2인자 : 타이머의 id, 윈도우가 여러 개의 타이머를 생성할 수 있어서 인덱스 값으로 타이머를 지정
   //     // 3인자 : 타이머 주기, 기본 값으로 0을 넣으면 1 / 1000 초로 설정
   //     // 4인자 : null(0)인 경우 3인자에서 설정한 주기대로 WM_TIMER 메세지 발생
   //
   //     break;
   //
   // case WM_TIMER:
   //
   //     InvalidateRect(hWnd, 0, true);
   //     // 화면 갱신 함수
   //
   //     // 2인자 : 갱신할 범위(렉트의 주소), 0인 경우 화면 전체 영역을 의미
   //     // 3인자 : 갱신 기준
   //     // true : 그려려 있지는 않은 화면 범위도 갱신
   //     // false : 그려진 부분만 화면 갱신
   //     break;

    case WM_CREATE:
        CreateTextBox(hWnd);
        break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            DestroyWindow(hWnd);
            break;
        }

        break;


    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hdc = BeginPaint(hWnd, &ps);

            EndPaint(hWnd, &ps);
        }
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    g_Dlg = hDlg;
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void CreateTextBox(HWND hWnd)
{
    
    // 전역 변수 hInst를 사용하거나 GetModuleHandle(NULL)을 사용해야 합니다.
    HWND hEdit = CreateWindowW(
        TEXT("EDIT"),
        TEXT(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, // 스타일 플래그 추가
        10, 650, 780, 25, // 위치 조정 (WINCY를 모름, 일단 하단 근처로 조정)
        hWnd,
        (HMENU)IDC_CHATINPUT,
        // *** 수정: NULL 대신 전역 변수 hInst 사용 ***
        hInst,
        NULL
    );

    // 생성 실패 확인 (디버깅 시 유용)
    if (!hEdit) {
        DWORD error = GetLastError(); // 에러 코드를 확인하여 디버깅
    }

    g_hEdit = hEdit;
}