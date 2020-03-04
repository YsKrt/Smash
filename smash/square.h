#pragma once
#include"pos.h"

class square
{
	
public:
	pos sEnemy, sPlayer;
	HWND hwnd = 0;
	WPARAM wp = 0;
	PAINTSTRUCT ps = {};            // ペイント構造体
	HBITMAP hBitmap = 0;
	HDC hMemDC = 0;
	HWND desk = 0;
	void init()
	{
		sEnemy.init(20, 10, 100, 100, 5, 1);
		sPlayer.init(120, 10, 100, 100, 5, 1);
		HDC hdc = GetDC(hwnd);
		this->hBitmap = CreateCompatibleBitmap(hdc, 1000, 1000);
		this->hMemDC = CreateCompatibleDC(hdc);      // カレントスクリーン互換
		SelectObject(this->hMemDC, this->hBitmap);
		SelectObject(this->hMemDC, CreateSolidBrush(RGB(0, 0xFF, 0)));
		Rectangle(hMemDC, 0, 0, sEnemy.width, sEnemy.height);
		SelectObject(this->hMemDC, CreateSolidBrush(RGB(0, 0, 0xFF)));
		Rectangle(hMemDC, sEnemy.width, 0, sPlayer.width+ sEnemy.width, sPlayer.height);
		ReleaseDC(hwnd, hdc);
	}
	void final()
	{
		DeleteDC(this->hMemDC);
		DeleteObject(this->hBitmap);
		DestroyWindow(this->hwnd);
	}
	void key()
	{
		if (GetKeyState(VK_UP) < 0) {
			sPlayer.vy-=2;
		}
		if (GetKeyState(VK_DOWN) < 0) {
			sPlayer.vy+=2;
		}
		if (GetKeyState(VK_LEFT) < 0) {
			sPlayer.vx-=2;
		}
		if (GetKeyState(VK_RIGHT) < 0) {
			sPlayer.vx+=2;
		}
	}
	void move()
	{
		sEnemy.x += sEnemy.vx + sEnemy.addvx;
		sEnemy.y += sEnemy.vy + sEnemy.addvy;
		sPlayer.x += sPlayer.vx + sPlayer.addvx;
		sPlayer.y += sPlayer.vy +sPlayer.addvy;
		if ((sEnemy.x + sEnemy.width > WIDTH)) {
			sEnemy.addvx -= 3;
			if (sEnemy.x + sEnemy.width > WIDTH + 200) {
				sEnemy.hp--;
				sEnemy.addvx -= 5;
			}
		}
		if ((sEnemy.x < 0)) {
			sEnemy.addvx += 3;
			if (sEnemy.x < -200) {
				sEnemy.hp--;
				sEnemy.addvx += 5;
			}
		}
		if ((sEnemy.y + sEnemy.height > HEIGHT)) {
			sEnemy.addvy -= 3;
			if (sEnemy.y + sEnemy.height > HEIGHT + 200) {
				sEnemy.hp--;
				sEnemy.addvy -= 5;
			}
		}
		if ((sEnemy.y < 0)) {
			sEnemy.addvy += 3;
			if (sEnemy.y < -200) {
				sEnemy.hp--;
				sEnemy.addvy += 5;
			}
		}
		//
		if ((sPlayer.x + sPlayer.width > WIDTH)) {
			sPlayer.addvx -= 3;
			if (sPlayer.x + sPlayer.width > WIDTH + 200) {
				sPlayer.hp--;
				sPlayer.addvx -= 5;
			}
		}
		if ((sPlayer.x < 0)) {
			sPlayer.addvx += 3;
			if (sPlayer.x < -200) {
				sPlayer.hp--;
				sPlayer.addvx += 5;
			}
		}
		if ((sPlayer.y + sPlayer.height > HEIGHT)) {
			sPlayer.addvy -= 3;
			if (sPlayer.y + sPlayer.height > HEIGHT + 200) {
				sPlayer.hp--;
				sPlayer.addvy -= 5;
			}
		}
		if ((sPlayer.y < 0)) {
			sPlayer.addvy += 3;
			if (sPlayer.y < -200) {
				sPlayer.hp--;
				sPlayer.addvy += 5;
			}
		}

		if (abs(sEnemy.x + sEnemy.width / 2 - sPlayer.x) > 100 && abs(sEnemy.y + sEnemy.height / 2 - sPlayer.y) > 100) {
			if (abs(sPlayer.x - sEnemy.x - sEnemy.width / 2) != 0 && abs(sPlayer.y - sEnemy.y - sEnemy.height / 2) != 0) {
				sEnemy.vx = (sPlayer.x - sEnemy.x - sEnemy.width / 2) / abs(sPlayer.x - sEnemy.x - sEnemy.width / 2) * 5;
				sEnemy.vy = (sPlayer.y - sEnemy.y - sEnemy.height / 2) / abs(sPlayer.y - sEnemy.y - sEnemy.height / 2) * 5;
			}
		}else if(abs(sEnemy.x + sEnemy.width / 2 - sPlayer.x) < 100 && abs(sEnemy.y + sEnemy.height / 2 - sPlayer.y) < 100){
			if (abs(sPlayer.x - sEnemy.x - sEnemy.width / 2) != 0 && abs(sPlayer.y - sEnemy.y - sEnemy.height / 2) != 0) {
				sEnemy.vx += (sPlayer.x - sEnemy.x - sEnemy.width / 2) / abs(sPlayer.x - sEnemy.x - sEnemy.width / 2) * 2;
				sEnemy.vy += (sPlayer.y - sEnemy.y - sEnemy.height / 2) / abs(sPlayer.y - sEnemy.y - sEnemy.height / 2) * 2;
			}
		}
		sEnemy.addvx /= 1.1;
		sEnemy.addvy /= 1.1;
		sPlayer.addvx /= 1.1;
		sPlayer.addvy /= 1.1;
		if(sPlayer.vx!=0)
		sPlayer.vx -= sPlayer.vx/abs(sPlayer.vx);
		if (sPlayer.vy != 0)
		sPlayer.vy -= sPlayer.vy / abs(sPlayer.vy);
	}

