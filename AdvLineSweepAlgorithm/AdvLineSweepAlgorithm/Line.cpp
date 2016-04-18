#include "StdAfx.h"
#include "Line.h"
#include <sys/timeb.h>
#include <cstdlib>
#include <cmath>

const UINT CLine::LOWEST_X_COORDINATE = 1;
const UINT CLine::HIGHEST_X_COORDINATE = 100;
const UINT CLine::LOWEST_Y_COORDINATE = 1;
const UINT CLine::HIGHEST_Y_COORDINATE = 100;

CLine::CLine(void)
{
	// Generate random upper and lower points
	m_ptUpper.nXCoordinate = 0;
	m_ptUpper.nYCoordinate = 0;

	m_ptLower.nXCoordinate = 0;
	m_ptLower.nYCoordinate = 0;

	/*SPACE_POINT pt1 = GenerateRandomPoint();
	::Sleep(1000);
	SPACE_POINT pt2 = GenerateRandomPoint();

	if(pt1.nYCoordinate > pt2.nYCoordinate)
	{
		m_ptUpper = pt1;
		m_ptLower = pt2;
	}
	else
	if(pt1.nYCoordinate < pt2.nYCoordinate)
	{
		m_ptUpper = pt2;
		m_ptLower = pt1;
	}
	else
	if(pt1.nYCoordinate == pt2.nYCoordinate)
	{
		if(pt1.nXCoordinate <= pt2.nXCoordinate)
		{
			m_ptUpper = pt1;
			m_ptLower = pt2;
		}
		else
		if(pt1.nXCoordinate <= pt2.nXCoordinate)
		{
			m_ptUpper = pt2;
			m_ptLower = pt1;
		}
	}*/
}

CLine::CLine(const CLine& data)
{
  m_ptUpper = data.m_ptUpper;
  m_ptLower = data.m_ptLower;
}

CLine::~CLine(void)
{
}

SPACE_POINT CLine::GenerateRandomPoint()
{
	SPACE_POINT ptRandom;

	srand((unsigned int) time(NULL));
	ptRandom.nXCoordinate = (rand() % CLine::HIGHEST_X_COORDINATE) + CLine::LOWEST_X_COORDINATE;
	ptRandom.nYCoordinate = (rand() % CLine::HIGHEST_Y_COORDINATE) + CLine::LOWEST_Y_COORDINATE;

	if(ptRandom.nXCoordinate == ptRandom.nYCoordinate)
	{
		ptRandom.nYCoordinate = CLine::HIGHEST_Y_COORDINATE - ptRandom.nYCoordinate;
	}

	return ptRandom;
}

SPACE_POINT CLine::getUpperPoint()
{
	return m_ptUpper;
}
SPACE_POINT CLine::getLowerPoint()
{
	return m_ptLower;
}

void CLine::SetUpperPoint(SPACE_POINT ptUpper)
{
	m_ptUpper = ptUpper;
}

void CLine::SetLowerPoint(SPACE_POINT ptLower)
{
	m_ptLower = ptLower;
}

bool CLine::PointLiesOnLine(SPACE_POINT ptPoint)
{
  return IsPointOnLine(ptPoint);
}

bool CLine::PointLiesBetweenLine(SPACE_POINT ptPoint)
{
	if(ptPoint == m_ptLower || ptPoint == m_ptUpper)
		return false;

  return IsPointOnLine(ptPoint);
}

bool CLine::IsPointOnLine(SPACE_POINT ptPoint)
{
  if(ptPoint.nXCoordinate == m_ptLower.nXCoordinate && ptPoint.nXCoordinate == m_ptUpper.nXCoordinate &&
    (ptPoint.nYCoordinate >= m_ptLower.nYCoordinate && ptPoint.nYCoordinate <= m_ptUpper.nYCoordinate))
    return true;

  if(ptPoint.nYCoordinate == m_ptLower.nYCoordinate && ptPoint.nXCoordinate == m_ptUpper.nYCoordinate &&
    (ptPoint.nXCoordinate >= m_ptLower.nXCoordinate && ptPoint.nXCoordinate <= m_ptUpper.nXCoordinate))
    return true;

  double nHigherXCoordinate(0.0);
  double nLowerXCoordinate(0.0);
  if(m_ptLower.nXCoordinate <= m_ptUpper.nXCoordinate)
  {
    nHigherXCoordinate = m_ptUpper.nXCoordinate;
    nLowerXCoordinate = m_ptLower.nXCoordinate;
  }
  else
  {
    nHigherXCoordinate = m_ptLower.nXCoordinate;
    nLowerXCoordinate = m_ptUpper.nXCoordinate;
  }

  // Ensure that the point is atleast in between the X range
  if(ptPoint.nXCoordinate < nLowerXCoordinate || ptPoint.nXCoordinate > nHigherXCoordinate)
    return false;

  double nHigherYCoordinate(0.0);
  double nLowerYCoordinate(0.0);
  if(m_ptLower.nYCoordinate <= m_ptUpper.nYCoordinate)
  {
    nHigherYCoordinate = m_ptUpper.nYCoordinate;
    nLowerYCoordinate = m_ptLower.nYCoordinate;
  }
  else
  {
    nHigherYCoordinate = m_ptLower.nYCoordinate;
    nLowerYCoordinate = m_ptUpper.nYCoordinate;
  }

  // Ensure that the point is atleast in between the Y range
  if(ptPoint.nYCoordinate < nLowerYCoordinate || ptPoint.nYCoordinate > nHigherYCoordinate)
    return false;
  
  // Line equation: y = mx + b
  // m = slope of line
  // b = y intercept
  
	double fSlope = 0.0;
  if((m_ptUpper.nXCoordinate - m_ptLower.nXCoordinate) != 0.0)
	  fSlope = (m_ptUpper.nYCoordinate - m_ptLower.nYCoordinate) / (m_ptUpper.nXCoordinate - m_ptLower.nXCoordinate);

  double yIntercept = (m_ptUpper.nYCoordinate) - (fSlope * m_ptUpper.nXCoordinate);
	
  double equationRHS = (fSlope * ptPoint.nXCoordinate) + yIntercept;

  if(ptPoint.nYCoordinate == equationRHS)
    return true;

  //If result is 99.5, and expectedResult is 100, then the relative error is 0.005
  // The maxRelativeError parameter specifies what relative error we are willing to tolerate. 
  // If we want 99.999% accuracy then we should pass a maxRelativeError of 0.00001

  double relativeError = fabs((ptPoint.nYCoordinate - equationRHS) / equationRHS);
  double maxRelativeError = 0.00001;
  if(relativeError <= maxRelativeError)
    return true;

  return false;
}

bool CLine::operator ==(CLine objLine)
{
  if(m_ptUpper == objLine.getUpperPoint() && m_ptLower == objLine.getLowerPoint())
    return true;

  return false;
}

bool CLine::operator !=(CLine objLine)
{
  if(m_ptUpper == objLine.getUpperPoint() && m_ptLower == objLine.getLowerPoint())
    return false;

  return true;
}
