HBITMAP CreateHBitmapFromBuffer(HDC hdc, SIZE *pSize, char *buf)
{
	BITMAPINFOHEADER bmih;
	BITMAPINFO bmi;
	memset( (void*)&bmih, 0, sizeof(bmih) );
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = pSize->cx;
	bmih.biHeight = pSize->cy;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	memset( (void*)&bmi, 0, sizeof(BITMAPINFO) );
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = pSize->cx;
	bmi.bmiHeader.biHeight = pSize->cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biCompression = BI_RGB;
	return CreateDIBitmap(hdc, &bmih, CBM_INIT, buf, &bmi, DIB_RGB_COLORS);
}