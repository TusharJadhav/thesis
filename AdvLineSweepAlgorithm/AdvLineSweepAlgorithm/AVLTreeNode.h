#pragma once
#include "EventQueueData.h"

template<typename NODETYPE> class CAVLTree;

template <typename NODETYPE>
class CAVLTreeNode
{
	private:
		NODETYPE m_data;
		CAVLTreeNode* m_pLeftChild;
		CAVLTreeNode* m_pRightChild;
		CAVLTreeNode* m_pParentNode;
    UINT m_nBalanceFactor;
    CAVLTree<NODETYPE>* m_pTree;

    CAVLTreeNode* m_pAdjLeftNode;
    CAVLTreeNode* m_pAdjRightNode;

	public:
    // Constructor
    template <typename NODETYPE>
    CAVLTreeNode(CAVLTree<NODETYPE>* pTree, 
                 NODETYPE data, 
                 UINT nBalanceFactor = 1, 
                 CAVLTreeNode* pLeftChild = NULL, 
                 CAVLTreeNode* pRightChild = NULL, 
                 CAVLTreeNode* pParentNode = NULL)
    {
      m_pTree = pTree;
      m_data = data;
      m_nBalanceFactor = nBalanceFactor;
      m_pLeftChild = pLeftChild;
      m_pRightChild = pRightChild;
      m_pParentNode = pParentNode;

			if(g_bOptimizedMode)
			{
				m_pAdjLeftNode = 0;
				m_pAdjRightNode = 0;
			}
    }

    template <>
    CAVLTreeNode(CAVLTree<CEventQueueData*>* pTree, 
                 CEventQueueData* data, 
                 UINT nBalanceFactor, 
                 CAVLTreeNode* pLeftChild, 
                 CAVLTreeNode* pRightChild, 
                 CAVLTreeNode* pParentNode)
    {
      m_pTree = pTree;
      m_data = new CEventQueueData(*data);
      m_nBalanceFactor = nBalanceFactor;
      m_pLeftChild = pLeftChild;
      m_pRightChild = pRightChild;
      m_pParentNode = pParentNode;

			if(g_bOptimizedMode)
			{
				m_pAdjLeftNode = 0;
				m_pAdjRightNode = 0;
			}
    }

    template <>
    CAVLTreeNode(CAVLTree<CLine*>* pTree, 
                 CLine* data, 
                 UINT nBalanceFactor, 
                 CAVLTreeNode* pLeftChild, 
                 CAVLTreeNode* pRightChild, 
                 CAVLTreeNode* pParentNode)
    {
      m_pTree = pTree;
      m_data = new CLine();
      m_data->SetUpperPoint(data->getUpperPoint());
      m_data->SetLowerPoint(data->getLowerPoint());
      m_nBalanceFactor = nBalanceFactor;
      m_pLeftChild = pLeftChild;
      m_pRightChild = pRightChild;
      m_pParentNode = pParentNode;

			if(g_bOptimizedMode)
			{
				m_pAdjLeftNode = 0;
				m_pAdjRightNode = 0;
			}
    }

    template <typename NODETYPE>
    void setData(NODETYPE data)
    {
    }

    template <>
    void setData(CEventQueueData* data)
    {
      SAFEDELETE(m_data);
      m_data = new CEventQueueData(*data);
    }

    template <>
    void setData(CLine* data)
    {
      SAFEDELETE(m_data);
      m_data = new CLine();
      m_data->SetUpperPoint(data->getUpperPoint());
      m_data->SetLowerPoint(data->getLowerPoint());
    }

    template <typename NODETYPE>
    void getData(NODETYPE *ppData)
    {
    }

    template <>
    void getData(CEventQueueData **ppData)
    {
      if(!ppData)
        return;

      (*ppData) = 0;

      CEventQueueData *pEventQueueData = new CEventQueueData(*m_data);
      (*ppData) = pEventQueueData;
    }

    template <>
    void getData(CLine **ppData)
    {
      if(!ppData)
        return;

      (*ppData) = 0;

      CLine* pLineData = new CLine(*m_data);
      (*ppData) = pLineData;
    }

    // Destructor
		virtual ~CAVLTreeNode(void);

    // Getter methods
		CAVLTreeNode<NODETYPE>* getLeftChild();
		CAVLTreeNode<NODETYPE>* getRightChild();
		CAVLTreeNode<NODETYPE>* getParentNode();

    CAVLTreeNode<NODETYPE>* getAdjacentLeftNode();
    CAVLTreeNode<NODETYPE>* getAdjacentRightNode();
    void setAdjacentLeftNode(CAVLTreeNode<NODETYPE>* pNewAdjLeftNode);
    void setAdjacentRightNode(CAVLTreeNode<NODETYPE>* pNewAdjRightNode);

		int getBalanceFactor();

    // Setter methods
    void setBalanceFactor(int nBalanceFactor);
		void setParentNode(CAVLTreeNode<NODETYPE>* pNode);
		void setLeftChild(CAVLTreeNode<NODETYPE>* pNode);
		void setRightChild(CAVLTreeNode<NODETYPE>* pNode);

