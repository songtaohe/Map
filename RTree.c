#include <stdio.h>
#include <malloc.h>

#define _M_ 4









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
	//void* key;
	int key;
};




struct Link
{
	struct Link* next;
	int key;
};


struct Node* TestInsert(struct Node* root, double minx,double miny, double maxx, double maxy, int key);
double LeastEnlargement(struct Rect target, struct Rect cur);
int Overlay(struct Rect r1, struct Rect r2)
{
	int ox = 1;
	int oy = 1;

	if(r1.MinX > r2.MaxX) ox = 0;
	if(r2.MinX > r1.MaxX) ox = 0;

	if(r1.MinY > r2.MaxY) oy = 0;
	if(r2.MinY > r1.MaxY) oy = 0;

	return ox*oy;
}


struct Node * PythonRoot = NULL;
struct Link * PythonLinkHead = NULL;
struct Link * PythonLinkCur = NULL;
int PythonInit = 0;

__attribute__((__visibility__("default"))) void PythonInsert(double minx,double miny,double maxx, double maxy, int key)
{
	if(PythonInit == 0) // Initialization
	{
		PythonRoot = (struct Node*)malloc(sizeof(struct Node));

	    PythonRoot->R.MinX = -1;
    	PythonRoot->R.MinY = -1;
    	PythonRoot->R.MaxX = 1;
    	PythonRoot->R.MaxY = 1;

    	PythonRoot->F = NULL;
    	for(int i = 0;i < _M_; i++) PythonRoot->C[i] = NULL;


    	PythonRoot->NumC = 0;
    	PythonRoot->key = -1;
    	PythonRoot->IsLeaf = 1;

		PythonLinkHead = (struct Link*)malloc(sizeof(struct Link));		
		PythonLinkHead->next = NULL;
		PythonLinkHead->key = -1;

		PythonLinkCur = PythonLinkHead;


		PythonInit = 1;
	}	

	PythonRoot = TestInsert(PythonRoot,minx,miny,maxx,maxy,key);
}

int PythonQueryRecursive(struct Node* node, double minx, double miny, double maxx, double maxy)
{
	struct Rect r;
	int ret = 0;
	r.MinX = minx;
	r.MinY = miny;
	r.MaxX = maxx;
	r.MaxY = maxy;

	if(node == NULL) return 0;
	if(node->IsLeaf) 
	{
		if(LeastEnlargement(node->R,r) == 0)
		{
			ret = 1;
			PythonLinkCur->key = node->key;
			if(PythonLinkCur->next == NULL)
			{
				PythonLinkCur->next = (struct Link*)malloc(sizeof(struct Link));
				PythonLinkCur = PythonLinkCur->next;
			}
			else
			{
				PythonLinkCur = PythonLinkCur->next;
			}
		}					
	}
	else
	{
		for(int i = 0; i< node->NumC; i++)
		{
			if(Overlay(node->C[i]->R,r))
			{
				ret += PythonQueryRecursive(node->C[i], minx, miny, maxx, maxy);
			} 

		}		
	}

	return ret;
}


__attribute__((__visibility__("default"))) int PythonQuery(double minx,double miny, double maxx, double maxy)
{
	int ret = 0;
	PythonLinkCur = PythonLinkHead;
	//TODO
	ret = PythonQueryRecursive(PythonRoot,minx,miny,maxx,maxy);		

	PythonLinkCur = PythonLinkHead;

	return ret;
}


__attribute__((__visibility__("default"))) int PythonReadKey()
{
	int ret = -1;
	if(PythonLinkCur != NULL) 
	{
		ret = PythonLinkCur->key;
		PythonLinkCur = PythonLinkCur->next;
	}

	return ret;
}






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

void LocalUpdateRect(struct Node* cur)
{
	struct Rect nR;

	nR = cur->C[0]->R;

	for(int i = 1;i < cur->NumC;i++)
	{
		if(nR.MinX > cur->C[i]->R.MinX) nR.MinX = cur->C[i]->R.MinX;	
		if(nR.MinY > cur->C[i]->R.MinY) nR.MinY = cur->C[i]->R.MinY;	
		if(nR.MaxX < cur->C[i]->R.MaxX) nR.MaxX = cur->C[i]->R.MaxX;	
		if(nR.MaxY < cur->C[i]->R.MaxY) nR.MaxY = cur->C[i]->R.MaxY;	
	}

	cur->R = nR;
	
}

