#include <windows.h>

#define APP_NAME TEXT("Sample_MainWindow")
const LPTSTR thread1 = TEXT("thread1");
const LPTSTR thread2 = TEXT("thread2");
const int interval = 20;
HDC hdc;

typedef struct _ThreadParam {
	HWND owner;
	POINT point;
} ThreadParam ;

DWORD WINAPI ThreadFunc(LPVOID vdParam) {
	RECT rect;
	ThreadParam param;

	param.owner = ((ThreadParam *)vdParam)->owner;
	param.point.x = ((ThreadParam *)vdParam)->point.x;
	param.point.y = ((ThreadParam *)vdParam)->point.y;
	GetClientRect(param.owner , &rect);

	hdc = GetDC(param.owner);
	SelectObject(hdc , GetStockObject(BLACK_BRUSH));
	while(param.point.y < rect.bottom) {
		Ellipse(
			hdc , param.point.x , param.point.y ,
			param.point.x + 10 , param.point.y + 10
		);
		param.point.y += 1;
		Sleep(10);
	}
	ReleaseDC(param.owner , hdc);

	return TRUE;
}

LRESULT CALLBACK WindowProc (
	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
) {
	ThreadParam param;
	DWORD dwID;

	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_LBUTTONDOWN:
		param.owner = hWnd;
		param.point.x = LOWORD(lParam);
		param.point.y = HIWORD(lParam);
		//TextOut(hdc, 10, 10, thread1, lstrlen(thread1));
		CreateThread(NULL , 0 , ThreadFunc , (LPVOID)&param , 0 , &dwID);
		return 0;
	}
	return DefWindowProc(hWnd , uMsg , wParam , lParam);
}

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow
) {
	WNDCLASS wc;
	MSG msg;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WindowProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL , IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL , IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= APP_NAME;

	if (!RegisterClass(&wc)) return 0;

	if (CreateWindow(
		APP_NAME , TEXT(__FILE__) ,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT , CW_USEDEFAULT,
		CW_USEDEFAULT  , CW_USEDEFAULT,
		NULL , NULL , hInstance , NULL
	) == NULL) return 0;

	while(GetMessage(&msg , NULL , 0 , 0) > 0) {
		DispatchMessage(&msg);
	}
	return msg.wParam;
}