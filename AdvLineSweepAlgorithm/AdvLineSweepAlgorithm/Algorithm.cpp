#pragma once

#include "StdAfx.h"
#include "Algorithm.h"
#include "AVLTree.h"
#include "AVLTree.cpp"
#include "AVLTreeNode.h"
#include "AVLTreeNode.cpp"
#include <cstdlib>
#include <Ctime>
#include <stdarg.h>
#include "tiostream.h"

using namespace std;

const UINT CAlgorithm::MAX_LINES = 20;
const UINT CAlgorithm::MIN_LINES = 2;
const UINT CAlgorithm::LINE_UPPER_BOUND = 100;
const UINT CAlgorithm::LINE_LOWER_BOUND = 1;

CAlgorithm::CAlgorithm(void)
	:m_nTotalLines(0),
	 m_pLineArray(NULL),
	 m_pEventQueue(NULL),
	 m_pStatusStructure(NULL)
{
	m_pEventQueue = new CAVLTree<CEventQueueData *>();
	m_pStatusStructure = new CAVLTree<CLine *>();
}

CAlgorithm::~CAlgorithm(void)
{
  if(m_pEventQueue)
  {
    m_pEventQueue->empty();
    SAFEDELETE(m_pEventQueue);
  }

  if(m_pStatusStructure)
  {
    m_pStatusStructure->empty();
    SAFEDELETE(m_pStatusStructure);
  }

  SAFEDELETEARRAY(m_pLineArray);
}

void CAlgorithm::Initialize()
{
	GenerateTotalNoOfLines();
	GenerateRandomLines();
	
	for(UINT nIndex = 0 ; nIndex < m_nTotalLines ; nIndex++)
	{
    CEventQueueData objEventDataUpper;

    objEventDataUpper.setLine(&(m_pLineArray[nIndex]));
		objEventDataUpper.setPoint(m_pLineArray[nIndex].getUpperPoint());

		m_pEventQueue->insert(&objEventDataUpper);

    CEventQueueData objEventDataLower;
		
		objEventDataLower.setLine(&(m_pLineArray[nIndex]));
		objEventDataLower.setPoint(m_pLineArray[nIndex].getLowerPoint());

		m_pEventQueue->insert(&objEventDataLower);
	}
}

void CAlgorithm::GenerateTotalNoOfLines()
{
	// Seed the random number generator with the current time so that 
	// the numbers will be different every time we run the function.
	using namespace std;

	srand((unsigned int)time(NULL));

	m_nTotalLines = (rand() % CAlgorithm::MAX_LINES) + CAlgorithm::MIN_LINES;

	if(m_nTotalLines >= (CAlgorithm::MAX_LINES + 1))
		m_nTotalLines = CAlgorithm::MAX_LINES;

	m_nTotalLines = 5;

	tcout << _T("Total Nuumber of Lines : ") << m_nTotalLines << endl << endl;
}

