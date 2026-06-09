#pragma once

/**
 * \ingroup XiahClient
 *
 * \version 1.0
 * first version
 *
 * \date 2005-01-14
 * 
 * \todo 
 *
 * \bug 
 *
 */
class Helper
{
public:
	Helper();
	~Helper();

	void Update();

	bool TalkShow(const int nOriginalID);
	bool TalkContinue();
	void TalkStop();
	void TalkBack();
	void TalkNext();
	bool SecretApplication();
	bool SecretMove();
	void SecretCancle();
	bool TalkListShow(const int nID);

	inline void SetSelectID(const int nID);

//private:
//	bool TalkListShow(const int nID);

protected:

private:
	int m_nSelectID;
	bool m_bSelect;

};

inline void Helper::SetSelectID(const int nID)
{
	m_nSelectID = nID;
}

extern Helper g_Helper;