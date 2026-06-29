#pragma once

// 퀵 슬롯 확장
#define MAX_SLOT	10 //5


class CSlot
{
public:

	CSlot();
	~CSlot();

	void Clear();

	void UpDateRender();

	void SetSlot( BYTE bySlotID, DWORD dwID);
	BOOL CheckSetItemOnSlot();
	void CheckSlotSelected();
	void SetSlotToolTip( BYTE bySlotIndex);

	// 퀵 슬롯 확장
	void ChangeSlot(BYTE bySlot = 255);

	// 슬롯에 무엇이 들어있는가를 검사
	DWORD CheckQuickSlot(int i);

	BOOL SelectSlot( BYTE bySlotIndex);
	void UseItem( BYTE bySlotIndex);

	void SetActiveSlot( BYTE byIndex, DWORD dwID);
	inline DWORD GetActiveSlot();
	inline BYTE GetCurrentSlot() const;
	inline BYTE GetCurrentSlotGroup() const;
	inline DWORD GetSlotContent(int i) const { return (i >= 0 && i < MAX_SLOT) ? m_dwSlot[i] : 0; }

	void SetCurrentSlotIndex( BYTE byIndex) { m_byCurrentSlotIndex = byIndex;};
	BYTE GetCurrentSlotIndex() { return m_byCurrentSlotIndex;};

private:

	BYTE	m_byCurrentSlotGroup;	// 현재 슬롯 그룹 (5개씩 그룹)
	BYTE	m_byCurrentSlotIndex;	
	DWORD	m_dwSlot[MAX_SLOT];		// slot

	LPDIRECT3DVERTEXBUFFER9	m_pVB;

	void SkillTimeRender(RECT& rtTemp);
};

inline DWORD CSlot::GetActiveSlot()
{ 
	if( m_byCurrentSlotIndex)
		return m_dwSlot[ m_byCurrentSlotIndex-1];
	else
		return 0;
};

inline BYTE CSlot::GetCurrentSlot() const
{
	return m_byCurrentSlotIndex;
};

inline BYTE CSlot::GetCurrentSlotGroup() const
{
	return m_byCurrentSlotGroup;
};