void CAlgorithm::GenerateRandomLines()
{
	// Generate m_nTotalLines number of random lines.
	// Note that each line will be having an upper end point and a lower end point.
	m_pLineArray = new CLine[m_nTotalLines];

	/*UINT upperPtXArray[] = {3,2,3,5,1};
	UINT upperPtYArray[] = {5,3,7,2,4};
	UINT lowerPtXArray[] = {1,1,3,2,4};
	UINT lowerPtYArray[] = {1,2,1,1,4};*/

  UINT upperPtXArray[] = {11,79,46,67,66,71,77,71,93};
	UINT upperPtYArray[] = {70,83,70,63,55,88,27,68,66};
	UINT lowerPtXArray[] = {35,67,69,56,30,49,90,3,6};
	UINT lowerPtYArray[] = {48,53,37,11,28,58,25,40,5};

	/*UINT upperPtXArray[] = {29,32,18,42,55,27,86,50,80,99,10,18,65};
	UINT upperPtYArray[] = {89,12,80,31,95,65,56,40,95,49,44,77,10};
	UINT lowerPtXArray[] = {35,47,49,86,28,52,73,24,45,14,45,49,75};
	UINT lowerPtYArray[] = {9,3,60,28,58,12,52,33,37,1,32,12,2};*/
	
  for(UINT nIndex = 0 ; nIndex < m_nTotalLines ; nIndex++)
	{
 		SPACE_POINT ptUpper;
		ptUpper.nXCoordinate = upperPtXArray[nIndex];
		ptUpper.nYCoordinate = upperPtYArray[nIndex];

		SPACE_POINT ptLower;
		ptLower.nXCoordinate = lowerPtXArray[nIndex];
		ptLower.nYCoordinate = lowerPtYArray[nIndex];

		m_pLineArray[nIndex].SetUpperPoint(ptUpper);
		m_pLineArray[nIndex].SetLowerPoint(ptLower);

		tcout << _T("Line") << nIndex+1 << _T(": Upper Point (") << upperPtXArray[nIndex] << _T(", ") << upperPtYArray[nIndex] << _T(") ")
			<< _T(" Lower Point (") << lowerPtXArray[nIndex] << _T(", ") << lowerPtYArray[nIndex] << _T(")") << endl;
	}

  /*for(UINT nIndex = 0 ; nIndex < m_nTotalLines ; nIndex++)
	{
		double y1 = (rand() % CAlgorithm::LINE_UPPER_BOUND) + CAlgorithm::LINE_LOWER_BOUND;
		if(y1 >= (CAlgorithm::LINE_UPPER_BOUND + 1))
			y1 = CAlgorithm::LINE_UPPER_BOUND;

		double y2 = (rand() % CAlgorithm::LINE_UPPER_BOUND) + CAlgorithm::LINE_LOWER_BOUND;
		if(y2 >= (CAlgorithm::LINE_UPPER_BOUND + 1))
			y2 = CAlgorithm::LINE_UPPER_BOUND;

		double x1 = (rand() % CAlgorithm::LINE_UPPER_BOUND) + CAlgorithm::LINE_LOWER_BOUND;
		if(x1 >= (CAlgorithm::LINE_UPPER_BOUND + 1))
			x1 = CAlgorithm::LINE_UPPER_BOUND;

		double x2 = (rand() % CAlgorithm::LINE_UPPER_BOUND) + CAlgorithm::LINE_LOWER_BOUND;
		if(x2 >= (CAlgorithm::LINE_UPPER_BOUND + 1))
			x2 = CAlgorithm::LINE_UPPER_BOUND;

		SPACE_POINT ptUpper;
		SPACE_POINT ptLower;

		if(y1 > y2)
		{
			ptUpper.nXCoordinate = x1;
			ptUpper.nYCoordinate = y1;

			ptLower.nXCoordinate = x2;
			ptLower.nYCoordinate = y2;
		}
		else
		if(y1 == y2)
		{
			if(x1 <= x2)
			{
				ptUpper.nXCoordinate = x1;
				ptUpper.nYCoordinate = y1;

				ptLower.nXCoordinate = x2;
				ptLower.nYCoordinate = y2;
			}
			else
			{
				ptUpper.nXCoordinate = x2;
				ptUpper.nYCoordinate = y2;

				ptLower.nXCoordinate = x1;
				ptLower.nYCoordinate = y1;
			}
		}
		else
		{
			ptUpper.nXCoordinate = x2;
			ptUpper.nYCoordinate = y2;

			ptLower.nXCoordinate = x1;
			ptLower.nYCoordinate = y1;
		}

		m_pLineArray[nIndex].SetUpperPoint(ptUpper);
		m_pLineArray[nIndex].SetLowerPoint(ptLower);

		tcout << _T("Line") 
					<< nIndex+1 
					<< _T(": Upper Point (") 
					<< m_pLineArray[nIndex].getUpperPoint().nXCoordinate
					<< _T(", ") 
					<< m_pLineArray[nIndex].getUpperPoint().nYCoordinate
					<< _T(") ")
					<< _T(" Lower Point (") 
					<< m_pLineArray[nIndex].getLowerPoint().nXCoordinate
					<< _T(", ") 
					<< m_pLineArray[nIndex].getLowerPoint().nYCoordinate
					<< _T(")") 
					<< endl;
	}*/

  tcout << endl;
}

