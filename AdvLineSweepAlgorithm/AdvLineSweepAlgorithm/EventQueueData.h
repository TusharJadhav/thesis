#pragma once
#include "Line.h"

class CEventQueueData
{
public:
  CEventQueueData(void);
  CEventQueueData(const CEventQueueData& data);
  virtual ~CEventQueueData(void);

private:
  CLine* m_pLineObj;
	SPACE_POINT m_stPt;

public:
  void getLine(CLine **ppLineObj);
  SPACE_POINT getPoint();
  void setLine(CLine* pLineObj);
  void setPoint(SPACE_POINT pt);
  bool operator ==(CEventQueueData objEventQueueData);
  bool operator !=(CEventQueueData objEventQueueData);
};
