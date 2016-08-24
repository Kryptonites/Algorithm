// Treematch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

void main()
{
	struct TreeNode {
		int val;
		struct TreeNode *left;
		struct TreeNode *right;
		TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
		}
	};
	class Solution {
	public:
		bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
		{
			bool result = false;
			if (pRoot1 != NULL&&pRoot2 != NULL){
				result = Tree1hasTree2(pRoot1, pRoot2);
				if (!result){ result = HasSubtree(pRoot1->left, pRoot2); }
				if (!result){ result = HasSubtree(pRoot1->right, pRoot2); }
			}
			return result;
		}
		bool Tree1hasTree2(TreeNode* pRootA, TreeNode* pRootB)
		{
			if (pRootA == NULL) return false;
			if (pRootB == NULL) return true;
			if (pRootB->val == pRootA->val){
				return Tree1hasTree2(pRootA->left, pRootB->left) && Tree1hasTree2(pRootA->right, pRootB->right);
			}
			else return false;
		}
	};
}