void CAlgorithm::FindIntersections()
{
	if(!m_pEventQueue)
		return;

	CAVLTreeIterator<CEventQueueData *> *pEventQIterator;
	pEventQIterator = m_pEventQueue->getIterator();
	CAVLTreeNode<CEventQueueData *> *pNode = NULL;
	
	if(pEventQIterator)
		pEventQIterator->movetoLeftMostLeafOfTree();
	pNode = pEventQIterator->getCurrentNode();

	while(1)
	{
		if(!pNode)
			break;

    CEventQueueData *pEventQueueData = 0;
    pNode->getData(&pEventQueueData);

    if(pEventQueueData)
      HandleEventPoint(pEventQueueData);

		tcout << endl << _T("******* Status Structure *******") << endl;
		m_pStatusStructure->PrintTree();
		tcout << endl << _T("********************************") << endl;

    SAFEDELETE(pEventQueueData);

		if(pEventQIterator->IsRightMostLeafOfTree())
			break;

		pEventQIterator->moveToAdjacentRightLeaf();
		pNode = pEventQIterator->getCurrentNode();
	}

  tcout << _T("******* Algorithm execution finished *********") << endl;

  SAFEDELETE(pEventQIterator);
}

void CAlgorithm::HandleEventPoint(CEventQueueData* pEventData)
{
  CLine* pLineObj = 0;
  pEventData->getLine(&pLineObj);
	SPACE_POINT ptEvent = pEventData->getPoint();
	
	CArray<CLine *, CLine *> arrayU;
	GetLineSet(PT_POSITION_UPPER, arrayU, ptEvent);

	CArray<CLine *, CLine *> arrayL;
	GetLineSet(PT_POSITION_LOWER, arrayL, ptEvent);

	CArray<CLine *, CLine *> arrayC;
	GetLineSet(PT_POSITION_MIDDLE, arrayC, ptEvent);

	CArray<CLine *, CLine *> arrayUnion;
  GetLineSetUnion(&arrayU, &arrayL, &arrayC, arrayUnion);

	if(arrayUnion.GetSize() > 1)
	{
		tcout << _T("VOILA !!! Intersection point found") << endl;
    tcout << _T("Intersection point :(") << ptEvent.nXCoordinate << _T(", ") << ptEvent.nYCoordinate << _T(")") << endl << endl;
	}

  // Set currently handled event point in the status structure
  m_pStatusStructure->SetCurrentlyHandledEventPoint(ptEvent);

  CArray<CLine *, CLine *> arrUnionLowerMiddle;
	GetLineSetUnion(&arrayL, &arrayC, arrUnionLowerMiddle);
	for(INT_PTR nIndex = 0 ; nIndex < arrUnionLowerMiddle.GetSize() ; nIndex++)
		m_pStatusStructure->remove(arrUnionLowerMiddle.GetAt(nIndex));

	CArray<CLine *, CLine *> arrUnionUpperMiddle;
	GetLineSetUnion(&arrayU, &arrayC, arrUnionUpperMiddle);
	for(INT_PTR nIndex = 0; nIndex < arrUnionUpperMiddle.GetSize() ; nIndex++)
    m_pStatusStructure->insert(arrUnionUpperMiddle.GetAt(nIndex));

	CAVLTreeNode<CLine *> *pEventNodeInStatusStruct = NULL;
	CAVLTreeNode<CLine *> *pLeftNode = NULL;
	CAVLTreeNode<CLine *> *pRightNode = NULL;
	CAVLTreeIterator<CLine *> *pStatusStructIter = NULL;
	pStatusStructIter = m_pStatusStructure->getIterator();

	if(!pStatusStructIter)
  {
    SAFEDELETE(pLineObj);
    return;
  }

	if(arrUnionUpperMiddle.GetSize() == 0)
  {
		pEventNodeInStatusStruct = NULL;
		pEventNodeInStatusStruct = pStatusStructIter->Find(pLineObj);
		if(!pEventNodeInStatusStruct)
    {
      SAFEDELETE(pLineObj);
      SAFEDELETE(pStatusStructIter);
      return;
    }
		
		pLeftNode = NULL;
		pStatusStructIter->setCurrentNode(pEventNodeInStatusStruct);
		pStatusStructIter->moveToAdjacentLeftLeaf();
		pLeftNode = pStatusStructIter->getCurrentNode();
		if(!pLeftNode)
    {
      SAFEDELETE(pLineObj);
      SAFEDELETE(pStatusStructIter);
      return;
    }
		
		pStatusStructIter->setCurrentNode(pEventNodeInStatusStruct);
		pStatusStructIter->moveToAdjacentRightLeaf();
		pRightNode = NULL;
		pRightNode = pStatusStructIter->getCurrentNode();
		if(!pRightNode)
    {
      SAFEDELETE(pLineObj);
      SAFEDELETE(pStatusStructIter);
      return;
    }
		
    CLine *pLeftNodeData = 0;
    CLine *pRightNodeData = 0;

    pLeftNode->getData(&pLeftNodeData);
    pRightNode->getData(&pRightNodeData);

    FindNewEvent(pLeftNodeData, pRightNodeData, pEventData);

    SAFEDELETE(pLeftNodeData);
    SAFEDELETE(pRightNodeData);
	}
	else
	{
		CLine* pLeftMostSeg = NULL;
		CAVLTreeNode<CLine *> *pLeftMostSegNode = NULL;
		pLeftMostSeg = GetLeftMostSegInLineSet(&arrUnionUpperMiddle);
			
		CLine* pRightMostSeg = NULL;
		CAVLTreeNode<CLine *> *pRightMostSegNode = NULL;
		pRightMostSeg = GetRightMostSegInLineSet(&arrUnionUpperMiddle);

		if(!pLeftMostSeg && !pRightMostSeg)
    {
      SAFEDELETE(pLineObj);
      SAFEDELETE(pStatusStructIter);
      return;
    }
		
		pLeftMostSegNode = 	pStatusStructIter->Find(pLeftMostSeg);
				
		if(pLeftMostSegNode)
		{
      CLine *pLeftMostSegNodeData = 0;
      pLeftMostSegNode->getData(&pLeftMostSegNodeData);

      if((*pLeftMostSegNodeData) == (*pLeftMostSeg))
			{
				pStatusStructIter->setCurrentNode(pLeftMostSegNode);
				pStatusStructIter->moveToAdjacentLeftLeaf();
				pLeftNode = pStatusStructIter->getCurrentNode();

				if(pLeftNode != NULL && pLeftMostSeg != NULL)
				{
          CLine *pLeftNodeData = 0;
          pLeftNode->getData(&pLeftNodeData);

          FindNewEvent(pLeftNodeData, pLeftMostSeg, pEventData);

          SAFEDELETE(pLeftNodeData);
				}
			}

      SAFEDELETE(pLeftMostSegNodeData);
		}

		pRightMostSegNode = pStatusStructIter->Find(pRightMostSeg);
		if(pRightMostSegNode)
		{
      CLine *pRightMostSegNodeData = 0;
      pRightMostSegNode->getData(&pRightMostSegNodeData);

      if((*pRightMostSegNodeData) == (*pRightMostSeg))
			{
				pStatusStructIter->setCurrentNode(pRightMostSegNode);
				pStatusStructIter->moveToAdjacentRightLeaf();
				pRightNode = pStatusStructIter->getCurrentNode();

				if(pRightNode != NULL && pRightMostSeg != NULL)
				{
          CLine *pRightNodeData = 0;
          pRightNode->getData(&pRightNodeData);

					FindNewEvent(pRightMostSeg, pRightNodeData, pEventData);

          SAFEDELETE(pRightNodeData);
				}
			}

      SAFEDELETE(pRightMostSegNodeData);
		}
	}

  SAFEDELETE(pLineObj);
  SAFEDELETE(pStatusStructIter);
}

