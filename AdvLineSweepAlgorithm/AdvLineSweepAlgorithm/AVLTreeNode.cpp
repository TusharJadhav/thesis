#pragma once

#include "StdAfx.h"
#include "AVLTreeNode.h"
#include <typeinfo.h>

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>::~CAVLTreeNode(void)
{
  SAFEDELETE(m_data);
}

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>* CAVLTreeNode<NODETYPE>::getLeftChild()
{
	return m_pLeftChild;
}

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>* CAVLTreeNode<NODETYPE>::getRightChild()
{
	return m_pRightChild;
}

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>* CAVLTreeNode<NODETYPE>::getParentNode()
{
	return m_pParentNode;
}

template <typename NODETYPE>
COMPARISON_RESULT CAVLTreeNode<NODETYPE>::insert(NODETYPE newData)
{
	COMPARISON_RESULT enumInsertedPosition = UNDEFINED;

	CAVLTreeNode<NODETYPE>* pNewNode = 0;
  COMPARISON_RESULT enumResult = compare(newData);

  pNewNode = new CAVLTreeNode<NODETYPE>(m_pTree, newData, 1, 0, 0, 0);	

	if( enumResult == LESS_THAN || enumResult == EQUAL_TO )
	{
		// Insert the new node as the left child.
		this->m_pLeftChild = pNewNode;
		pNewNode->setParentNode(this);

		if(enumResult == LESS_THAN)
			enumInsertedPosition = LESS_THAN;
		else
			enumInsertedPosition = EQUAL_TO;
	}
	else
	{
		// Insert the new node as the right child.
		this->m_pRightChild = pNewNode;
		pNewNode->setParentNode(this);

		enumInsertedPosition = MORE_THAN;
	}

	return enumInsertedPosition;
}

template <typename NODETYPE>
bool CAVLTreeNode<NODETYPE>::IsRoot()
{
  return(this->m_pParentNode == 0);
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setBalanceFactor(int nBalanceFactor)
{
	m_nBalanceFactor = nBalanceFactor;
}

template <typename NODETYPE>
int CAVLTreeNode<NODETYPE>::getBalanceFactor()
{
	return m_nBalanceFactor;
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setParentNode(CAVLTreeNode<NODETYPE>* pNode)
{
	m_pParentNode = pNode;
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setLeftChild(CAVLTreeNode<NODETYPE>* pNode)
{
	m_pLeftChild = pNode;
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setRightChild(CAVLTreeNode<NODETYPE>* pNode)
{
	m_pRightChild = pNode;
}

template <typename NODETYPE>
bool CAVLTreeNode<NODETYPE>::isLeftSibling()
{
	CAVLTreeNode<NODETYPE>* pParentNode = 0;
	pParentNode = getParentNode();

	if(pParentNode)
		return(pParentNode->getLeftChild() == this);
		
	return false;
}

template <typename NODETYPE>
bool CAVLTreeNode<NODETYPE>::isRightSibling()
{
	CAVLTreeNode<NODETYPE>* pParentNode = 0;
	pParentNode = getParentNode();

	if(pParentNode)
		return(pParentNode->getRightChild() == this);
			
	return false;
}

template <typename NODETYPE>
bool CAVLTreeNode<NODETYPE>::isLeaf()
{
	return(!(this->getLeftChild()) && !(this->getRightChild()));
}

template <typename NODETYPE>
bool CAVLTreeNode<NODETYPE>::hasSibling()
{
  if(IsRoot())
    return false;

  CAVLTreeNode<NODETYPE>* pParentNode = 0;
  pParentNode = getParentNode();
  if(!pParentNode)
    return false;

	if(isRightSibling())
	{
    if(pParentNode->getLeftChild() != 0)
			return true;
	}
	else
	{
    if(pParentNode->getRightChild() != 0)
			return true;
	}

	return false;
}

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>* CAVLTreeNode<NODETYPE>::getAdjacentLeftNode()
{
  return m_pAdjLeftNode;
}

template <typename NODETYPE>
CAVLTreeNode<NODETYPE>* CAVLTreeNode<NODETYPE>::getAdjacentRightNode()
{
  return m_pAdjRightNode;
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setAdjacentLeftNode(CAVLTreeNode<NODETYPE>* pNewAdjLeftNode)
{
  if(m_pAdjLeftNode == pNewAdjLeftNode)
    return;

  if(this->getParentNode() == 0)
    return;

  m_pAdjLeftNode = pNewAdjLeftNode;

  if(pNewAdjLeftNode)
    pNewAdjLeftNode->setAdjacentRightNode(this);
}

template <typename NODETYPE>
void CAVLTreeNode<NODETYPE>::setAdjacentRightNode(CAVLTreeNode<NODETYPE>* pNewAdjRightNode)
{
  if(m_pAdjRightNode == pNewAdjRightNode)
    return;

  if(this->getParentNode() == 0)
    return;

  m_pAdjRightNode = pNewAdjRightNode;

  if(pNewAdjRightNode)
    pNewAdjRightNode->setAdjacentLeftNode(this);
}

