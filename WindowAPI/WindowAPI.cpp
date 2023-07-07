// WindowAPI.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "WindowAPI.h"

/* 이미지 */
#pragma comment(lib, "msimg32.lib")
HBITMAP hBackImage;
BITMAP bitBack;
HBITMAP hTransparentImage;
BITMAP bitBackTransparent;
HBITMAP hFrontImage;
BITMAP bitFront;

/* 애니메이션 */
HBITMAP hAniImage;
BITMAP bitAni;
const int SPRITE_SIZE_X = 57;
const int SPRITE_SIZE_Y = 52; // 나중에는 이렇게 사용 ㄴㄴ
int RUN_FRAME_MAX = 0;
int RUN_FRAME_MIN = 0;
int curframe = RUN_FRAME_MIN;
int SPRITE_FRAME_COUNT_X = 0;
int SPRITE_FRAME_COUNT_Y = 0; // 나중에 계산

/* 더블 버퍼링 */
HBITMAP hDoubleBufferImage;
// << : ANI

// >> : text

// << :

RECT rectView;

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
	//while (GetMessage(&msg, nullptr, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		else
		{
			// update(), render()...
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

#define timer_ID_1 11
#define timer_ID_2 123

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#define LIST_SIZE 100
	HDC hdc;
	PAINTSTRUCT ps;
	char temp[100] = {};
	POINT cur_point;
	static vector<CObject*> shapes_list;
	static vector<CObject*> copy_list;

	static int list_number = 0;

	static int state;

	OPENFILENAME ofn;
	TCHAR filter[] = _T("Every File(*.*)\0*.*\0Text file\0*.txt;*.doc\0");
	TCHAR lpstrFile[100] = _T("");

	static HMENU hMenu, hSubMenu;
	HBRUSH hBrush, oldBrush;

	switch (message)
	{
	case WM_SIZE:
		GetClientRect(hWnd, &rectView);
		break;
	case WM_CREATE: // 생성자처럼 초기값이 설정된다
	{
		state = 1;
		ofn.lpstrFile = lpstrFile;
		GetClientRect(hWnd, &rectView);
		/*SetTimer(hWnd, 1, 20, NULL);*/

		SetTimer(hWnd, timer_ID_2, 10, AniProc); // 제로 애니메이션 설정값

		hMenu = GetMenu(hWnd);
		hSubMenu = GetSubMenu(hMenu, 3);
		EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
		EnableMenuItem(hSubMenu, ID_EDITPASTE, MF_GRAYED);

		CreateBitmap(); // 이미지 초기 설정

		break;
	}
	case WM_RBUTTONDOWN:
	{
		cur_point.x = LOWORD(lParam);
		cur_point.y = HIWORD(lParam);
		for (int i = 0; i < list_number; i++)
		{
			if (shapes_list[i]->InObject(cur_point.x, cur_point.y) == TRUE)
			{
				shapes_list[i]->setSelection();
				break;
			}
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		cur_point.x = LOWORD(lParam);
		cur_point.y = HIWORD(lParam);

		if (list_number < LIST_SIZE)
		{
			CObject* temp;

			random_device rand;
			mt19937_64 random(rand());
			uniform_int_distribution<int> dist1(1, 3);
			int random_draw = dist1(random);

			switch (random_draw)
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
		if (wParam == timer_ID_1)
		{

		}

		if (wParam == timer_ID_2)
		{
			UpdateFrame(hWnd);
		}
	//{
	//	switch (state)
	//	{
	//	case 1: // 반사
	//	{
	//	for (int i = 0; i < shapes_list.size(); i++)
	//	{
	//		for (int j = 0; j < shapes_list.size(); j++)
	//		{
	//			if (j == i)
	//				continue;
	//			if (shapes_list[i]->Collision(*shapes_list[j], rectView) == TRUE)
	//			{
	//				shapes_list[i]->setCollide();
	//				shapes_list[j]->setCollide();
	//				shapes_list[i]->Update(rectView);
	//				shapes_list[j]->Update(rectView);
	//				break;
	//			}
	//		}
	//		shapes_list[i]->Update(rectView);
	//	}
	//	break;
	//	}
	//	case 2: // 합체
	//	{
	//		for (int i = 0; i < shapes_list.size(); i++)
	//		{
	//			for (int j = 0; j < shapes_list.size(); j++)
	//			{
	//				if (j == i || shapes_list[j]->isitDeleted() == TRUE)
	//					continue;

	//				/*if (shapes_list[i]->Collision(*shapes_list[j], rectView) == TRUE && shapes_list[j]->isitCollide() == FALSE)*/
	//				if (shapes_list[i]->Collision(*shapes_list[j], rectView) == TRUE)
	//				{
	//					if (shapes_list[i]->getCompatibility() == shapes_list[j]->getType())
	//					{
	//						if (shapes_list[i]->getRadius() > 160)
	//						{
	//							shapes_list[i]->setDeleted();
	//							shapes_list[j]->setDeleted();
	//							break;
	//						}
	//						CObject* temp = nullptr;
	//						POINT ptemp = { shapes_list[i]->getX(), shapes_list[i]->getY() };
	//						switch (shapes_list[i]->getType())
	//						{
	//						case Circle:
	//							temp = new CCircle(ptemp);
	//							break;
	//						case Rect:
	//							temp = new CRect(ptemp);
	//							break;
	//						case Star:
	//							temp = new CStar(ptemp);
	//							break;
	//						}
	//						temp->setRadius(shapes_list[i]->getRadius() + shapes_list[j]->getRadius() / 2);
	//						temp->setCollide();
	//						shapes_list[j]->setRadius(0);

	//						delete shapes_list[i];
	//						shapes_list[j]->setDeleted();

	//						shapes_list[i] = temp;
	//					}
	//					else
	//					{
	//						shapes_list[i]->Update(rectView);
	//						shapes_list[j]->Update(rectView);
	//					}
	//					break;
	//				}
	//			}
	//			shapes_list[i]->Update(rectView);
	//		}
	//		vector<CObject*> temp_list;
	//		for (int i = 0; i < shapes_list.size(); i++)
	//		{
	//			if (shapes_list[i]->isitDeleted() == TRUE)
	//				delete shapes_list[i];
	//			else
	//				temp_list.push_back(shapes_list[i]);
	//		}
	//		shapes_list = temp_list;
	//		list_number = shapes_list.size();
	//		

	//		break;
	//	}
	//	case 3: // 분열
	//	{
	//		for (int i = 0; i < list_number; i++)
	//		{
	//			for (int j = 0; j < list_number; j++)
	//			{
	//				if (j == i || shapes_list[j]->isitDeleted() == TRUE || shapes_list[j]->isitDevide() == TRUE)
	//					continue;

	//				/*if (shapes_list[i]->Collision(*shapes_list[j], rectView) == TRUE && shapes_list[j]->isitCollide() == FALSE)*/
	//				if (shapes_list[i]->Collision(*shapes_list[j], rectView) == TRUE)
	//				{
	//					if (shapes_list[i]->getCompatibility() == shapes_list[j]->getType())
	//					{
	//						if (shapes_list[i]->getRadius() <= 10)
	//						{
	//							shapes_list[i]->setDeleted();
	//							break;
	//						}

	//						for (int k = 0; k < 2; k++)
	//						{
	//							CObject* temp = nullptr;
	//							POINT ptemp = { shapes_list[i]->getX() + 20*k, shapes_list[i]->getY() + 20*k };
	//							switch (shapes_list[i]->getType())
	//							{
	//							case Circle:
	//								temp = new CCircle(ptemp);
	//								break;
	//							case Rect:
	//								temp = new CRect(ptemp);
	//								break;
	//							case Star:
	//								temp = new CStar(ptemp);
	//								break;
	//							}
	//							temp->setRadius(shapes_list[i]->getRadius() / 3 + shapes_list[j]->getRadius() / 3);
	//							temp->setCollide();
	//							temp->setDevide();

	//							shapes_list.push_back(temp);
	//						}

	//						shapes_list[i]->setDeleted();
	//					}
	//					else
	//					{
	//						shapes_list[i]->Update(rectView);
	//						shapes_list[j]->Update(rectView);
	//					}
	//					break;
	//				}
	//			}
	//			shapes_list[i]->Update(rectView);
	//		}
	//		vector<CObject*> temp_list;
	//		for (int i = 0; i < shapes_list.size(); i++)
	//		{
	//			if (shapes_list[i]->isitDeleted() == TRUE)
	//				delete shapes_list[i];
	//			else
	//				temp_list.push_back(shapes_list[i]);
	//		}
	//		shapes_list = temp_list;
	//		list_number = shapes_list.size();

	//		break;
	//	}
	//	}

		
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

		if (wParam == 'C' || wParam == 'c')
		{
			UINT state = GetMenuState(hSubMenu, ID_EDITCOPY, MF_BYCOMMAND);
			if ((state & MF_DISABLED) || (state & MF_GRAYED))
				EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_ENABLED);
			else if(state == MF_ENABLED)
				EnableMenuItem(hSubMenu, ID_EDITCOPY, MF_GRAYED);
		}
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
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

		//for (int i = 0; i < shapes_list.size(); i++)
		//{
		//	if (!shapes_list[i]->isifSelect())
		//		SelectObject(hdc, oldBrush);
		//	else
		//		SelectObject(hdc, hBrush);
		//	shapes_list[i]->Draw(hdc);
		//	shapes_list[i]->setCollideFalse();
		//	shapes_list[i]->setDevideFalse();
		//}

		//DrawBitmap(hWnd, hdc); // 비트맵 그리기 함수
		DrawBitmapDoubleBuffering(hWnd, hdc);
		DrawRectText(hdc);

		// 여기까지 그리기
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, timer_ID_2);
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);

		DeleteBitmap();

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

void CreateBitmap()
{
	// 수지
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지.bmp"), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); // 이미지, 좌표, 옵션들 설정
	if (hBackImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러 1"), _T("에러"), MB_OK);
	}
	GetObject(hBackImage, sizeof(BITMAP), &bitBack); 

	// 시공
	hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); 
	if (hTransparentImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러 2"), _T("에러"), MB_OK);
	}
	GetObject(hTransparentImage, sizeof(BITMAP), &bitBackTransparent); 

	// 제로
	hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/zero_run.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	if (hAniImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러 3"), _T("에러"), MB_OK);
	}
	GetObject(hAniImage, sizeof(BITMAP), &bitAni); // 저장을 어디에 하는지?

	// 애니메이션 설정값 계산
	RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X - 1; // -1 해야 시작점이 ㅁㄴㅇㄹ
	RUN_FRAME_MIN = 2; // 위치값
	curframe = RUN_FRAME_MIN;
	SPRITE_FRAME_COUNT_X = bitAni.bmWidth / SPRITE_SIZE_X;
	SPRITE_FRAME_COUNT_Y = bitAni.bmHeight / SPRITE_SIZE_Y;

	// 프론트 배경
	hFrontImage = (HBITMAP)LoadImage(NULL, TEXT("images/Background.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE); // 이미지, 좌표, 옵션들 설정
	if (hFrontImage == NULL)
	{
		DWORD dwError = GetLastError();
		MessageBox(NULL, _T("이미지 로드 에러 4"), _T("에러"), MB_OK);
	}
	GetObject(hFrontImage, sizeof(BITMAP), &bitFront);
}

static int xPos = 0;
void DrawBitmap(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;
	
	// 수지
	{
		/* 계속 그려줄 그림이라면, SelectObject 설정만 하고 리셋+반환을 안해도 된다 */
		hMemDC = CreateCompatibleDC(hdc); // 화면에 맞게 출력 설정 (hdc가 모니터의 설정을 가지고 잇음)
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage); // hOldBitmap 형변환하여 값 대입은 안해도 되긴 한다(사용 안할거면)
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY); // 그림 출력

		SelectObject(hMemDC, hOldBitmap); // 원래 이미지로 다시 돌려주고(리셋)
		DeleteDC(hMemDC);
	}

	// 시공
	{
		hMemDC = CreateCompatibleDC(hdc); 
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage); 
		bx = bitBackTransparent.bmWidth;
		by = bitBackTransparent.bmHeight;

		/*BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY); */
		TransparentBlt(hdc, 150, 150, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

		SelectObject(hMemDC, hOldBitmap); 
		DeleteDC(hMemDC);
	}

	// 제로
	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
		bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X; 
		by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y; // 전체 이미지를 다 따오는게 아니라서, 나눠주어야 한다

		int xStart = curframe * bx;
		int yStart = 0;

		// hMemDC, 0, 0 -> 이 0 0 위치가 사진 시작 위치
		/*TransparentBlt(hdc, 250, 250, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));*/
		TransparentBlt(hdc, xPos, 250, bx, by, hMemDC, xStart, yStart, bx, by, RGB(255, 0, 255));

		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}

}