void CAlgorithm::GetLineSet(POINT_POSITION ptPosition, CArray<CLine *, CLine *>& resultSet, SPACE_POINT ptReference)
{
	for(UINT nIndex = 0 ; nIndex < m_nTotalLines ; nIndex++)
	{
		CLine* pLineObj = &(m_pLineArray[nIndex]);
		
		if(pLineObj)
		{
			switch(ptPosition)
			{
				case PT_POSITION_LOWER:
										if(pLineObj->getLowerPoint() == ptReference)
											resultSet.Add(pLineObj);
										break;

				case PT_POSITION_UPPER:
										if(pLineObj->getUpperPoint() == ptReference)
											resultSet.Add(pLineObj);
										break;

				case PT_POSITION_MIDDLE:
										if(pLineObj->PointLiesBetweenLine(ptReference))
											resultSet.Add(pLineObj);
										break;
			}
		}
	}
}

void CAlgorithm::GetLineSetUnion(CArray<CLine *, CLine *>* arrSet1, CArray<CLine *, CLine *>* arrSet2, CArray<CLine *, CLine *>* arrSet3, CArray<CLine *, CLine *>& resultSet)
{
	if(arrSet1->GetCount() != 0)
	{
		for(INT_PTR nIndex = 0; nIndex < arrSet1->GetCount() ; nIndex++)
      resultSet.Add(arrSet1->GetAt(nIndex));
	}

	if(arrSet2->GetCount() != 0)
	{
		for(INT_PTR nIndex = 0; nIndex < arrSet2->GetCount() ; nIndex++)
		{
			if(LineArrayContainsValue(&resultSet, arrSet2->GetAt(nIndex)) == false)
				resultSet.Add(arrSet2->GetAt(nIndex));
		}
	}

	if(arrSet3->GetCount() != 0)
	{
		for(INT_PTR nIndex = 0; nIndex < arrSet3->GetCount() ; nIndex++)
		{
			if(LineArrayContainsValue(&resultSet, arrSet3->GetAt(nIndex)) == false)
				resultSet.Add(arrSet3->GetAt(nIndex));
		}
	}
}

