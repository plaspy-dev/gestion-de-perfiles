#pragma once
# define MAX_DIAS	15

class __CONECTOR_EXPORT___ CTrialInfo
{
	int		m_nLeftDays;
	int		m_nMaxDays;
public:
	CTrialInfo(int maxdays=MAX_DIAS);
public:
	~CTrialInfo(void);
public:
	bool Register();
	int  GetLeftDays();
};
