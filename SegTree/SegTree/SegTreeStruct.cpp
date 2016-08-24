#include <iostream>
#include <algorithm>

using namespace std;

const int maxind = 256;
int segTree[maxind * 4 + 10];
int array[maxind];

void build(int node, int begin, int end)
{
	if (begin == end)
		segTree[node] = array[begin]; 
	else
	{
		build(2 * node, begin, (begin + end) / 2);
		build(2 * node + 1, (begin + end) / 2 + 1, end);

		//回溯父节点值，可以是max、min或者根据题目要求设置条件
		if (segTree[2 * node] <= segTree[2 * node + 1])
			segTree[node] = segTree[2 * node];
		else
			segTree[node] = segTree[2 * node + 1];
	}
}

int query(int node, int begin, int end, int left, int right)
{
	if (left > end || right < begin)
	{
		return -1;
	}

	if (begin < left && end > right)
	{
		return segTree[node];
	}

	int lpart, rpart;
	lpart = query(2 * node, begin, (begin + end) / 2, left, right);
	rpart = query(2 * node + 1, (begin + end) / 2 + 1, end, left, right);

	if (lpart == -1)
	{
		return rpart;
	}

	if (rpart == -1)
	{
		return lpart;
	}

	if (lpart <= rpart)
	{
		return lpart;
	}
	//根据筛选条件设置返回值
	return rpart;
}

void Update(int node, int left, int right, int idx, int value)
{
	if (left == right)
	{
		segTree[idx] = value;
	}

	int mid = (left + right) / 2;
	if (idx <= mid)
	{
		Update(node * 2, left, mid, idx, value);
	}
	else
	{
		Update(node * 2 + 1, mid + 1, right, idx, value);
	}

	//回溯更新父节点，根据条件可以为max、min或者定制条件
	segTree[node] = min(segTree[node * 2], segTree[node * 2 + 1]);
}