void CAlgorithm::GetLineSetUnion(CArray<CLine *, CLine *>* arrSet1, CArray<CLine *, CLine *>* arrSet2, CArray<CLine *, CLine *>& resultSet)
{
	if(arrSet1->GetCount() != 0)
	{
		for(INT_PTR nIndex = 0; nIndex < arrSet1->GetCount() ; nIndex++)
      resultSet.Add(arrSet1->GetAt(nIndex));
	}

	if(arrSet2->GetCount() != 0)
	{
		for(INT_PTR nIndex = 0; nIndex < arrSet2->GetCount() ; nIndex++)
		{
			if(LineArrayContainsValue(&resultSet, arrSet2->GetAt(nIndex)) == false)
				resultSet.Add(arrSet2->GetAt(nIndex));
		}
	}
}

bool CAlgorithm::LineArrayContainsValue(const CArray<CLine *, CLine *>* pArrLine, CLine *pLineObj)
{
	for(INT_PTR nIndex = 0 ; nIndex < pArrLine->GetSize() ; nIndex++)
	{
		CLine* pTempLineObj = NULL;
		pTempLineObj = pArrLine->GetAt(nIndex);
		if(pLineObj == pTempLineObj)
			return true;
	}

	return false;
}

void CAlgorithm::FindNewEvent(CLine *pLeftLine, CLine *pRightLine, CEventQueueData* pEventData)
{
	SPACE_POINT ptIntersection;
	SPACE_POINT ptEvent = pEventData->getPoint();
	if(PerformIntersectionTest(pLeftLine, pRightLine, ptIntersection) == true)
	{
    if((ptIntersection.nYCoordinate < ptEvent.nYCoordinate && ptIntersection.nYCoordinate >= 0 && ptIntersection.nXCoordinate >= 0) ||
       (ptIntersection.nYCoordinate == ptEvent.nYCoordinate && ptIntersection.nXCoordinate > ptEvent.nXCoordinate)
       )
		{
			CAVLTreeIterator<CEventQueueData *> *pIterEventQueue = NULL;
			CAVLTreeNode<CEventQueueData *> *pEventNode = NULL;
			pIterEventQueue = m_pEventQueue->getIterator();
			if(!pIterEventQueue)
        return;

			pEventNode = pIterEventQueue->Find(pEventData);
			if(pEventNode != NULL && pEventNode->isLeaf() == true)
			{
				pIterEventQueue->setCurrentNode(pEventNode);
				while(1)
				{
					CAVLTreeNode<CEventQueueData *> *pTempNode = NULL;
					pTempNode = pIterEventQueue->getCurrentNode();

          CEventQueueData *pTempNodeData = 0;
          if(pTempNode)
            pTempNode->getData(&pTempNodeData);

          if(pTempNodeData && (pTempNodeData->getPoint() == ptIntersection))
          {
            SAFEDELETE(pTempNodeData);
						return;
          }

          SAFEDELETE(pTempNodeData);

					if(pIterEventQueue->IsLeftMostLeafOfTree())
						break;

					pIterEventQueue->moveToAdjacentLeftLeaf();
				}

				CEventQueueData* pDataNew = new CEventQueueData();
        pDataNew->setLine(pLeftLine);
        pDataNew->setPoint(ptIntersection);
				
				if(pIterEventQueue->Find(pDataNew) == NULL)
					m_pEventQueue->insert(pDataNew);

        SAFEDELETE(pDataNew);
			}

      SAFEDELETE(pIterEventQueue);
		}
	}
}

