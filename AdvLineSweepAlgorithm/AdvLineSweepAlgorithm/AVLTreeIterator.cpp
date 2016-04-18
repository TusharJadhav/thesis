#include "StdAfx.h"
#include "AVLTreeIterator.h"

template <typename ITERATORTYPE>
CAVLTreeIterator<ITERATORTYPE>::CAVLTreeIterator(CAVLTree<ITERATORTYPE>* pAVLTree)
{
	m_pAVLTree = pAVLTree;
	m_pCurrentNode = pAVLTree->m_pRoot;
}

template <typename ITERATORTYPE>
CAVLTreeIterator<ITERATORTYPE>::~CAVLTreeIterator(void)
{
}

template <typename ITERATORTYPE>
CAVLTreeNode<ITERATORTYPE>* CAVLTreeIterator<ITERATORTYPE>::getCurrentNode()
{
	return m_pCurrentNode;
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveRight()
{
	if(m_pCurrentNode)
		m_pCurrentNode = m_pCurrentNode->getRightChild();
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveLeft()
{
	if(m_pCurrentNode)
		m_pCurrentNode = m_pCurrentNode->getLeftChild();
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveUp()
{
	if(m_pCurrentNode)
		m_pCurrentNode = m_pCurrentNode->getParentNode();
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::setCurrentNode(CAVLTreeNode<ITERATORTYPE>* pNode)
{
	m_pCurrentNode = pNode;
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveToAdjacentRightLeaf()
{
  if(!m_pCurrentNode)
		return;

	BOOL						ret(TRUE);
	bool            bHasFreq;
	LARGE_INTEGER   freq;
	LARGE_INTEGER		start; 
	LARGE_INTEGER		stop;

	if(g_bProfileExecutionTime)
	{
		::SetQuadPart(&freq, 0);
		ret = ::QueryPerformanceFrequency(&freq);
		ASSERT(ret);
		bHasFreq = (TRUE == ret ? true : false);

		ret = TRUE;

		if(bHasFreq)
		{
			::SetQuadPart(&start, 0);
			::SetQuadPart(&stop, 0);
			ret = ::QueryPerformanceCounter(&start);
			ASSERT(ret);
		}
	}

	if(g_bOptimizedMode)
	{
		m_pCurrentNode = m_pCurrentNode->getAdjacentRightNode();
	}
	else
	{
		// If the current node is not a leaf node then traverse to the left most leaf from the current node.
		if(!m_pCurrentNode->isLeaf())
			moveToLeftMostLeaf();
		else
		{
			if(IsRightMostLeafOfTree())
			{
				m_pCurrentNode = 0;
				return;
			}

			while(1)
			{
				if(m_pCurrentNode)
				{
					CAVLTreeNode<ITERATORTYPE>* pParent = 0;
					pParent = m_pCurrentNode->getParentNode();

					if(!pParent)
						break;

					if(pParent->getRightChild() && (pParent->getRightChild() != m_pCurrentNode))
					{
						m_pCurrentNode = pParent;
						break;
					}
				
					m_pCurrentNode = pParent;
				}
			}

			if(m_pCurrentNode)
			{
				m_pCurrentNode = m_pCurrentNode->getRightChild();
			
				while(m_pCurrentNode && m_pCurrentNode->isLeaf() == false)
				{
					if(m_pCurrentNode->getLeftChild())
						m_pCurrentNode = m_pCurrentNode->getLeftChild();
					else
						m_pCurrentNode = m_pCurrentNode->getRightChild();
				}
			}
			else
				m_pCurrentNode = 0;
		}
	}

	if(g_bProfileExecutionTime)
	{
		ret = TRUE;
		if(bHasFreq)
		{
			ret = ::QueryPerformanceCounter(&stop);
			ASSERT(ret);
			double elapsed(0.0);
			elapsed = static_cast<double>(stop.QuadPart - start.QuadPart) / static_cast<double>(freq.QuadPart);

			g_AggRightNeighborSearchTime += elapsed;
		}
	}
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveToAdjacentLeftLeaf()
{
	if(!m_pCurrentNode)
		return;

	BOOL						ret(TRUE);
	bool            bHasFreq;
	LARGE_INTEGER   freq;
	LARGE_INTEGER		start; 
	LARGE_INTEGER		stop;

	if(g_bProfileExecutionTime)
	{
		::SetQuadPart(&freq, 0);
		ret = ::QueryPerformanceFrequency(&freq);
		ASSERT(ret);
		bHasFreq = (TRUE == ret ? true : false);

		ret = TRUE;

		if(bHasFreq)
		{
			::SetQuadPart(&start, 0);
			::SetQuadPart(&stop, 0);
			ret = ::QueryPerformanceCounter(&start);
			ASSERT(ret);
		}
	}

	if(g_bOptimizedMode)
	{
		m_pCurrentNode = m_pCurrentNode->getAdjacentLeftNode();
	}
	else
	{
		// If the current node is not a leaf node then traverse to the right most leaf from the current node.
		if(m_pCurrentNode->isLeaf() == false)
		{
			moveToRightMostLeaf();
		}
		else
		{
			if(IsLeftMostLeafOfTree() == true)
			{
				m_pCurrentNode = 0;
				return;
			}

			while(1)
			{
				if(m_pCurrentNode)
				{
					CAVLTreeNode<ITERATORTYPE>* pParent = 0;
					pParent = m_pCurrentNode->getParentNode();

					if(!pParent)
						break;

					if(pParent->getLeftChild() && (pParent->getLeftChild() != m_pCurrentNode))
					{
						m_pCurrentNode = pParent;
						break;
					}
				
					m_pCurrentNode = pParent;
				}
			}

			if(m_pCurrentNode)
			{
				m_pCurrentNode = m_pCurrentNode->getLeftChild();
			
				while(m_pCurrentNode && m_pCurrentNode->isLeaf() == false)
				{
					if(m_pCurrentNode->getRightChild())
						m_pCurrentNode = m_pCurrentNode->getRightChild();
					else
						m_pCurrentNode = m_pCurrentNode->getLeftChild();
				}
			}
			else
				m_pCurrentNode = 0;
		}
	}

	if(g_bProfileExecutionTime)
	{
		ret = TRUE;
		if(bHasFreq)
		{
			ret = ::QueryPerformanceCounter(&stop);
			ASSERT(ret);
			double elapsed(0.0);
			elapsed = static_cast<double>(stop.QuadPart - start.QuadPart) / static_cast<double>(freq.QuadPart);

			g_AggLeftNeighborSearchTime += elapsed;
		}
	}
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveToLeftMostLeaf()
{
	if(m_pCurrentNode == NULL)
		return;

	while(m_pCurrentNode->isLeaf() == false)
	{
		if(m_pCurrentNode->getLeftChild() != NULL)
			m_pCurrentNode = m_pCurrentNode->getLeftChild();
		else
		{
			if(m_pCurrentNode->getRightChild() != NULL)
				m_pCurrentNode = m_pCurrentNode->getRightChild();
			else
				break;
		}
	}
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::moveToRightMostLeaf()
{
	if(m_pCurrentNode == NULL)
		return;

	while(m_pCurrentNode->isLeaf() == false)
	{
		if(m_pCurrentNode->getRightChild() != NULL)
			m_pCurrentNode = m_pCurrentNode->getRightChild();
		else
		{
			if(m_pCurrentNode->getLeftChild() != NULL)
				m_pCurrentNode = m_pCurrentNode->getLeftChild();
			else
				break;
		}
	}
}

template <typename ITERATORTYPE>
bool CAVLTreeIterator<ITERATORTYPE>::IsRightMostLeafOfTree()
{
	if(m_pCurrentNode == NULL)
		return false;

	if(m_pCurrentNode->isLeaf() == false)
		return false;

	CAVLTreeNode<ITERATORTYPE>* pNodeToCheck = 0;
  pNodeToCheck = m_pAVLTree->m_pRoot;

	while(1)
	{
    if(!pNodeToCheck)
      return false;

		if(pNodeToCheck->getRightChild() == NULL)
		{
			if(pNodeToCheck->getLeftChild() != NULL)
				pNodeToCheck = pNodeToCheck->getLeftChild();
			else
				break;
		}
		else
		{
			pNodeToCheck = pNodeToCheck->getRightChild();
		}
	}

	if(m_pCurrentNode == pNodeToCheck)
		return true;
	else
		return false;
}

template <typename ITERATORTYPE>
bool CAVLTreeIterator<ITERATORTYPE>::IsLeftMostLeafOfTree()
{
	if(m_pCurrentNode == NULL)
		return false;

	if(m_pCurrentNode->isLeaf() == false)
		return false;

	CAVLTreeNode<ITERATORTYPE>* pNodeToCheck = m_pAVLTree->m_pRoot;

	while(1)
	{
		if(pNodeToCheck->getLeftChild() == NULL)
		{
			if(pNodeToCheck->getRightChild() != NULL)
				pNodeToCheck = pNodeToCheck->getRightChild();
			else
				break;
		}
		else
		{
			pNodeToCheck = pNodeToCheck->getLeftChild();
		}
	}

	if(m_pCurrentNode == pNodeToCheck)
		return true;
	else
		return false;
}

template <typename ITERATORTYPE>
CAVLTreeNode<ITERATORTYPE>* CAVLTreeIterator<ITERATORTYPE>::getLeftMostParent()
{
	CAVLTreeNode<ITERATORTYPE>* pLeftMostParent = NULL;

	if(m_pCurrentNode)
	{
		if(m_pCurrentNode->isRightSibling())
		{
			pLeftMostParent = m_pCurrentNode;

			while(pLeftMostParent && pLeftMostParent->isRightSibling())
			{
				pLeftMostParent = pLeftMostParent->getParentNode();
			}
		}
    else
    if(m_pCurrentNode->isLeftSibling())
      pLeftMostParent = m_pCurrentNode;
	}

	return pLeftMostParent;
}

template <typename ITERATORTYPE>
void CAVLTreeIterator<ITERATORTYPE>::movetoLeftMostLeafOfTree()
{
	if(m_pCurrentNode != NULL && m_pAVLTree != NULL)
	{
		m_pCurrentNode = m_pAVLTree->m_pRoot;
		bool bFlag(true);
		
		while(1)
		{
			while(m_pCurrentNode->getLeftChild() != NULL)
				m_pCurrentNode = m_pCurrentNode->getLeftChild();

			if(!m_pCurrentNode->isLeaf())
			{
				if(m_pCurrentNode->getRightChild() != NULL)
					m_pCurrentNode = m_pCurrentNode->getRightChild();
				else
					return;
			}
			else
				return;
		}
	}
}

template<typename ITERATORTYPE>
CAVLTreeNode<ITERATORTYPE>* CAVLTreeIterator<ITERATORTYPE>::Find(ITERATORTYPE data)
{
	if(m_pAVLTree == NULL)
		return NULL;

	CAVLTreeNode<ITERATORTYPE>* pSearchNode = NULL;
	pSearchNode = m_pAVLTree->m_pRoot;

	if(pSearchNode == NULL)
		return NULL;
	
	COMPARISON_RESULT enumCompare = UNDEFINED;
	enumCompare = pSearchNode->compare(data);

	while(1)
	{
		if(enumCompare == LESS_THAN)
		{
			if(pSearchNode->getLeftChild() != NULL)
				pSearchNode = pSearchNode->getLeftChild();
			else
			{
				pSearchNode = NULL;
				break;
			}
		}
		else
		if(enumCompare == MORE_THAN)
		{
			if(pSearchNode->getRightChild() != NULL)
				pSearchNode = pSearchNode->getRightChild();
			else
			{
				pSearchNode = NULL;
				break;
			}
		}
		else
		if(enumCompare == EQUAL_TO)
		{
			if(pSearchNode->isLeaf() == false)
				pSearchNode = pSearchNode->getLeftChild();
			else
				break;
		}

		enumCompare = pSearchNode->compare(data);
	}

	return pSearchNode;
}
