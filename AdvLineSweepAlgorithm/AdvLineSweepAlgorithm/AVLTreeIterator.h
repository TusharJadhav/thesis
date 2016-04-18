#pragma once

template<typename TREETYPE> class CAVLTree;
template<typename NODETYPE> class CAVLTreeNode;

template <typename ITERATORTYPE>
class CAVLTreeIterator
{
	private:
		CAVLTree<ITERATORTYPE>* m_pAVLTree;
		CAVLTreeNode<ITERATORTYPE>* m_pCurrentNode;

	public:
    // Constructor
		CAVLTreeIterator(CAVLTree<ITERATORTYPE>* pAVLTree);

    // Destructor
		virtual ~CAVLTreeIterator(void);

		CAVLTreeNode<ITERATORTYPE>* getCurrentNode();
    CAVLTreeNode<ITERATORTYPE>* getLeftMostParent();
		CAVLTreeNode<ITERATORTYPE>* Find(ITERATORTYPE data);

		void moveRight();
		void moveLeft();
		void moveUp();
		void setCurrentNode(CAVLTreeNode<ITERATORTYPE>* pNode);
		void moveToAdjacentRightLeaf();
		void moveToAdjacentLeftLeaf();
		void moveToLeftMostLeaf();
		void moveToRightMostLeaf();
		void movetoLeftMostLeafOfTree();
		bool IsRightMostLeafOfTree();
		bool IsLeftMostLeafOfTree();
};
