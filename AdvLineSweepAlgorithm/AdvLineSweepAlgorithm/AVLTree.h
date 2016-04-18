#pragma once
#include "AVLTreeNode.h"
#include "AVLTreeIterator.h"

template <typename TREETYPE>
class CAVLTree
{
	private:
		CAVLTreeNode<TREETYPE>* m_pRoot;
		static int nTempHeight;
    SPACE_POINT m_ptCurrentEventPoint;

	public:
    // Constructor
		CAVLTree(void);

    // Destructor
		virtual ~CAVLTree(void);

    void insert(TREETYPE newData);
		void remove(TREETYPE deleteData);
    void empty();
    void SetCurrentlyHandledEventPoint(SPACE_POINT ptEventPoint);
    SPACE_POINT GetCurrentlyHandledEventPoint();
		CAVLTreeIterator<TREETYPE>* getIterator(void);
		void PrintTree();

	private:
		CAVLTreeNode<TREETYPE>* getNodeToInsert(TREETYPE newData);
		CAVLTreeNode<TREETYPE>* getNodeToRemove(TREETYPE deleteData);
    CAVLTreeNode<TREETYPE>* GetRightMostChild(CAVLTreeNode<TREETYPE>* pNode);
		void calculateMaxDept(CAVLTreeNode<TREETYPE>* pNode, int &nHeight);
		void Restructure(CAVLTreeNode<TREETYPE>* pNode, bool bAfterDeletingNode = true);
		void Rotate(CAVLTreeNode<TREETYPE>* pNode, int nBalanceFactor, bool bAfterDeletingNode);
		void PerformLeftRotation(CAVLTreeNode<TREETYPE>* pRotatingNode);
		void PerformRightRotation(CAVLTreeNode<TREETYPE>* pRotatingNode);
		void CalculateBalanceFactor(CAVLTreeNode<TREETYPE>* pNode);
		void CheckInternalNodesForInsert();
    void DeleteNodeSafely(CAVLTreeNode<TREETYPE>* pNode);
    void DeleteRecursively(CAVLTreeNode<TREETYPE>* pNode);
		int CalculateMaxHeight(CAVLTreeNode<TREETYPE>* pNode);
		void Print(CAVLTreeNode<TREETYPE>* pRoot, int nDepth);

  public:
		friend class CAVLTreeIterator<TREETYPE>;
};
