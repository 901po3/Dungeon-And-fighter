#include"BKG.h"
#include"st.h"

Object* BKG::Create()
{
	Object* bkg = new BKG;
	bkg->init();
	return bkg;
}

void BKG::init()
{
	m_hResource = st::call()->getBmp(IDM_BackGround);
	Object::init();
}

void BKG::dealloc()
{
	DeleteObject(m_hResource);
	m_hResource = NULL;
}

void BKG::render()
{
	HDC b = st::call()->backDC;
	HDC m = st::call()->memDC;
	
	SelectObject(m, m_hResource);

	BitBlt(b, 0, 0, 1840, 600, m, 0, 0, SRCCOPY);

}

void BKG::update(float dt)
{

}