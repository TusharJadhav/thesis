#include "StdAfx.h"
#include "EventQueueData.h"


CEventQueueData::CEventQueueData(void)
{
  m_pLineObj = 0;
  m_stPt.nXCoordinate = 0;
  m_stPt.nYCoordinate = 0;
}

CEventQueueData::CEventQueueData(const CEventQueueData& data)
{
  m_pLineObj = new CLine(*(data.m_pLineObj));
  m_stPt = data.m_stPt;
}

CEventQueueData::~CEventQueueData(void)
{
  SAFEDELETE(m_pLineObj);
}

void CEventQueueData::getLine(CLine **ppLine)
{
  if(!ppLine)
    return;

  (*ppLine) = 0;

  CLine *pLineObj = new CLine(*m_pLineObj);
  (*ppLine) = pLineObj;
}

SPACE_POINT CEventQueueData::getPoint()
{
  return m_stPt;
}

void CEventQueueData::setLine(CLine* pLineObj)
{
  SAFEDELETE(m_pLineObj);
  m_pLineObj = new CLine(*pLineObj);
}

void CEventQueueData::setPoint(SPACE_POINT pt)
{
  m_stPt = pt;
}

bool CEventQueueData::operator ==(CEventQueueData objEventQueueData)
{
  CLine *pLineObj = 0;
  objEventQueueData.getLine(&pLineObj);

  SPACE_POINT pt;
  pt = objEventQueueData.getPoint();

  if(m_pLineObj && pLineObj && m_pLineObj == pLineObj && m_stPt == pt)
  {
    SAFEDELETE(pLineObj);
    return true;
  }

  SAFEDELETE(pLineObj);
  return false;
}

bool CEventQueueData::operator !=(CEventQueueData objEventQueueData)
{
  CLine *pLineObj = 0;
  objEventQueueData.getLine(&pLineObj);

  SPACE_POINT pt;
  pt = objEventQueueData.getPoint();

  if(m_pLineObj && pLineObj && m_pLineObj == pLineObj && m_stPt == pt)
  {
    SAFEDELETE(pLineObj);
    return false;
  }

  SAFEDELETE(pLineObj);
  return true;
}