CLine* CAlgorithm::GetLeftMostSegInLineSet(CArray<CLine*,CLine*> *arrLineSet)
{
	CLine* pLine = NULL;

	if(arrLineSet != NULL)
	{
		pLine = arrLineSet->GetAt(0);

		for(INT_PTR nIndex = 0 ; nIndex < arrLineSet->GetSize() ; nIndex++)
		{
			CLine* pCurrentLine = arrLineSet->GetAt(nIndex);

      if(pCurrentLine->getUpperPoint().nYCoordinate == pCurrentLine->getLowerPoint().nYCoordinate)
      {
        if(pLine)
        {
          if(pLine->getUpperPoint().nYCoordinate == pLine->getLowerPoint().nYCoordinate)
          {
            if(pCurrentLine->getLowerPoint().nYCoordinate > pLine->getLowerPoint().nYCoordinate)
            {
              pLine = pCurrentLine;
              continue;
            }
          }
        }
        
        pLine = pCurrentLine;
      }
      else
			if(pCurrentLine->getLowerPoint().nXCoordinate < pLine->getLowerPoint().nXCoordinate)
				pLine = pCurrentLine;
		}
	}

	return pLine;
}

CLine* CAlgorithm::GetRightMostSegInLineSet(CArray<CLine*,CLine*> *arrLineSet)
{
	CLine* pLine = NULL;

	if(!arrLineSet)
    return pLine;
	
	pLine = arrLineSet->GetAt(0);

	for(INT_PTR nIndex = 0 ; nIndex < arrLineSet->GetSize() ; nIndex++)
	{
		CLine* pCurrentLine = arrLineSet->GetAt(nIndex);

    if(pCurrentLine->getUpperPoint().nYCoordinate == pCurrentLine->getLowerPoint().nYCoordinate)
    {
      if(pLine)
      {
        if(pLine->getUpperPoint().nYCoordinate == pLine->getLowerPoint().nYCoordinate)
        {
          if(pCurrentLine->getUpperPoint().nYCoordinate < pLine->getUpperPoint().nYCoordinate)
          {
            pLine = pCurrentLine;
            continue;
          }
        }
      }
      
      pLine = pCurrentLine;
    }

		if(pCurrentLine->getLowerPoint().nXCoordinate > pLine->getLowerPoint().nXCoordinate)
			pLine = pCurrentLine;
	}

	return pLine;
}

