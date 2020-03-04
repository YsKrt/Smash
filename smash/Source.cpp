#include <windows.h>
#include <tchar.h>    // Unicode�������g���̂ɕK�v
#include <iostream>
#pragma comment(lib,"msimg32")    // TransparentBlt���g���̂ɕK�v
#include <mmsystem.h>     // timeGetTime()���g���̂ɕK�v
#pragma comment(lib,"winmm.lib") // timeGetTime()���g���̂ɕK�v

#include"resource.h"
#include"pos.h"
#include"square.h"



BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CLOSE:
        EndDialog(hwnd, IDCANCEL);
        PostQuitMessage(0);
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wp) == IDCANCEL) {
            PostQuitMessage(0);
        }
        if (LOWORD(wp) == IDOK)flag = 1;
        if (LOWORD(wp) == 3)flag = 3;
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static pos enemy;
    static square square;
    switch (msg) {
    case WM_DESTROY://�E�B���h�E�����Ƃ�
        switch (flag) {
        case 1:
            enemy.final();
            break;
        case 3:
           square.final();
            break;
        }
        PostQuitMessage(0);
        break;
    case WM_CREATE://�E�B���h�E���J���Ƃ�
        DialogBox(
            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
            TEXT("KITTY"), NULL, DialogProc);
        switch (flag) {
        case 1:
            enemy.init(20, 10, 100, 100, 5, 1);
            enemy.hwnd = hwnd;
            enemy.wp = wp;
            break;
        case 3:
            square.init();
            square.hwnd = hwnd;
            square.wp = wp;
           
            break;
        }
       
        break;

    case WM_USER + 1:   // �Q�[�����[�v�iSendMessage����Ă΂��j
        
        switch (flag) {
        case 1:
            enemy.update();
            break;
        case 3:
            square.update();
            break;
        }
        InvalidateRect(hwnd, NULL, true);//�f�X�N�g�b�v�ĕ`��
        break;
    case WM_PAINT://�����̈悪����Ƃ��̍ĕ`��BInvalidateRgn����Ă΂��B
       
        switch (flag) {
        case 1:
            enemy.draw();
            break;
        case 3:
            square.draw();
            break;
        }
        break;
    case WM_SIZE:
    case WM_MOVE:
        GetWindowInfo(hwnd, &winfo);
        WIDTH = winfo.rcWindow.right- winfo.rcWindow.left;
        HEIGHT = winfo.rcWindow.bottom - winfo.rcWindow.top;
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);//�f�t�H���g�E�B���h�E�v���[�W�����Ăяo��
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASS winc;

    winc.style = CS_HREDRAW | CS_VREDRAW;
    winc.lpfnWndProc = WndProc;
    winc.cbClsExtra = winc.cbWndExtra = 0;
    winc.hInstance = hInstance;
    winc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    winc.hCursor = LoadCursor(NULL, IDC_ARROW);
    winc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    winc.lpszMenuName = NULL;
    winc.lpszClassName = TEXT("KITTY");
    if (!RegisterClass(&winc)) return -1;

    GetWindowInfo(GetDesktopWindow(),&deskinfo);
    WIDTH = deskinfo.rcWindow.right * 8 / 10;
    HEIGHT = deskinfo.rcWindow.bottom * 8 / 10;

    hwnd = CreateWindow(
        winc.lpszClassName, TEXT("Kitty on your lap"),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        deskinfo.rcWindow.right/10, deskinfo.rcWindow.bottom / 10,
        WIDTH, HEIGHT,
        NULL, NULL,
        hInstance, NULL
    );

    GetWindowInfo(hwnd,&winfo);
    
    if (hwnd == NULL) return -1;
    // �t���[�����[�g����p
    static DWORD now_time, prev_time, fps;
    timeBeginPeriod(1); // timeGetTime�̐��x���グ��
    while (1) {//���b�Z�[�W���󂯎��
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);// ���b�Z�[�W�����z�L�[���當���ɕϊ�
            DispatchMessage(&msg);//�E�B���h�E�v���[�W���ɓn��
        }
        else {
            now_time = timeGetTime();// ���ݎ������擾
            // �t���[�����[�g��60�ɂȂ�悤�ɒ���
            if ((now_time - prev_time) >= 1000 / 60)
            {
                prev_time = now_time;
                // �Q�[�����[�v(WM_USER+1)��
                SendMessage(hwnd, WM_USER + 1, 0, 0);
                Sleep(1);
            }
        }

    }
    timeEndPeriod(1);   // �^�C�}�[�I���
    return msg.wParam;
}