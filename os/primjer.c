#include <windows.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    /* Dio 1: registracija prozorske klase */
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0; wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0; wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "PROZOR";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Neuspjela registracija!", "Greska!",
                MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    /* Dio 2: kreiranje prozora */
    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "PROZOR", "Pravougaonik",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 350, 200,
            NULL, NULL, hInstance, NULL);
    if(hwnd == NULL) {
        MessageBox(NULL, "Neuspjelo kreiranje!", "Greska!",
                MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    /* Dio 3: Petlja poruka */
    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
/* Dio 4 prozorska procedura */
PAINTSTRUCT ps;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM
        lParam) {
    HDC hdc;
    HBRUSH NewBrush;
    switch(msg) {
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_PAINT:
            hdc=BeginPaint(hwnd,&ps);
            NewBrush = CreateSolidBrush(RGB(250, 25, 5));
            SelectObject(hdc, NewBrush);
            Rectangle(hdc,20, 20, 250, 125);
            EndPaint(hwnd,&ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