struct Node* Split(struct Node* cur, struct Node* new)
{
	//Re-designate Children
	// TODO Optimization
	for(int i = _M_/2; i< _M_; i++)
	{
		new->C[i-_M_/2+1] = cur->C[i];
		cur->C[i]->F = new;
	}

	new->NumC = _M_/2+1;
	cur->NumC = _M_/2;
	
	LocalUpdateRect(new);
	LocalUpdateRect(cur);
	
	
	if(cur->F == NULL)
	{	// A new root
		struct Node* newRoot;
		newRoot = (struct Node*)malloc(sizeof(struct Node));
		cur->F = newRoot;
		new->F = newRoot;
		newRoot->NumC = 2;
		newRoot->C[0] = cur;
		newRoot->C[1] = new;
		newRoot->IsLeaf = 0;

		LocalUpdateRect(newRoot);		

		return newRoot;
	}

	if(cur->F->NumC < _M_)
	{
		cur->F->C[cur->F->NumC] = new;
		new->F = cur->F;
		cur->F->NumC ++;
		
		UpdateRect(cur->F, new->R);

		return NULL;
	}
	else
	{
		struct Node* _new = (struct Node*)malloc(sizeof(struct Node));
        _new->R = new->R;
        _new->NumC = 1;
        _new->C[0] = new;
        _new->C[0]->F = _new;
        _new->IsLeaf = 0;
		
		return Split(cur->F, _new);
	}

}


struct Node * Insert(struct Node* root,struct Rect newRect, int newKey)
{
	struct Node* L = ChooseLeaf(root, newRect);

	if(L->NumC < _M_)
	{
		L->C[L->NumC] = (struct Node*)malloc(sizeof(struct Node));
		L->C[L->NumC]->R = newRect;
		L->C[L->NumC]->key = newKey;
		L->NumC ++;

		UpdateRect(L, newRect);
		return root;
	}
	else
	{
		struct Node* newRoot = NULL;
		struct Node* new = (struct Node*)malloc(sizeof(struct Node));
		new->R = newRect;
		new->NumC = 1;
		new->C[0] = (struct Node*)malloc(sizeof(struct Node));
		new->C[0]->R = newRect;
		new->C[0]->key = newKey;
		new->C[0]->F = new;
		new->IsLeaf = 1;
				
		newRoot = Split(L, new);

		if(newRoot != NULL) return newRoot;
		else return root;
	}

}

void Dump(struct Node* cur)
{
	if(cur==NULL) return;
	
	printf("\nNode %p   [%.2lf, %.2lf, %.2lf, %.2lf]\n", cur,
						cur->R.MinX, cur->R.MinY, cur->R.MaxX, cur->R.MaxY);
	for(int i = 0;i < cur->NumC; i++)
	{
		struct Rect R = cur->C[i]->R;
		printf("    -  %p [%.2lf, %.2lf, %.2lf, %.2lf]\n", cur->C[i],
						R.MinX, R.MinY, R.MaxX, R.MaxY);
	}

	for(int i = 0;i<cur->NumC;i++)
	{
		Dump(cur->C[i]);
	}	
}

struct Node* TestInsert(struct Node* root, double minx,double miny, double maxx, double maxy, int key)
{
	struct Rect R;
	R.MinX = minx;
	R.MinY = miny;
	R.MaxX = maxx;
	R.MaxY = maxy;

	return Insert(root,R,key);

}


void main()
{
	struct Node * root;
	root = (struct Node*)malloc(sizeof(struct Node));

	root->R.MinX = -1;
	root->R.MinY = -1;
	root->R.MaxX = 1;
	root->R.MaxY = 1;

	root->F = NULL;
	for(int i = 0;i < _M_; i++) root->C[i] = NULL;


	root->NumC = 0;
	root->key = -1;	
	root->IsLeaf = 1;




	
	root = TestInsert(root,-10,-10,10,10,1);
	root = TestInsert(root, -20,-5,20,5,2);

	root = TestInsert(root,10,10,20,20,1);
	root = TestInsert(root, -30,-5,-20,5,2);

	root = TestInsert(root,1,1,2,2,1);
	root = TestInsert(root, -2,-2,-1,-1,2);

	Dump(root);


}



