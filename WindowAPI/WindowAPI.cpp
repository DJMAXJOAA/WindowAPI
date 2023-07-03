// WindowAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowAPI.h"

#define MAX_LOADSTRING 100

using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWAPI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWAPI));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWAPI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	HWND hWnd = CreateWindowW(szWindowClass, _T("szTitle"), WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		200, 300, 1024, 768, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define LIST_SIZE 100
	HDC hdc;
	PAINTSTRUCT ps;
	char temp[100] = {};
	POINT cur_point;
	static vector<CObject*> shapes_list;

	static int list_number = 0;
	static RECT rectView;

	static int state;

	OPENFILENAME ofn;
	TCHAR filter[] = _T("Every File(*.*)\0*.*\0Text file\0*.txt;*.doc\0");
	TCHAR lpstrFile[100] = _T("");

	switch (message)
	{
	case WM_CREATE: // 생성자처럼 초기값이 설정된다
	{
		state = 1;
		ofn.lpstrFile = lpstrFile;
		GetClientRect(hWnd, &rectView);
		SetTimer(hWnd, 1, 20, NULL);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		cur_point.x = LOWORD(lParam);
		cur_point.y = HIWORD(lParam);
		
		if (list_number < LIST_SIZE)
		{
			CObject* temp;
			switch (state)
			{
			case 1:
				temp = new CCircle(cur_point);
				break;
			case 2:
				temp = new CRect(cur_point);
				break;
			case 3:
				temp = new CStar(cur_point);
				break;
			}
			shapes_list.push_back(temp);
			list_number++;
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONUP:
	{
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}
	case WM_MOUSEMOVE:
	{
		break;
	}
	case WM_TIMER:
	{
		for (int i = 0; i < list_number; i++)
		{		
			for (int j = 0; j < list_number; j++)
			{
				if (j == i)
					continue;
				if (shapes_list[i]->Collision(*shapes_list[j]) == TRUE && !shapes_list[i]->isitCollide())
				{
					break;
				}
			}
			shapes_list[i]->Update(rectView);
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 0x31)
			state = 1;
		if (wParam == 0x32)
			state = 2;
		if (wParam == 0x33)
			state = 3;
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}
    case WM_KEYUP:
	{
		break;
	}
	case WM_CHAR:
	{
		hdc = GetDC(hWnd);
		InvalidateRgn(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		int itemp = state;
		int answ = 0;
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case ID_DRAW_CIRCLE:
		{
			answ = MessageBox(hWnd, _T("원"), _T("도형 선택"), MB_YESNOCANCEL);
			if (answ == IDYES)
			{
				state = 1;
			}
			else if (answ == IDNO)
			{

			}
			else
			{

			}
			InvalidateRgn(hWnd, NULL, TRUE);
			break;
		}
		case ID_DRAW_RECT:
			state = 2;
			InvalidateRgn(hWnd, NULL, TRUE);
			break;
		case ID_DRAW_STAR:
			state = 3;
			break;
		case ID_FILEOPEN:
			{
			memset(&ofn, 0, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd; // 부모 윈도우 -> 파일 오픈 창이 닫혔을때 어디로 가는지
			ofn.lpstrFilter = filter;
			ofn.lpstrFile = lpstrFile; //  선택 ㅏㅍ일 어디로? 저장
			ofn.nMaxFile = 100; // 총 몇개의 파일
			ofn.lpstrInitialDir = _T("."); // 현재 디렉토리
			if (GetOpenFileName(&ofn) != 0)
			{
				TCHAR str[100];
				_stprintf_s(str, _T("%s 파일을 열겠습니까?"), ofn.lpstrFile);
				MessageBox(hWnd, str, _T("파일 선택"), MB_OK); // 무조건 선택
				OutFromFile(ofn.lpstrFile, hWnd);
				MessageBox(hWnd, lpstrFile, ofn.lpstrFile, MB_OK);
			}

			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		for (int i = 0; i < list_number; i++)
		{
			shapes_list[i]->Draw(hdc);
		}

		// 여기까지 그리기
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
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

void DrawGrid(HDC hdc, POINT start, POINT end, LONG number)
{
	LONG devide_x = (end.x - start.x) / number;
	LONG devide_y = (end.y - start.y) / number;
	for (int i = start.x; i <= end.x; i += devide_x)
	{
		MoveToEx(hdc, i, start.y, NULL);
		LineTo(hdc, i, end.y);
	}
	for (int i = start.y; i <= end.y; i += devide_y)
	{
		MoveToEx(hdc, start.x, i, NULL);
		LineTo(hdc, end.x, i);
	}
}

void DrawCircle(HDC hdc, POINT point, LONG radius)
{
	POINT a = { point.x - radius, point.y - radius };
	POINT b = { point.x + radius, point.y + radius };
	Ellipse(hdc, a.x, a.y, b.x, b.y);
}

bool DrawSunFlower(HDC hdc, POINT point, LONG radius, LONG number)
{
	if (number < 3)
	{
		return false;
	}
	double angle = 2.0 / (double)number * PI;
	LONG s_radius = (radius * sin(angle / 2)) / (1 - sin(angle / 2));

	POINT temp = { point.x, point.y };
	temp.x = point.x + (radius + s_radius) * (cos(0));
	temp.y = point.y + (radius + s_radius) * (sin(0));
	DrawCircle(hdc, temp, s_radius);
	for (int i = 0; i < number; i++)
	{
		angle += angle;
		temp.x = point.x + (radius + s_radius) * (cos(angle));
		temp.y = point.y + (radius + s_radius) * (sin(angle));
		DrawCircle(hdc, temp, s_radius);
	}
}

void DrawStar(HDC hdc, POINT point, LONG radius)
{
	double angle = 2.0 * PI / (double)10;

	POINT temp[10];

	double temp_angle = 0;
	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 1)
		{
			temp[i].x = point.x + (sin(temp_angle) * radius / 2);
			temp[i].y = point.y + (cos(temp_angle) * radius / 2);
		}
		else
		{
			temp[i].x = point.x + (sin(temp_angle) * radius);
			temp[i].y = point.y + (cos(temp_angle) * radius);
		}
		temp_angle += angle;
	}
	Polygon(hdc, temp, 10);
}

double LengthPts(POINT pt1, POINT pt2)
{
	return (sqrt((float)(pt2.x - pt1.x) * (pt2.x - pt1.x) +
		(float)(pt2.y - pt1.y) * (pt2.y - pt1.y)));
}

void OutFromFile(TCHAR filename[], HWND hWnd)
{
	FILE* fptr;
	HDC hdc;
	int line;
	TCHAR buffer[500];
	line = 0;
	hdc = GetDC(hWnd);
#ifdef _UNICODE
	_tfopen_s(&fptr, filename, _T("r, ccs = UNICODE"));
#else
	_tfopen_s(&fptr, filename, _T("r"));
#endif
	while (_fgetts(buffer, 100, fptr) != NULL)
	{
		if (buffer[_tcslen(buffer) - 1] == _T('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer)); // 여기서 출력
		line++;
	}
	fclose(fptr);
	ReleaseDC(hWnd, hdc);
}
