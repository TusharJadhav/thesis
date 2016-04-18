#pragma once

#include "StdAfx.h"
#include "AVLTree.h"
#include "AVLTreeNode.cpp"
#include "AVLTreeIterator.cpp"

#include <iostream>
#include <utility>
#include <algorithm>
#include <list>


template <typename TREETYPE>
int CAVLTree<TREETYPE>::nTempHeight = 1;

template <typename TREETYPE>
CAVLTree<TREETYPE>::CAVLTree(void):
	m_pRoot(0)
{
  m_ptCurrentEventPoint.nXCoordinate = -1;
  m_ptCurrentEventPoint.nYCoordinate = -1;
}

template <typename TREETYPE>
CAVLTree<TREETYPE>::~CAVLTree(void)
{
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::insert(TREETYPE newData)
{
	// if the tree is empty then insert the new node as the root node.
	if(!m_pRoot)
	{
		m_pRoot = new CAVLTreeNode<TREETYPE>(this, newData, 1, 0, 0, 0);
		return;
	}

	// Get the appropriate node to insert.
	CAVLTreeNode<TREETYPE>* pPivotNodeToInsert = 0;
  pPivotNodeToInsert = getNodeToInsert(newData);

  if(!pPivotNodeToInsert)
    return;

	// Insert the node at the appropriate position in the tree.
  COMPARISON_RESULT enumInsertedPos = pPivotNodeToInsert->insert(newData);
  if(enumInsertedPos == UNDEFINED)
    return;

  TREETYPE pPivotData = 0;
  pPivotNodeToInsert->getData(&pPivotData);
  if(!pPivotData)
    return;

	CAVLTreeNode<TREETYPE>* pPivotsAdjLeft = 0;
	CAVLTreeNode<TREETYPE>* pPivotsAdjRight = 0;

	if(g_bOptimizedMode)
	{
		pPivotsAdjLeft = pPivotNodeToInsert->getAdjacentLeftNode();
		pPivotsAdjRight = pPivotNodeToInsert->getAdjacentRightNode();

		// Set the pivots node adjacent left and right nodes pointer NULL as now it wont be a left node
		pPivotNodeToInsert->setAdjacentLeftNode(0);
		pPivotNodeToInsert->setAdjacentRightNode(0);
	}

  CAVLTreeNode<TREETYPE>* pNewNode = 0;
  pNewNode = new CAVLTreeNode<TREETYPE>(this, pPivotData, 1, 0, 0, pPivotNodeToInsert);

  CAVLTreeNode<TREETYPE>* pNewlyInsertedNode = 0;
	
  if(enumInsertedPos == LESS_THAN || enumInsertedPos == EQUAL_TO)
  {
		if(g_bOptimizedMode)
			pNewlyInsertedNode = pPivotNodeToInsert->getLeftChild();

    if(!pPivotNodeToInsert->getRightChild())
    {
      pPivotNodeToInsert->setRightChild(pNewNode);
      pPivotNodeToInsert->setData(newData);

			if(g_bOptimizedMode)
			{
				if(pNewlyInsertedNode)
				{
					pNewlyInsertedNode->setAdjacentLeftNode(pPivotsAdjLeft);
					pNewlyInsertedNode->setAdjacentRightNode(pNewNode);
				}

				if(pNewNode)
				{
					pNewNode->setAdjacentLeftNode(pNewlyInsertedNode);
					pNewNode->setAdjacentRightNode(pPivotsAdjRight);
				}
			}
    }
    else
    {
			if(g_bOptimizedMode)
			{
				CAVLTreeNode<TREETYPE> *pRightNode = 0;
				pRightNode = pPivotNodeToInsert->getRightChild();

				if(pNewlyInsertedNode)
				{
					pNewlyInsertedNode->setAdjacentLeftNode(pPivotsAdjLeft);
					pNewlyInsertedNode->setAdjacentRightNode(pRightNode);
				}

				if(pRightNode)
				{
					pRightNode->setAdjacentLeftNode(pNewlyInsertedNode);
					pRightNode->setAdjacentRightNode(pPivotsAdjRight);
				}
			}
    }
  }
  else if(enumInsertedPos == MORE_THAN)
  {
		if(g_bOptimizedMode)
			pNewlyInsertedNode = pPivotNodeToInsert->getRightChild();

    if(!pPivotNodeToInsert->getLeftChild())
    {
      pPivotNodeToInsert->setLeftChild(pNewNode);

      CAVLTreeIterator<TREETYPE> *pTreeIterator = 0;
      pTreeIterator = getIterator();
      if(!pTreeIterator)
      {
        SAFEDELETE(pPivotData);
        return;
      }

      pTreeIterator->setCurrentNode(pPivotNodeToInsert);

      CAVLTreeNode<TREETYPE>* pLeftMostParent = 0;
      pLeftMostParent = pTreeIterator->getLeftMostParent();
      
      CAVLTreeNode<TREETYPE> *pParentOfLeftMostParent = 0;
      if(pLeftMostParent)
        pParentOfLeftMostParent = pLeftMostParent->getParentNode();

      if(pParentOfLeftMostParent)
        pParentOfLeftMostParent->setData(newData);

			if(g_bOptimizedMode)
			{
				CAVLTreeNode<TREETYPE> *pLeftNode = 0;
				pLeftNode = pPivotNodeToInsert->getLeftChild();

				if(pLeftNode)
				{
					pLeftNode->setAdjacentLeftNode(pPivotsAdjLeft);
					pLeftNode->setAdjacentRightNode(pNewlyInsertedNode);
				}

				if(pNewlyInsertedNode)
				{
					pNewlyInsertedNode->setAdjacentLeftNode(pLeftNode);
					pNewlyInsertedNode->setAdjacentRightNode(pPivotsAdjRight);
				}
			}

      SAFEDELETE(pTreeIterator);
    }
    else
    {
			if(g_bOptimizedMode)
			{
				CAVLTreeNode<TREETYPE> *pLeftNode = 0;
				pLeftNode = pPivotNodeToInsert->getLeftChild();

				if(pLeftNode)
				{
					pLeftNode->setAdjacentLeftNode(pPivotsAdjLeft);
					pLeftNode->setRightChild(pNewlyInsertedNode);
				}

				if(pNewlyInsertedNode)
				{
					pNewlyInsertedNode->setAdjacentLeftNode(pLeftNode);
					pNewlyInsertedNode->setAdjacentRightNode(pPivotsAdjRight);
				}
			}
    }
  }

  SAFEDELETE(pPivotData);

  // Recaluclate the BalanceFactor from the parent of newly inserted node till root node.
	Restructure(pPivotNodeToInsert, false);
}

template <typename TREETYPE>
CAVLTreeIterator<TREETYPE>* CAVLTree<TREETYPE>::getIterator(void)
{
	CAVLTreeIterator<TREETYPE> *pTreeIterator = 0;
	pTreeIterator = new CAVLTreeIterator<TREETYPE>(this);
	return pTreeIterator;
}

template <typename TREETYPE>
CAVLTreeNode<TREETYPE>* CAVLTree<TREETYPE>::getNodeToInsert(TREETYPE newData)
{
	if(!m_pRoot)
		return 0;

	CAVLTreeNode<TREETYPE>* pTempAVLNode = 0;
	CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
	pTreeIterator = getIterator();

	if(!pTreeIterator)
    return 0;
	
	pTempAVLNode = pTreeIterator->getCurrentNode();
	
	while(pTempAVLNode != 0)
	{
		COMPARISON_RESULT enumResult = pTempAVLNode->compare(newData);

		if( (enumResult == LESS_THAN || enumResult == EQUAL_TO) && pTempAVLNode->getLeftChild() == 0)
		{
			// The new node needs to be inserted in the left child of the current value of the
			// temporary node.
			break;
		}
		else
		if( (enumResult == LESS_THAN || enumResult == EQUAL_TO) && pTempAVLNode->getLeftChild() != 0)
		{
			pTreeIterator->moveLeft();
		}
		else
		if( enumResult == MORE_THAN && pTempAVLNode->getRightChild() == 0)
		{
			// The new node needs to be inserted in the right child of the current value of the
			// temporary node.
			break;
		}
		else
		if( enumResult == MORE_THAN && pTempAVLNode->getRightChild() != 0)
		{
			pTreeIterator->moveRight();
		}

		pTempAVLNode = pTreeIterator->getCurrentNode();
	} // while

  SAFEDELETE(pTreeIterator);

	return pTempAVLNode;
}

template <typename TREETYPE>
int CAVLTree<TREETYPE>::CalculateMaxHeight(CAVLTreeNode<TREETYPE>* pNode)
{
  // nTempHeight is a static variable which is used inside calculateMaxDept. It's important to be initialized
  // here to one.
	CAVLTree<TREETYPE>::nTempHeight = 1;
	int nHeight = 1;
	calculateMaxDept(pNode, nHeight);

	return nHeight;
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::calculateMaxDept(CAVLTreeNode<TREETYPE> *pNode, int &nHeight)
{
	if(!pNode)
		return;

	int nCnt = 1;
	CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
	pTreeIterator = getIterator();
	bool bLoopBreakFlag = false;

	if(pTreeIterator)
	{
		pTreeIterator->setCurrentNode(pNode);
		pTreeIterator->moveToLeftMostLeaf();
		CAVLTreeNode<TREETYPE>* pCurrentNode = 0;
		pCurrentNode = pTreeIterator->getCurrentNode();
		CAVLTreeNode<TREETYPE>* pLatestLeafNode = 0;
		pLatestLeafNode = pTreeIterator->getCurrentNode();
	
		while(pCurrentNode != 0)
		{
			while(pCurrentNode != pNode)
			{
				if(pCurrentNode->getParentNode() == 0)
				{
					bLoopBreakFlag = true;
					break;
				}

				nCnt++;
				pTreeIterator->moveUp();
				pCurrentNode = pTreeIterator->getCurrentNode();

				if(pCurrentNode->IsRoot())
				{
					bLoopBreakFlag = true;
					break;
				}
			}

			if(pCurrentNode == pNode)
			{
				if(nCnt > CAVLTree<TREETYPE>::nTempHeight)
					CAVLTree<TREETYPE>::nTempHeight = nCnt;

				nCnt = 1;

				pTreeIterator->setCurrentNode(pLatestLeafNode);
				pTreeIterator->moveToAdjacentRightLeaf();
				
				if(pTreeIterator->getCurrentNode() == pLatestLeafNode)
					break;
				else
					pLatestLeafNode = pTreeIterator->getCurrentNode();
			}

			if(bLoopBreakFlag == true)
				break;

			if(pTreeIterator->IsRightMostLeafOfTree() == true)
				bLoopBreakFlag = true;

			pCurrentNode = pTreeIterator->getCurrentNode();
		}
	}

  SAFEDELETE(pTreeIterator);
  
	nHeight = CAVLTree<TREETYPE>::nTempHeight;
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::Rotate(CAVLTreeNode<TREETYPE>* pNode, int nBalanceFactor, bool bAfterDeletingNode)
{
	if(!pNode)
		return;

	CAVLTreeNode<TREETYPE>* pRightChild = 0;
	CAVLTreeNode<TREETYPE>* pLeftChild = 0;

	pRightChild = pNode->getRightChild();
	pLeftChild = pNode->getLeftChild();

	if(!pLeftChild && !pRightChild)
		return;

	int nLeftBF = 0;
	int nRightBF = 0;

	if(pLeftChild)
		nLeftBF = pLeftChild->getBalanceFactor();

	if(pRightChild)
		nRightBF = pRightChild->getBalanceFactor();

	if(nBalanceFactor == -2)
	{
		if(nRightBF == -1)
		{
			// Perform Left Rotation.
			PerformLeftRotation(pNode);
		}
		else
		if(nRightBF == 1)
		{
			// Perform Right Left Rotation.
			PerformRightRotation(pNode->getRightChild());
			PerformLeftRotation(pNode);
		}
		
		// These operations are specific only if the node is deleted.
		/*if(nLeftBF == 0 && bAfterDeletingNode)
		{
			PerformRightRotation(pNode);
		}*/
	}
	else
	if(nBalanceFactor == 2)
	{
		if(nLeftBF == 1)
		{
			// Perform Right Rotation.
			PerformRightRotation(pNode);
		}
		else
		if(nLeftBF == -1)
		{
			// Perform Left Right Rotation.
			PerformLeftRotation(pNode->getLeftChild());
			PerformRightRotation(pNode);
		}
		
		// These operations are specific only if the node is deleted.
		/*if(nRightBF == 0 && bAfterDeletingNode)
		{
			PerformLeftRotation(pNode);
		}*/
	}
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::PerformLeftRotation(CAVLTreeNode<TREETYPE>* pRotatingNode)
{
  if(!pRotatingNode)
    return;

  bool bLeftSibling(false);
  bLeftSibling = pRotatingNode->isLeftSibling();

	CAVLTreeNode<TREETYPE>* pRightChild = 0;
	pRightChild = pRotatingNode->getRightChild();

	CAVLTreeNode<TREETYPE>* pParent = 0;
	pParent = pRotatingNode->getParentNode();

  CAVLTreeNode<TREETYPE>* pRightChildsLeftChild = 0;
  if(pRightChild)
    pRightChildsLeftChild = pRightChild->getLeftChild();

  if(pRightChild)
    pRightChild->setParentNode(pParent);


  if(pParent)
    bLeftSibling ? pParent->setLeftChild(pRightChild) : pParent->setRightChild(pRightChild);
  else
    m_pRoot = pRightChild;

  if(pRightChild)
    pRightChild->setLeftChild(pRotatingNode);

  pRotatingNode->setParentNode(pRightChild);
  pRotatingNode->setRightChild(pRightChildsLeftChild);
  if(pRightChildsLeftChild)
    pRightChildsLeftChild->setParentNode(pRotatingNode);

  CalculateBalanceFactor(pRotatingNode);
	CalculateBalanceFactor(pRightChild);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::PerformRightRotation(CAVLTreeNode<TREETYPE>* pRotatingNode)
{
  if(!pRotatingNode)
    return;

  bool bRightSibling(false);
  bRightSibling = pRotatingNode->isRightSibling();

	CAVLTreeNode<TREETYPE>* pLeftChild = 0;
	pLeftChild = pRotatingNode->getLeftChild();

	CAVLTreeNode<TREETYPE>* pParent = 0;
	pParent = pRotatingNode->getParentNode();

  CAVLTreeNode<TREETYPE>* pLeftChildsRightChild = 0;
  if(pLeftChild)
    pLeftChildsRightChild = pLeftChild->getRightChild();

	if(pLeftChild)
		pLeftChild->setParentNode(pParent);

	if(pParent)
    bRightSibling ? pParent->setRightChild(pLeftChild) : pParent->setLeftChild(pLeftChild);
  else
    m_pRoot = pLeftChild;

  if(pLeftChild)
	  pLeftChild->setRightChild(pRotatingNode);

	pRotatingNode->setParentNode(pLeftChild);
	pRotatingNode->setLeftChild(pLeftChildsRightChild);
	if(pLeftChildsRightChild)
		pLeftChildsRightChild->setParentNode(pRotatingNode);

  CalculateBalanceFactor(pRotatingNode);
	CalculateBalanceFactor(pLeftChild);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::CalculateBalanceFactor(CAVLTreeNode<TREETYPE>* pNode)
{
	if(pNode == 0)
		return;

	CAVLTreeNode<TREETYPE>* pLeftChild = pNode->getLeftChild();
	CAVLTreeNode<TREETYPE>* pRightChild = pNode->getRightChild();

	if(pLeftChild == 0 && pRightChild == 0)
		pNode->setBalanceFactor(1);
	else
	{
		if(pLeftChild != 0)
			CalculateBalanceFactor(pLeftChild);

		if(pRightChild != 0)
			CalculateBalanceFactor(pRightChild);

		int nLeftHt = 1;
		if(pLeftChild != 0)
			nLeftHt = CalculateMaxHeight(pLeftChild);
		else
			nLeftHt = 0;

		int nRightHt = 1;
		if(pRightChild != 0)
			nRightHt = CalculateMaxHeight(pRightChild);
		else
			nRightHt = 0;

		int nBalance = nLeftHt - nRightHt;
		pNode->setBalanceFactor(nBalance);
	}
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::remove(TREETYPE deleteData)
{
	if(!m_pRoot)
		return;

	CAVLTreeNode<TREETYPE>* pNode = 0;
	pNode = getNodeToRemove(deleteData);
  if(!pNode)
    return;

  if(pNode == m_pRoot)
  {
    ASSERT(!pNode->getLeftChild());
    ASSERT(!pNode->getRightChild());
    SAFEDELETE(pNode);
    m_pRoot = 0;
    return;
  }

  CAVLTreeNode<TREETYPE>* pParent = 0;
	pParent = pNode->getParentNode();

	CAVLTreeNode<TREETYPE>* pLeftOfNodeToDelete = 0;
  CAVLTreeNode<TREETYPE>* pRightOfNodeToDelete = 0;

	if(g_bOptimizedMode)
	{
		pLeftOfNodeToDelete = pNode->getAdjacentLeftNode();
		pRightOfNodeToDelete = pNode->getAdjacentRightNode();
	}

  if(pNode->isLeftSibling() && pParent)
  {
    CAVLTreeNode<TREETYPE>* pGrandParent = 0;
    pGrandParent = pParent->getParentNode();

    if(!pGrandParent)
    {
      // Parent node is the root node.
      CAVLTreeNode<TREETYPE>* pRootNodesRightChild = 0;
      pRootNodesRightChild = m_pRoot->getRightChild();
      
			if(g_bOptimizedMode)
			{
				pRootNodesRightChild->setAdjacentLeftNode(0);
				pRootNodesRightChild->setAdjacentRightNode(0);
			}

      m_pRoot = pRootNodesRightChild;
			if(pRootNodesRightChild)
				pRootNodesRightChild->setParentNode(NULL);

      SAFEDELETE(pNode);
      SAFEDELETE(pParent);
      Restructure(m_pRoot);
      return;
    }
    else
    {
      CAVLTreeNode<TREETYPE>* pParentsRightNode = 0;
      pParentsRightNode = pParent->getRightChild();

      if(pParent->isRightSibling())
        pGrandParent->setRightChild(pParentsRightNode);
      else
        pGrandParent->setLeftChild(pParentsRightNode);

      if(pParentsRightNode)
        pParentsRightNode->setParentNode(pGrandParent);

			if(g_bOptimizedMode)
			{
				if(pParentsRightNode)
					pParentsRightNode->setAdjacentLeftNode(pLeftOfNodeToDelete);

				if(pLeftOfNodeToDelete)
					pLeftOfNodeToDelete->setAdjacentRightNode(pParentsRightNode);
			}
			
      SAFEDELETE(pNode);
      SAFEDELETE(pParent);
      Restructure(pParentsRightNode);
      return;
    }
  }
  else if(pNode->isRightSibling() && pParent)
  {
    CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
    pTreeIterator = getIterator();
    if(!pTreeIterator)
      return;

    pTreeIterator->setCurrentNode(pNode);

    if(pTreeIterator->IsRightMostLeafOfTree())
    {
      SAFEDELETE(pNode);
      pParent->setRightChild(0);
      
      CAVLTreeNode<TREETYPE> *pParentLeftNode = 0;
      pParentLeftNode = pParent->getLeftChild();

      if(pParentLeftNode && pParentLeftNode->isLeaf())
      {
				if((pParent != m_pRoot) && g_bOptimizedMode)
        {
          pParent->setAdjacentRightNode(0);
          if(pLeftOfNodeToDelete)
            pParent->setAdjacentLeftNode(pLeftOfNodeToDelete->getAdjacentLeftNode());
          else
            pParent->setAdjacentLeftNode(0);
        }

        SAFEDELETE(pParentLeftNode);
        pParent->setLeftChild(0);
      }
      else
      {
				if(pLeftOfNodeToDelete && g_bOptimizedMode)
          pLeftOfNodeToDelete->setAdjacentRightNode(0);
      }
      
      Restructure(pParent);
      SAFEDELETE(pTreeIterator);
      return;
    }

    CAVLTreeNode<TREETYPE>* pLeftMostParent = 0;
    pLeftMostParent = pTreeIterator->getLeftMostParent();
    if(!pLeftMostParent)
    {
      SAFEDELETE(pTreeIterator);
      return;
    }

    CAVLTreeNode<TREETYPE>* pParentOfLeftMostParent = 0;
    if(pLeftMostParent)
      pParentOfLeftMostParent = pLeftMostParent->getParentNode();
    if(!pParentOfLeftMostParent)
    {
      SAFEDELETE(pTreeIterator);
      return;
    }

    if(pParentOfLeftMostParent == m_pRoot)
    {
      CAVLTreeNode<TREETYPE>* pRootsRightChild = 0;
      pRootsRightChild = m_pRoot->getRightChild();

      pParent->setRightChild(pRootsRightChild);
      if(pRootsRightChild)
        pRootsRightChild->setParentNode(pParent);

      m_pRoot = pLeftMostParent;
      pLeftMostParent->setParentNode(NULL);

			if(g_bOptimizedMode)
			{
				if(pRightOfNodeToDelete)
					pRightOfNodeToDelete->setAdjacentLeftNode(pLeftOfNodeToDelete);
				
				if(pLeftOfNodeToDelete)
					pLeftOfNodeToDelete->setAdjacentRightNode(pRightOfNodeToDelete);
			}

      SAFEDELETE(pNode);
      SAFEDELETE(pParentOfLeftMostParent);
      Restructure(pParent);
      SAFEDELETE(pTreeIterator);
      return;
    }
    else
    {
      CAVLTreeNode<TREETYPE> *pNodeA = 0;
      pNodeA = pParentOfLeftMostParent->getParentNode();
      if(!pNodeA)
      {
        SAFEDELETE(pTreeIterator);
        return;
      }

      CAVLTreeNode<TREETYPE> *pNodeB = 0;
      pNodeB = pParentOfLeftMostParent->getRightChild();

      pParent->setRightChild(pNodeB);
      if(pNodeB)
        pNodeB->setParentNode(pParent);
      else
      {
        CAVLTreeNode<TREETYPE> *pLeftChildOfParent = 0;
        pLeftChildOfParent = pParent->getLeftChild();
        SAFEDELETE(pLeftChildOfParent);
        pParent->setLeftChild(0);
      }

      pLeftMostParent->setParentNode(pNodeA);
      if(pParentOfLeftMostParent->isLeftSibling())
        pNodeA->setLeftChild(pLeftMostParent);
      else
        pNodeA->setRightChild(pLeftMostParent);

			if(g_bOptimizedMode)
			{
				if(pLeftOfNodeToDelete)
					pLeftOfNodeToDelete->setAdjacentRightNode(pRightOfNodeToDelete);

				if(pRightOfNodeToDelete)
					pRightOfNodeToDelete->setAdjacentLeftNode(pLeftOfNodeToDelete);
			}

      SAFEDELETE(pNode);
      SAFEDELETE(pParentOfLeftMostParent);
      Restructure(pParent);
      SAFEDELETE(pTreeIterator);
      return;
    }
  }
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::DeleteNodeSafely(CAVLTreeNode<TREETYPE> *pNode)
{
	if(!pNode)
    return;
	
	pNode->setParentNode(0);
	pNode->setLeftChild(0);
	pNode->setRightChild(0);

	if (g_bOptimizedMode)
	{
		  pNode->setAdjacentLeftNode(0);
		  pNode->setAdjacentRightNode(0);
	}
  
  SAFEDELETE(pNode);
}

template <typename TREETYPE>
CAVLTreeNode<TREETYPE>* CAVLTree<TREETYPE>::getNodeToRemove(TREETYPE deleteData)
{
	if(!m_pRoot)
		return 0;

	CAVLTreeNode<TREETYPE>* pTempAVLNode = 0;
	CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
	pTreeIterator = getIterator();

	if(!pTreeIterator)
    return 0;
	
  pTreeIterator->movetoLeftMostLeafOfTree();
	pTempAVLNode = pTreeIterator->getCurrentNode();

  while(pTempAVLNode)
  {
    COMPARISON_RESULT enumResult = pTempAVLNode->compareToRemoveNode(deleteData);
    if(enumResult == EQUAL_TO)
      break;

    pTreeIterator->moveToAdjacentRightLeaf();
    pTempAVLNode = pTreeIterator->getCurrentNode();
  }

  SAFEDELETE(pTreeIterator);

  return pTempAVLNode;
}

template <typename TREETYPE>
CAVLTreeNode<TREETYPE>* CAVLTree<TREETYPE>::GetRightMostChild(CAVLTreeNode<TREETYPE>* pNode)
{
	if(pNode == 0)
		return 0;

	CAVLTreeNode<TREETYPE>* pRightMostNode = pNode;
	
	CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
	pTreeIterator = getIterator();
  if(!pTreeIterator)
    return 0;

	pTreeIterator->setCurrentNode(pRightMostNode);

	while(pRightMostNode->getRightChild() != 0)
	{
		pTreeIterator->moveRight();
		pRightMostNode = pTreeIterator->getCurrentNode();
	}

  SAFEDELETE(pTreeIterator);

	return pRightMostNode;
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::Restructure(CAVLTreeNode<TREETYPE>* pNode, bool bAfterDeletingNode)
{
	CAVLTreeIterator<TREETYPE>* pTreeIterator = 0;
	pTreeIterator = getIterator();
	if(!pTreeIterator)
    return;

  pTreeIterator->setCurrentNode(pNode);
	
	CAVLTreeNode<TREETYPE>* pCurrentNode = 0;
	pCurrentNode = pTreeIterator->getCurrentNode();
		
	while(pCurrentNode != 0)
	{
		int nLeftHeight = 1;
		int nRightHeight = 1;

		if(pCurrentNode->getLeftChild() == 0)
			nLeftHeight = 0;
		else
			nLeftHeight = CalculateMaxHeight(pCurrentNode->getLeftChild());

		if(pCurrentNode->getRightChild() == 0)
			nRightHeight = 0;
		else
			nRightHeight = CalculateMaxHeight(pCurrentNode->getRightChild());

		int nBalanceFactor = nLeftHeight - nRightHeight;
		if(nBalanceFactor == -2 || nBalanceFactor == 2)
		{
			Rotate(pCurrentNode, nBalanceFactor, bAfterDeletingNode);
			break;
		}

		pCurrentNode->setBalanceFactor(nBalanceFactor);

		pTreeIterator->moveUp();
		pCurrentNode = pTreeIterator->getCurrentNode();
	} // while

  SAFEDELETE(pTreeIterator);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::SetCurrentlyHandledEventPoint(SPACE_POINT ptEventPoint)
{
  m_ptCurrentEventPoint = ptEventPoint;
}

template <typename TREETYPE>
SPACE_POINT CAVLTree<TREETYPE>::GetCurrentlyHandledEventPoint()
{
  return m_ptCurrentEventPoint;
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::empty()
{
  if(!m_pRoot)
    return;

  DeleteRecursively(m_pRoot);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::DeleteRecursively(CAVLTreeNode<TREETYPE>* pNode)
{
  if(!pNode)
    return;

  CAVLTreeNode<TREETYPE>* pLeftNode = 0;
  CAVLTreeNode<TREETYPE>* pRightNode = 0;

  pLeftNode = pNode->getLeftChild();
  pRightNode = pNode->getRightChild();

  DeleteNodeSafely(pNode);

  DeleteRecursively(pLeftNode);
  DeleteRecursively(pRightNode);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::PrintTree()
{
	Print(m_pRoot, 0);
}

template <typename TREETYPE>
void CAVLTree<TREETYPE>::Print(CAVLTreeNode<TREETYPE>* pRoot, int nDepth)
{
	if(!pRoot)
	{
		for(int nCnt = 0 ; nCnt < nDepth ; nCnt++)
			cout << '\t';

		cout << endl;
	}
	else
	{
		Print(pRoot->getRightChild(), nDepth + 1);

		for(int nCnt = 0 ; nCnt < nDepth ; nCnt++)
			cout << '\t';

		CLine* pLineData = 0;
		CEventQueueData* pEventQueueData = 0;
		
		pRoot->getData(&pLineData);
		if(pLineData)
		{
			tcout << _T("(") << pLineData->getUpperPoint().nXCoordinate << _T(",") << pLineData->getUpperPoint().nYCoordinate << _T(") ") <<
							 _T("(") << pLineData->getLowerPoint().nXCoordinate << _T(",") << pLineData->getLowerPoint().nYCoordinate << _T(") ") << endl;

			SAFEDELETE(pLineData);
		}
						
		Print(pRoot->getLeftChild(), nDepth + 1);
	}
}