    // Other methods
    bool IsRoot();
		bool isLeftSibling();
		bool isRightSibling();
		bool isLeaf();
		bool hasSibling();
    COMPARISON_RESULT insert(NODETYPE newData);

		template <typename NODETYPE>
		COMPARISON_RESULT compare(NODETYPE dataToCompare)
		{
			if((*dataToCompare) > (*m_data))
				return MORE_THAN;
			else
			if((*dataToCompare) < (*m_data))
				return LESS_THAN;
			else
				return EQUAL_TO;
		}

    template <>
		COMPARISON_RESULT compare(CEventQueueData *dataToCompare)
		{
			if(dataToCompare->getPoint().nYCoordinate < m_data->getPoint().nYCoordinate)
				return MORE_THAN;
			else
			if(dataToCompare->getPoint().nYCoordinate > m_data->getPoint().nYCoordinate)
				return LESS_THAN;
			else
			{
				if(dataToCompare->getPoint().nXCoordinate < m_data->getPoint().nXCoordinate)
					return LESS_THAN;
				else
				if(dataToCompare->getPoint().nXCoordinate > m_data->getPoint().nXCoordinate)
					return MORE_THAN;
				else
					return EQUAL_TO;
			}
		}

		template <>
		COMPARISON_RESULT compare(CLine *dataToCompare)
		{
      ASSERT(m_pTree);
      if(!m_pTree)
        return UNDEFINED;

      SPACE_POINT ptToCompare;
      SPACE_POINT ptReference;

      SPACE_POINT ptEventPoint;
      ptEventPoint = m_pTree->GetCurrentlyHandledEventPoint();

      if(dataToCompare->getUpperPoint().nYCoordinate < ptEventPoint.nYCoordinate)
        ptToCompare = dataToCompare->getUpperPoint();
      else
      if(dataToCompare->getUpperPoint().nYCoordinate == ptEventPoint.nYCoordinate && 
        dataToCompare->getUpperPoint().nXCoordinate >= ptEventPoint.nXCoordinate)
        ptToCompare = dataToCompare->getUpperPoint();
      else
        ptToCompare = dataToCompare->getLowerPoint();

      if(m_data->getUpperPoint().nYCoordinate >= ptEventPoint.nYCoordinate &&
         m_data->getUpperPoint().nXCoordinate >= ptEventPoint.nXCoordinate)
        ptReference = m_data->getUpperPoint();
      else
        ptReference = m_data->getLowerPoint();
      
      if(ptToCompare.nXCoordinate < ptReference.nXCoordinate)
				return LESS_THAN;
			else
			if(ptToCompare.nXCoordinate > ptReference.nXCoordinate)
				return MORE_THAN;
      else
      if(ptToCompare.nYCoordinate < ptReference.nYCoordinate)
        return MORE_THAN;
      else
      if(ptToCompare.nYCoordinate > ptReference.nYCoordinate)
        return LESS_THAN;
			else
				return EQUAL_TO;
		}

    template <typename NODETYPE>
		COMPARISON_RESULT compareToRemoveNode(NODETYPE dataToCompare)
		{
			if(dataToCompare > m_data)
				return MORE_THAN;
			else
			if(dataToCompare < m_data)
				return LESS_THAN;
			else
				return EQUAL_TO;
		}

    template <>
		COMPARISON_RESULT compareToRemoveNode(CEventQueueData *dataToCompare)
		{
			if(dataToCompare->stPt.nYCoordinate < m_data->stPt.nYCoordinate)
				return MORE_THAN;
			else
			if(dataToCompare->stPt.nYCoordinate > m_data->stPt.nYCoordinate)
				return LESS_THAN;
			else
			{
				if(dataToCompare->stPt.nXCoordinate < m_data->stPt.nXCoordinate)
					return LESS_THAN;
				else
				if(dataToCompare->stPt.nXCoordinate > m_data->stPt.nXCoordinate)
					return MORE_THAN;
				else
					return EQUAL_TO;
      }
		}

    template <>
		COMPARISON_RESULT compareToRemoveNode(CLine *dataToCompare)
		{
      ASSERT(m_pTree);
      if(!m_pTree)
        return UNDEFINED;

      SPACE_POINT ptReference = m_data->getUpperPoint();
      SPACE_POINT ptToCompare = dataToCompare->getUpperPoint();

      if(ptToCompare.nXCoordinate < ptReference.nXCoordinate)
				return LESS_THAN;
			else
			if(ptToCompare.nXCoordinate > ptReference.nXCoordinate)
				return MORE_THAN;
      else
			if(ptToCompare.nXCoordinate == ptReference.nXCoordinate)
      {
        if(ptToCompare.nYCoordinate < ptReference.nYCoordinate)
          return LESS_THAN;
        else
        if(ptToCompare.nYCoordinate > ptReference.nYCoordinate)
          return MORE_THAN;
      }
      return EQUAL_TO;
		}
};
