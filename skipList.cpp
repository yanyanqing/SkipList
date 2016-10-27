/*************************************************************************
	> File Name: skipList.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年10月27日 星期四 12时50分10秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include <iostream> 
#include <cstdlib>
#include <time.h>
#include <cassert>
#define MAXLEVEL 5
using namespace std;

typedef struct NodeStruct
{
	int key;
	int value;
	struct NodeStruct *forward[1];
}nodeStruct;

typedef struct SkipList
{
	int level;
	nodeStruct *head;
}skipList;


nodeStruct* createNodeStruct(int level, int key, int value)
{
	nodeStruct *ns = (nodeStruct*)malloc(sizeof(nodeStruct)+level*sizeof(nodeStruct*));
	if(ns == NULL) exit(1);
	
	ns->key = key;
	ns->value = value;
	
	return ns;
}

void initSkipList(skipList *psl)
{
	psl->head = createNodeStruct(MAXLEVEL-1, 0, 0);
	psl->level = 0;
	
	for(int i = 0; i < MAXLEVEL; ++i)
	{
		psl->head->forward[i] = NULL;
	}
}

int randLevel()
{
	int k = 1;
	while(rand() % 2)
		++k;
		
	k = (k<MAXLEVEL)?k:MAXLEVEL;  
    return k;    
}


bool insert(skipList *psl, int key, int value)
{
	assert(psl != NULL);
	
	nodeStruct *update[MAXLEVEL];
	nodeStruct *p = psl->head;
	nodeStruct *q = NULL;
	int k = psl->level;
	
	for(int i = k-1; i >= 0; --i)
	{
		while((q=p->forward[i]) && (q->key < key) )p = q;
		
		update[i] = p;
	}
	
	if(q != NULL && q->key == key) return false;
	
	k = randLevel();
	//k = k > MAXLEVEL ? MAXLEVEL:k;
	if(k > psl->level)
	{
		for(int i = psl->level; i < k; ++i)
		{
			update[i] = psl->head;
		}
		psl->level = k;
	}
	
	
	q = createNodeStruct(k, key, value);
	
	for(int i = 0; i < k; ++i)
	{
		q->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = q;
	}
	
	return true;
}

void show(skipList *ptr)
{
	nodeStruct *p = ptr->head;

	for(int i = ptr->level-1; i >= 0; --i)
	{
		p = ptr->head;
		while(p != NULL)
		{
			cout << p->value << "->";
			p = p->forward[i];
		}
		cout << endl;
	}
	cout << endl;
}

bool remove(skipList *ptr, int key)
{
	if(ptr == NULL) return false;

	int k = ptr->level - 1;
	nodeStruct *p = ptr->head;
	nodeStruct *q = NULL;
	nodeStruct *qq = NULL;
	for(int i = k; i >= 0; --i)
	{
		p = ptr->head;
		while((q=p->forward[i]) && q->key != key)
			p = q;

		if(q != NULL)
		{
			if(i == k) qq = q;
			if(ptr->head->key == key)
			{
				ptr->head = q->forward[i];
			}
			else
			{
				p->forward[i] = q->forward[i];
			}
		}
	}
	if(q == NULL) return false;
	free(qq);
	return true;
}


int main()
{
	skipList sl;
	initSkipList(&sl);
	
	for(int i = 1; i < 20; ++i)
	{
		insert(&sl, i, i*2);
	}
	show(&sl);
	//show(&sl);
	int x;
	while(cin >> x, x != -1)
	{
		remove(&sl, x);
		show(&sl);
	}
	cout << endl;
	
	return 0;
}