	void bump()
	{
		if (abs(sEnemy.x+sEnemy.width/2-sPlayer.x-sPlayer.width/2)<sPlayer.width && abs(sEnemy.y+sEnemy.height/2-sPlayer.y-sPlayer.height/2 )<sPlayer.height) {
			int dl = abs(sEnemy.x - sPlayer.x-sPlayer.width);//Enemy基準
			int dr = abs(sEnemy.x + sEnemy.width - sPlayer.x);
			int du = abs(sEnemy.y - sPlayer.y-sPlayer.height);
			int dd = abs(sEnemy.y+ sEnemy.height - sPlayer.y);
			if (dl < dr && dl < dd && dl < du) {
				sEnemy.x += dl;
				sPlayer.x -= dl;
			}
			else if (dr < dl && dr < du && dr < dd) {
				sEnemy.x -= dr;
				sPlayer.x += dr;
			}
			if (du < dd && du < dr && du < dl) {
				sEnemy.y += du;
				sPlayer.y -= du;
			}
			else if (dd < du && dd < dr && dd < dl) {
				sEnemy.y -= dd;
				sPlayer.y += dd;
			}
			
			sPlayer.addvx = sEnemy.vx*  (5 - sEnemy.hp % 20);
			sPlayer.addvy = sEnemy.vy * (5 - sEnemy.hp % 20);
			sEnemy.addvx = sPlayer.vx * (5 - sPlayer.hp % 20);
			sEnemy.addvy = sPlayer.vy * (5 - sPlayer.hp % 20);
		
		}
	}
	void draw()
	{
		HDC hdc = BeginPaint(this->hwnd, &this->ps);
		debug(hdc);
		static TCHAR s1[15], s2[15];
		wsprintf(s1, TEXT("Enemy HP:%d"), sEnemy.hp);
		wsprintf(s2, TEXT("Player HP:%d"), sPlayer.hp);
		TextOut(hdc, 10, 0, s1, lstrlen(s1));
		TextOut(hdc, winfo.rcClient.right - 300, 0, s2, lstrlen(s2));
		BitBlt(hdc, sEnemy.x, sEnemy.y, sEnemy.width, sEnemy.height, this->hMemDC, 0, 0, SRCCOPY);
		BitBlt(hdc, sPlayer.x, sPlayer.y, sPlayer.width, sPlayer.height, this->hMemDC, sEnemy.width, 0, SRCCOPY);
		EndPaint(this->hwnd, &this->ps);

	}
	void update()
	{
		try {
			move();
			bump();
			key();
			if (sEnemy.hp < 0 | sPlayer.hp < 0) {
				int on_button = MessageBox(NULL, (sEnemy.hp < 0) ? TEXT("Player Win") : TEXT("Enemy Win"),
					TEXT("Winner"), MB_OK);
				if (on_button == IDOK)
					PostQuitMessage(0);
			}
		}
		catch (char* str) {
			str;
		}
	}
	void debug(HDC hdc)
	{

		static TCHAR strCount[8];
		wsprintf(strCount, TEXT("(%d,%d)"), sPlayer.x,sPlayer.y);
		TextOut(hdc, 10, 30, strCount, lstrlen(strCount));

	}
};