/**
The function BitmapToRegion() below, as its name implies, create a region from a bitmap. It scans the bitmap content (using a memory DC associated to a 32 bit DIB section), find continguous lines of non "transparent" pixels, add these lines to a RGNDATA structure, and finally, create the resulting region by calling ExtCreateRgn(). The function takes three arguments : the bitmap to scan, the base color which indicates the minimum value for the "transparents" pixels, and the tolerance color which, added to the base color, gives the maximum value for theses "transparents" pixels.

The sample Win32 project you can download shows how to use BitmapToRegion() to create a non-rectangular splash screen. The resulting region is equal to the "winnt256.bmp" bitmap, minus the black and "almost black" pixels. This region is then passed as an argument to the SetWindowRgn() function, with a handle to a window we created before. At last, we have only to paint de bitmap into the window to get the non-rectangular splash screen !
BitmapToRegion() is a pure Win32 function. It doesn't make any use of the MFC. Its code is standalone and only need to be compiled in a C++ file (a C compiler implies some minor modifications in the code). The function has been tested successfully on both Window 95, Windows 98 and Windows NT4. On the two formers systems, we only had have to modify the function slightly, because ExtCreateRgn() failed to create regions containing more than 4000 rectangles. The performances are quite good : on a PII-400 under NT4, the function spend only 70 ms to scan a 575x846x8 drawing (from the great artist Luis Royo) and create a corresponding region of more than 21000 rectangles !
-------------------------------------------------------------------------------------
*/

// BitmapToRegion : Create a region from the "non-transparent" pixels of a bitma
// Author :  Jean-Edouard Lachand-Robert (http://www.geocities.com/Paris/LeftBank/1160/resume.htm), June 1998
// hBmp :   Source bitma
// cTransparentColor : Color base for the "transparent" pixels (default is black
// cTolerance :  Color tolerance for the "transparent" pixels
// A pixel is assumed to be transparent if the value of each of its 3 components (blue, green and red) is
// greater or equal to the corresponding value in cTransparentColor and is lower or equal to the
// corresponding value in cTransparentColor + cTolerance

HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010)
{
	HRGN hRgn = NULL;

	ASSERT(hBmp);
	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap conten
		HDC hMemDC = CreateCompatibleDC(NULL);
		ASSERT(hMemDC);
		if (hMemDC)
		{
			// Get bitmap siz
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC
			BITMAPINFOHEADER RGB32BITSBITMAPINFO = {
				sizeof(BITMAPINFOHEADER),	// biSize
				bm.bmWidth,					// biWidth;
				bm.bmHeight,				// biHeight;
				1,							// biPlanes;
				32,							// biBitCount
				BI_RGB,						// biCompression;
				0,							// biSizeImage;
				0,							// biXPelsPerMeter;
				0,							// biYPelsPerMeter;
				0,							// biClrUsed;
				0							// biClrImportant;
			};
			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			ASSERT(hbm32);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory D
				HDC hDC = CreateCompatibleDC(hMemDC);
				ASSERT(hDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits
					BITMAP bm32;
					VERIFY(GetObject(hbm32, sizeof(bm32), &bm32));
					while (bm32.bmWidthBytes % 4)
					bm32.bmWidthBytes++;

					// Copy the bitmap into the memory D
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					VERIFY(BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY));

					// For better performances, we will use the ExtCreateRegion() function to create th
					// region. This function take a RGNDATA structure on entry. We will add rectangles b
					// amount of ALLOC_UNIT number in this structure
					#define ALLOC_UNIT 100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixel
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to righ
						for (int x = 0; x < bm.bmWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < bm.bmWidth)
							{
								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the regio
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									VERIFY(hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE));
									pData = (RGNDATA *)GlobalLock(hData);
									ASSERT(pData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is to
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									ASSERT(h);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangle
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					ASSERT(h);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean u
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}
	}

	return hRgn;
}
