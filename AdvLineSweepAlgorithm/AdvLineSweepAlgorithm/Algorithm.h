#pragma once
#include "Line.h"
#include "EventQueueData.h"

template<typename NODETYPE> class CAVLTreeNode;
template<typename TREETYPE> class CAVLTree;

class CAlgorithm
{
	public:
		static const UINT MAX_LINES;
		static const UINT MIN_LINES;
		static const UINT	LINE_UPPER_BOUND;
		static const UINT	LINE_LOWER_BOUND;

	private:
		UINT m_nTotalLines;
		CLine* m_pLineArray;
		CAVLTree<CEventQueueData *> *m_pEventQueue;
		CAVLTree<CLine *> *m_pStatusStructure;
						
	public:
		CAlgorithm(void);
		virtual ~CAlgorithm(void);
		void Initialize();
		void FindIntersections();

	private:
		void GenerateTotalNoOfLines();
		void GenerateRandomLines();
		void HandleEventPoint(CEventQueueData* pEventData);
		void FindNewEvent(CLine* pLeftLine, CLine* pRightLine, CEventQueueData* pEventData);
		void GetLineSet(POINT_POSITION ptPosition, CArray<CLine *, CLine *>& resultSet, SPACE_POINT ptReference);
		void GetLineSetUnion(CArray<CLine *, CLine *>* arrSet1, CArray<CLine *, CLine *>* arrSet2, CArray<CLine *, CLine *>* arrSet3, CArray<CLine *, CLine *>& resultSet);
		void GetLineSetUnion(CArray<CLine *, CLine *>* arrSet1, CArray<CLine *, CLine *>* arrSet2, CArray<CLine *, CLine *>& resultSet);
		bool LineArrayContainsValue(const CArray<CLine *, CLine *>*, CLine *);
		CLine* GetLeftMostSegInLineSet(CArray<CLine*, CLine*> *arrLineSet);
		CLine* GetRightMostSegInLineSet(CArray<CLine*, CLine*> *arrLineSet);
		bool PerformIntersectionTest(CLine* pL1, CLine* pL2, SPACE_POINT &ptIntersection);
		bool IsPointOnLine(CLine *pLine, SPACE_POINT ptPoint);
};
