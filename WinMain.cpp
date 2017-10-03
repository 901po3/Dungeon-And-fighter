#include"Header.h"
#include"Player.h"
#include"BKG.h"
#include"st.h"
#include"SceneTower.h"

st st::Instance;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR strCmdLine, int nShowCmd)
{
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(wcex));

	wcex.cbSize        = sizeof(wcex);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance     = hInstance;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hIconSm       = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpfnWndProc   = WndProc;
	wcex.lpszClassName = L"MyWindow";
	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindow(wcex.lpszClassName, wcex.lpszClassName, WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 1840, 640,NULL, NULL, hInstance, NULL);

	if(hWnd == NULL)
	{
		MessageBox(NULL, L"윈도우 생성 실패", L"에러", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);


	DWORD prevTime = timeGetTime();		
	DWORD delayTime = timeGetTime();

	MSG msg;
	memset(&msg, 0, sizeof(msg));
	while( msg.message != WM_QUIT )
	{   
		delayTime = timeGetTime() - prevTime;
		prevTime = timeGetTime();

		float dt = delayTime / 1000.f;

		st::call()->pNowRunningScene->run(dt);

		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			st::call()->CreateBackBuf(hWnd);
			st::call()->sm.init();
			st::call()->sm.loading();
			SceneTower::SetActive();
			break;
		}
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint( hWnd, &ps );

			BitBlt(hDC, 0, 0, 1840, 640, st::call()->backDC, 0, 0, SRCCOPY );

			EndPaint(hWnd, &ps);
			break;
		}
	case WM_DESTROY :
		{
			st::call()->dealloc();
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
