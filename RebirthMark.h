

//#include "munpamark.h"

class RebirthMark
//	: public CMunpaMark
{
public:
	RebirthMark();
	~RebirthMark();

	void Init();
	void RenderMark(int nMarkID, int nX, int nY);

protected:

	void Release();

private:

	LPDIRECT3DTEXTURE9	m_pTexture[13];		// 환생 텍스쳐
	LPDIRECT3DVERTEXBUFFER9	m_pVB;

};

extern RebirthMark g_RebirthMark;