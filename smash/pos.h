#pragma once

using namespace std;
int  WIDTH;
int HEIGHT;
WINDOWINFO winfo;
WINDOWINFO deskinfo;
int flag=0;
class pos
{
public:

	int x;
	int y;
	int width;
	int height;
	int vx;
	int vy;
	int hp=100;
	int mhp=100;
	int addvx = 0, addvy = 0;
	HWND hwnd = 0;
	WPARAM wp = 0;
	PAINTSTRUCT ps = {};            // ペイント構造体
	HBITMAP hBitmap = 0;
	HDC hMemDC = 0;
	HWND desk = 0;
	int premx = 0, premy = 0;
	int mvx = 0, mvy = 0, addmvx = 0, addmvy = 0, mx = 0, my = 0;
	POINT po;
	void init(int x, int y, int width, int height, int vx, int vy)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->vx = vx;
		this->vy = vy;

		HDC hdc = GetDC(hwnd);
		this->hBitmap = CreateCompatibleBitmap(hdc, 1000, 1000);
		this->hMemDC = CreateCompatibleDC(hdc);      // カレントスクリーン互換
		SelectObject(this->hMemDC, this->hBitmap);
		SelectObject(this->hMemDC, CreateSolidBrush(RGB(0, 0xFF, 0)));
		ReleaseDC(hwnd, hdc);
	}
	void final()
	{
		DeleteDC(this->hMemDC);
		DeleteObject(this->hBitmap);
		DestroyWindow(this->hwnd);
	}
	void draw_out()
	{//CreateDC→DeleteDC, GetDC→ReleaseDC
		SelectObject(this->hMemDC, CreateSolidBrush(RGB(0xFF, 0, 0)));
		Rectangle(this->hMemDC, this->x, this->y, 100 + this->x, 100 + this->y);
		DeleteObject(SelectObject(this->hMemDC, GetStockObject(WHITE_BRUSH)));
	}
	void draw_in()
	{
		HDC hdc = BeginPaint(this->hwnd, &this->ps);
		Rectangle(hdc, this->x, this->y, width + this->x, height + this->y);
		EndPaint(this->hwnd, &this->ps);
	}
	void move()
	{
		this->x += this->vx + addvx;
		this->y += this->vy + addvy;

		if ((this->x + width > WIDTH)) {
			addvx -= 3;
			if (x + width > WIDTH + 200) {
				hp--;
				addvx -= 5;
			}
		}
		if ((this->x < 0)) {
			addvx += 3;
			if (x < -200) {
				hp--;
				addvx += 5;
			}
		}
		if ((this->y + height > HEIGHT)) {
			addvy -= 3;
			if (y+height >HEIGHT+200) {
				hp--;
				addvy -= 5;
			}
		}
		if ((this->y < 0)) {
			addvy += 3;
			if (y < -200) {
				hp--;
				addvy += 5;
			}
		}
		if ((mx > WIDTH)) {
			//addmvx -= 3;
			mhp--;
		}
		if ((mx < 0)) {
			//addmvx += 3;
			mhp--;
		}
		if ((my > HEIGHT)) {
			//addmvy -= 3;
			mhp--;
		}
		if ((my < 0)) {
			//addmvy += 3;
			mhp--;
		}

		if (abs(x + width / 2 - mx) > 100 && abs(y + height / 2 - my) > 100) {
			if (abs(mx - x - width / 2) != 0 && abs(my - y - height / 2) != 0) {
				vx = (mx - x - width / 2) / abs(mx - x - width / 2) * 5;
				vy = (my - y - height / 2) / abs(my - y - height / 2) * 5;
			}
		}
		else if(abs(x + width / 2 - mx) < 100 && abs(y + height / 2 - my) < 100){
			if (abs(mx - x - width / 2) != 0 && abs(my - y - height / 2) != 0) {
				vx += (mx - x - width / 2) / abs(mx - x - width / 2);
				vy += (my - y - height / 2) / abs(my - y - height / 2);
			}
		}
		addvx /= 1.1;
		addvy /= 1.1;
		addmvx /= 1.1;
		addmvy /= 1.1;
	}
	void bump()
	{
		if (x<mx && x + width>mx&& y<my && y + height>my) {
			int dl = abs(x - mx);
			int dr = abs(x + width - mx);
			int du = abs(y - my);
			int dd = abs(y + height - my);
			if (dl < dr && dl < dd && dl < du) {
				x += dl;
				mx -= dl;
			}
			else if (dr < dl && dr < du && dr < dd) {
				x -= dr;
				mx += dr;
			}
			if (du < dd && du < dr && du < dl) {
				y += du;
				my -= du;
			}
			else if (dd < du && dd < dr && dd < dl) {
				y -= dd;
				my += dd;
			}
			addmvx = vx * (5-hp%20) + addvx;
			addmvy = vy * (5-hp % 20) + addvy;
			addvx = mvx* (5-mhp % 20);
			addvy = mvy* (5-mhp % 20);
		}
		SetCursorPos(po.x + addmvx, po.y + addmvy);
		mvx = mx - premx;
		mvy = my - premy;
		premx = mx;
		premy = my;
	}

	void draw()
	{

		HDC hdc = BeginPaint(this->hwnd, &this->ps);
		//debug(hdc);
		static TCHAR s1[15],s2[15];
		wsprintf(s1, TEXT("Enemy HP:%d"), hp);
		wsprintf(s2, TEXT("Player HP:%d"), mhp);
		TextOut(hdc, 10, 0, s1, lstrlen(s1));
		TextOut(hdc, winfo.rcClient.right - 300, 0, s2, lstrlen(s2));
		BitBlt(hdc, x, y, width, height, this->hMemDC, 0, 0, SRCCOPY);
		EndPaint(this->hwnd, &this->ps);

	}
	void update()
	{
		try {
			GetCursorPos(&po);
			mx = po.x - winfo.rcClient.left;
			my = po.y - winfo.rcClient.top;
			move();
			bump();
			if (hp < 0 | mhp<0) {
				int on_button= MessageBox(NULL, (hp<0)?TEXT("Player Win"):TEXT("Enemy Win"),
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
		wsprintf(strCount, TEXT("今のx座標は%dです。"), x);
		TextOut(hdc, 10, 30, strCount, lstrlen(strCount));

	}
};