bool CAlgorithm::PerformIntersectionTest(CLine* pL1, CLine* pL2, SPACE_POINT &ptIntersection)
{
	ptIntersection.nXCoordinate = 0;
	ptIntersection.nYCoordinate = 0;

	if(pL1 == NULL || pL2 == NULL)
		return false;

  // Check if line L1 is a vertical line.
  bool bLineFirstIsVertical(false);
  SPACE_POINT ptLowerL1 = pL1->getLowerPoint();
	SPACE_POINT ptUpperL1 = pL1->getUpperPoint();

  if(ptLowerL1.nXCoordinate == ptUpperL1.nXCoordinate)
    bLineFirstIsVertical = true;

  // Check if line L2 is a vertical line.
  bool bLineSecondIsVertical(false);
  SPACE_POINT ptLowerL2 = pL2->getLowerPoint();
	SPACE_POINT ptUpperL2 = pL2->getUpperPoint();

  if(ptLowerL2.nXCoordinate == ptUpperL2.nXCoordinate)
    bLineSecondIsVertical = true;

  // Two vertical lines can never intersect each other
  if(bLineFirstIsVertical && bLineSecondIsVertical)
    return false;

  // If either one if them is a vertical line then...
  if(bLineFirstIsVertical && !bLineSecondIsVertical)
  {
    // Calculate X coordinate of intersection point
    ptIntersection.nXCoordinate = ptUpperL1.nXCoordinate;

    // Calculate slope of line L2
	  double fSlopeL2 = 0.0;
    if((ptUpperL2.nXCoordinate - ptLowerL2.nXCoordinate) != 0)
	    fSlopeL2 =  (ptUpperL2.nYCoordinate - ptLowerL2.nYCoordinate) / (ptUpperL2.nXCoordinate - ptLowerL2.nXCoordinate);

    // Calculate y intercept of line L2
    double yInterceptLine2 = 0.0;
    yInterceptLine2 = ptUpperL2.nYCoordinate - (fSlopeL2 * ptUpperL2.nXCoordinate);

    // Calculate Y coordinate of intersection point
    ptIntersection.nYCoordinate = (fSlopeL2 * ptIntersection.nXCoordinate) + yInterceptLine2;
  }
  else
  if(!bLineFirstIsVertical && bLineSecondIsVertical)
  {
    // Calculate X coordinate of intersection point
    ptIntersection.nXCoordinate = ptUpperL2.nXCoordinate;

    // Calculate slope of line L1
	  double fSlopeL1 = 0.0;
    if((ptUpperL1.nXCoordinate - ptLowerL1.nXCoordinate) != 0)
	    fSlopeL1 =  (ptUpperL1.nYCoordinate - ptLowerL1.nYCoordinate) / (ptUpperL1.nXCoordinate - ptLowerL1.nXCoordinate);

    // Calculate y intercept of line L1
    double yInterceptLine1 = 0.0;
    yInterceptLine1 = ptUpperL1.nYCoordinate - (fSlopeL1 * ptUpperL1.nXCoordinate);

    // Calculate Y coordinate of intersection point
    ptIntersection.nYCoordinate = (fSlopeL1 * ptIntersection.nXCoordinate) + yInterceptLine1;
  }
  else
  {
    // Calculate slope of line L1
	  double fSlopeL1 = 0.0;
    if((ptUpperL1.nXCoordinate - ptLowerL1.nXCoordinate) != 0)
	    fSlopeL1 =  (ptUpperL1.nYCoordinate - ptLowerL1.nYCoordinate) / (ptUpperL1.nXCoordinate - ptLowerL1.nXCoordinate);

	  // Calculate slope of line L2
	  double fSlopeL2 = 0.0;
    if((ptUpperL2.nXCoordinate - ptLowerL2.nXCoordinate) != 0)
	    fSlopeL2 =  (ptUpperL2.nYCoordinate - ptLowerL2.nYCoordinate) / (ptUpperL2.nXCoordinate - ptLowerL2.nXCoordinate);

    // Calculate y intercept of line L1
    double yInterceptLine1 = 0.0;
    yInterceptLine1 = ptUpperL1.nYCoordinate - (fSlopeL1 * ptUpperL1.nXCoordinate);

    // Calculate y intercept of line L2
    double yInterceptLine2 = 0.0;
    yInterceptLine2 = ptUpperL2.nYCoordinate - (fSlopeL2 * ptUpperL2.nXCoordinate);

    // Find X-coordinate of intersection point
    ptIntersection.nXCoordinate = (yInterceptLine2 - yInterceptLine1) / (fSlopeL1 - fSlopeL2);

    // Find Y-coordinate of interscetion point
    ptIntersection.nYCoordinate = (fSlopeL1 * ptIntersection.nXCoordinate) + yInterceptLine1;
  }

	// Check if the intersection point exists on both the line segments.
	if(IsPointOnLine(pL1, ptIntersection) && IsPointOnLine(pL2, ptIntersection))
		return true;

	return false;
}

bool CAlgorithm::IsPointOnLine(CLine *pLine, SPACE_POINT ptPoint)
{
	SPACE_POINT ptLineUpper = pLine->getUpperPoint();
	SPACE_POINT ptLineLower = pLine->getLowerPoint();

	CLine tempLine;
	tempLine.SetUpperPoint(ptLineUpper);
	tempLine.SetLowerPoint(ptLineLower);

	if(ptLineUpper.nYCoordinate == ptLineLower.nYCoordinate)
	{
		if(ptLineUpper.nXCoordinate < ptLineLower.nXCoordinate)
		{
			SPACE_POINT ptTemp = ptLineUpper;
			ptLineUpper = ptLineLower;
			ptLineLower = ptTemp;

			CLine tempLine;
			tempLine.SetUpperPoint(ptLineUpper);
			tempLine.SetLowerPoint(ptLineLower);
		}
	}
	
	return(tempLine.PointLiesOnLine(ptPoint));
}
