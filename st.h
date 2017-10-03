#pragma once
#include"Scene.h"
#include"BulletManager.h"
#include"EnemyManager.h"
#include"EffectManager.h"
#include"Player.h"
#include"SoundManager.h"

class st
{
private:
	st() {pNowRunningScene = NULL;}
public:
	static st Instance;
	static st* call() {return &Instance;}
public:
	HDC						backDC, memDC;
	HBITMAP					hBackBuf;
	HINSTANCE				hInstance;
	HWND					hWnd;
	Scene					*pNowRunningScene;
	BulletManager			bm;
	EnemyManager			em;
	RECT					rect;
	EffectManager			eftm;
	SoundManager			sm;

	void CreateBackBuf(HWND _hWnd)
	{
		hWnd			= _hWnd;
		hInstance		= (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
		HDC hDC			= GetDC(_hWnd);									
		memDC			= CreateCompatibleDC( hDC );					
		backDC			= CreateCompatibleDC(hDC);					
		hBackBuf		= CreateCompatibleBitmap(hDC, 2880, 640);	
		SelectObject( backDC, hBackBuf );					
		ReleaseDC(hWnd, hDC);
		CreateGDIObject();
	}

	HBITMAP getBmp(int _index)
	{
		return LoadBitmap(hInstance, MAKEINTRESOURCE(_index));
	}

	void dealloc()
	{
		DeleteObject( backDC);
		DeleteDC( memDC );
		pNowRunningScene->dealloc();
		bm.dealloc();

		for(int i=0; i<4; i++) DeleteObject(pen[i]);
		for(int i=0; i<5; i++) DeleteObject(bru[i]);
		DeleteObject(fnt[0]);
	}

	void ReplaceScene(Scene * curScene)
	{
		if(pNowRunningScene != NULL)
		{
			pNowRunningScene->dealloc();
			delete pNowRunningScene;
			pNowRunningScene = NULL;
		}
		pNowRunningScene = curScene;
	}

	HPEN				originalPen;
	HBRUSH				originalBrush;	
	HFONT				originalFont;	

	HPEN				pen[4];
	HBRUSH				bru[5];
	HFONT				fnt[1];

	void CreateGDIObject()
	{
		pen[0]				= CreatePen( PS_SOLID, 1, RGB(255,128,0) );
		pen[1]				= CreatePen( PS_DASH , 1, RGB(255,128,0) );
		pen[2]				= CreatePen( PS_SOLID, 1, RGB(0,255,0 ) );
		pen[3]				= CreatePen( PS_SOLID, 1, RGB(133,144,255) );

		bru[0]				= (HBRUSH)GetStockObject( NULL_BRUSH );
		bru[1]				= CreateSolidBrush( RGB(0,255,0) );
		bru[2]				= CreateSolidBrush( RGB(255,0,0) );
		bru[3]				= CreateSolidBrush( RGB(20,20,20) );
		bru[4]				= CreateSolidBrush( RGB(0,0,0) );

		fnt[0]				= CreateFont(10,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,0,L"Arial");
		SetBkColor(backDC, RGB(0,0,0) );
		SetTextColor(backDC, RGB(255,128,0) );

		originalPen			= (HPEN)SelectObject(backDC, pen[0]);
		originalBrush		= (HBRUSH)SelectObject(backDC, bru[0]);
		originalFont		= (HFONT)SelectObject(backDC, fnt[0]);
	}
	void ResetGDIObject()
	{
		SelectObject(backDC, originalPen);
		SelectObject(backDC, originalBrush);
		SelectObject(backDC, originalFont);
	}
};