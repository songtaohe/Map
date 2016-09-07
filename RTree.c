#include <stdio.h>
#include <malloc.h>

#define _M_ 8

struct Rect
{
	double MinX;
	double MinY;
	double MaxX;
	double MaxY;
};

struct Node
{
	struct Rect R;
	struct Node* F;
	struct Node* C[_M_];	
	int NumC;
	int IsLeaf;
	void* key;
};


double LeastEnlargement(struct Rect target, struct Rect cur)
{
	double f = 0;
	f += cur.MinX < target.MinX ? target.MinX - cur.MinX: 0;
	f += cur.MinY < target.MinY ? target.MinY - cur.MinY: 0;
	f += cur.MaxX > target.MaxX ? cur.MaxX - target.MaxX: 0;
	f += cur.MaxY > target.MaxY	? cur.MaxY - target.MaxY: 0; 
	return f;
}


struct Node* ChooseLeaf(struct Node* cur, struct Rect newRect)
{
	if(cur->IsLeaf == 1) return cur;
	double MinLeastEnlargement = 10000000000.0;
	int next = 0;
	for(int i = 0;i < cur->NumC; i++)
	{
		double curLE = LeastEnlargement(newRect, cur->C[i]->R);
		if(curLE < MinLeastEnlargement)
		{	
			MinLeastEnlargement = curLE;
			next = i;
		}
	}

	return ChooseLeaf(cur->C[next],newRect);
}


void UpdateRect(struct Node* cur, struct Rect newRect)
{
	if(cur == NULL) return;
	
	if(cur->R.MinX > newRect.MinX) cur->R.MinX = newRect.MinX;
	if(cur->R.MinY > newRect.MinY) cur->R.MinY = newRect.MinY;
	
	if(cur->R.MaxX < newRect.MaxX) cur->R.MaxX = newRect.MaxX;
	if(cur->R.MaxY < newRect.MaxY) cur->R.MaxY = newRect.MaxY;

	UpdateRect(cur->F,cur->R);
}


struct Node * Insert(struct Node* root,struct Rect newRect, void * newKey)
{
	struct Node* L = ChooseLeaf(root, newRect);

	if(L->NumC < _M_)
	{
		L->C[L->NumC] = (struct Node*)malloc(sizeof(struct Node));
		L->C[L->NumC]->R = newRect;
		L->C[L->NumC]->Key = newKey;
		L->NumC ++;

		UpdateRect(L, newRect);
		return root;
	}
	else
	{
		

	}


}




void main()
{
	struct Node * root;
	root = (struct Node*)malloc(sizeof(struct Node));

	root->R.MinX = -100;
	root->R.MinY = -100;
	root->R.MaxX = 100;
	root->R.MaxY = 100;

	root->F = NULL;
	for(int i = 0;i < _M_; i++) root->C[i] = NULL;

	root->NumC = 0;
	root->key = NULL;	
	root->IsLeaf = 1;


}



