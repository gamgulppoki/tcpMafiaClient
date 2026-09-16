// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가

#include <list>
#include <unordered_map>


#include <thread>
#include <mutex>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <Windows.h>
#include <wingdi.h>
#pragma comment(lib, "msimg32.lib")   // AlphaBlend 함수가 들어있는 라이브러리

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <io.h>

#include "../etc/fmod.h"
//#include "../etc/fmod.hpp"


//#pragma comment(lib, "C:\\Users\sarah\Desktop\API시연회\Mafia_Client_Last\DefaultWindow\z.code\etc\fmodex_vc.lib")
#pragma comment(lib, "fmod_vc.lib")

using namespace std;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

#endif //PCH_H