void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hTransparentImage);
}

void Animation(int xPos, int yPos, HDC hdc)
{
}

void UpdateFrame(HWND hWnd)
{
	curframe++;
	if (curframe > RUN_FRAME_MAX)
	{
		curframe = RUN_FRAME_MIN;
	}
	InvalidateRect(hWnd, NULL, false);
}

static int yPos = 0;
VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	UpdateFrame(hWnd);

	yPos += 5;
	xPos += 5;
	if (yPos > rectView.bottom) yPos = 10;
	if (xPos > rectView.right) xPos = 10;
}

void DrawRectText(HDC hdc)
{
	TCHAR strTest[] = _T("이미지 출력");
	TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	HDC hMemDC2;
	HBITMAP hOldBitmap2;


	hMemDC = CreateCompatibleDC(hdc);
	if (hDoubleBufferImage == NULL)
	{
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom); // 화면 영역 만큼 만들어 준다
	}

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);

	// 수지
	{
		hMemDC2 = CreateCompatibleDC(hMemDC); // hMemDC에 기반한 DC2
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	// 시공
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
		bx = bitBackTransparent.bmWidth;
		by = bitBackTransparent.bmHeight;

		/*BitBlt(hdc, 100, 100, bx, by, hMemDC, 0, 0, SRCCOPY); */
		TransparentBlt(hMemDC, 150, 150, bx, by, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	// 제로
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);
		bx = bitAni.bmWidth / SPRITE_FRAME_COUNT_X;
		by = bitAni.bmHeight / SPRITE_FRAME_COUNT_Y; // 전체 이미지를 다 따오는게 아니라서, 나눠주어야 한다

		int xStart = curframe * bx;
		int yStart = 0;

		// hMemDC, 0, 0 -> 이 0 0 위치가 사진 시작 위치
		/*TransparentBlt(hdc, 250, 250, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));*/
		TransparentBlt(hMemDC, xPos, 250, bx, by, hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	// hdc에 그려주기 (hdc가 front buffer)
	/*BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);*/

	// 프론ㅌ트 (젤앞)
	{
		hMemDC2 = CreateCompatibleDC(hMemDC); // hMemDC에 기반한 DC2
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hFrontImage);
		bx = bitFront.bmWidth;
		by = bitFront.bmHeight;

		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, hBrush);

		Ellipse(hMemDC2, 200, 100, 700, 500);

		
		SelectObject(hMemDC2, oldBrush);
		DeleteObject(hBrush);

		TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, bx, by, RGB(255, 255, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	// 앞배경 - hdc에 그려줌
	TransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom, hMemDC, 0, 0, rectView.right, rectView.bottom, RGB(255, 255, 255));
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);

}

