void DrawBitmapButton(
		HWND hBtn,
		HDC hDC,
		HBITMAP hParentBgBmp,
		HBITMAP hBmp,
		COLORREF crTransparent
	)
{
	RECT rcBtn, rcParent;
	BITMAP bmp;
	HBITMAP hMaskBmp;
	HDC hMemDC, hParentBgDC, hMaskDC;
	//取图片信息
	GetObject((HGDIOBJ)hBmp, sizeof(bmp), (LPVOID)&bmp);
	//取按钮RECT
	GetWindowRect(hBtn, &rcBtn);
	GetWindowRect(GetParent(hBtn), &rcParent);
	//创建Mask Bitmap
	hMaskBmp = CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, 1, NULL);
	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hParentBgDC = CreateCompatibleDC(hDC);
	hMaskDC = CreateCompatibleDC(hDC);
	//为DC选择Bitmap
	SelectObject(hMemDC, (HGDIOBJ)hBmp);
	SelectObject(hParentBgDC, (HGDIOBJ)hParentBgBmp);
	SelectObject(hMaskDC, (HGDIOBJ)hMaskBmp);
	//生成Mask Bitmap，透明色部份为白色，其它部份为黑色
	SetBkColor(hMemDC, crTransparent);
	BitBlt(hMaskDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
	//将按钮图片透明色部份设置为白色
	SetBkColor(hMemDC, RGB(255,255,255));
	BitBlt(hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMaskDC, 0, 0, SRCPAINT);
	//复制父窗口背景到按钮
	BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hParentBgDC, rcBtn.left-rcParent.left, rcBtn.top-rcParent.top, SRCCOPY);
	//将按钮图片非透明部份设置为白色
	BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMaskDC, 0, 0, MERGEPAINT);
	//合并
	BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCAND);
	//清理
	DeleteObject((HGDIOBJ)hMaskBmp);
	DeleteDC(hMemDC);
	DeleteDC(hParentBgDC);
	DeleteDC(hMaskDC);
}

void DrawBitmapButton2(
		HWND hBtn,
		HDC hDC,
		HBITMAP hParentBgBmp,
		HBITMAP hBmp,
		COLORREF crTransparent
	)
{
	RECT rcBtn, rcParent;
	BITMAP bmp;
	HDC hMemDC, hBgDC;
	//取图片信息
	GetObject((HGDIOBJ)hBmp, sizeof(bmp), (LPVOID)&bmp);
	//取按钮RECT
	GetWindowRect(hBtn, &rcBtn);
	GetWindowRect(GetParent(hBtn), &rcParent);
	//创建内存DC
	hMemDC = CreateCompatibleDC(hDC);
	hBgDC = CreateCompatibleDC(hDC);
	//为DC选择Bitmap
	SelectObject(hMemDC, (HGDIOBJ)hBmp);
	SelectObject(hBgDC, (HGDIOBJ)hParentBgBmp);
	//合并
	TransparentBlt(hBgDC, rcBtn.left-rcParent.left, rcBtn.top-rcParent.top, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, bmp.bmWidth, bmp.bmHeight, crTransparent);
	//复制父窗口背景到按钮
	BitBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hBgDC, rcBtn.left-rcParent.left, rcBtn.top-rcParent.top, SRCCOPY);
	//清理
	DeleteDC(hMemDC);
	DeleteDC(hBgDC);
}