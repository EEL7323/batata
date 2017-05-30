#include <iostream>
#include<cctype>
#include <stdlib.h>
#include <conio.h>

using namespace std;

struct node
{
	unsigned int card_id;
	node *left;
	node *right;
	unsigned int height;
	short int card_credit;
	short int app_credit;
};
typedef struct node *nodeptr;
class bstree
{
public:
	void insert(unsigned int, nodeptr &);
	void bstree::insertCredits(unsigned int, short int, short int, nodeptr &);
	void del(unsigned int, nodeptr &);
	unsigned int deletemin(nodeptr &);
	nodeptr find(unsigned int, nodeptr &);
	void makeempty(nodeptr &);
	void copy(nodeptr &, nodeptr &);
	nodeptr nodecopy(nodeptr &);
	int bsheight(nodeptr);
	nodeptr srl(nodeptr &);
	nodeptr drl(nodeptr &);
	nodeptr srr(nodeptr &);
	nodeptr drr(nodeptr &);
	int max(int, int);
	int nonodes(nodeptr);
};
// Inserting a node
void bstree::insert(unsigned int x, nodeptr &p)
{
	if (p == NULL)
	{
		p = new node;
		p->card_id = x;
		p->app_credit = NULL;
		p->card_credit = NULL;
		p->left = NULL;
		p->right = NULL;
		p->height = 0;
		if (p == NULL)
		{
			cout << "Out of Space\n" << endl;
		}
	}
	else
	{
		if (x<p->card_id)
		{
			insert(x, p->left);
			if ((bsheight(p->left) - bsheight(p->right)) == 2)
			{
				if (x < p->left->card_id)
				{
					p = srl(p);
				}
				else
				{
					p = drl(p);
				}
			}
		}
		else if (x>p->card_id)
		{
			insert(x, p->right);
			if ((bsheight(p->right) - bsheight(p->left)) == 2)
			{
				if (x > p->right->card_id)
				{
					p = srr(p);
				}
				else
				{
					p = drr(p);
				}
			}
		}
		else
		{
			cout << "Element Exists\n" << endl;
		}
	}
	int m, n, d;
	m = bsheight(p->left);
	n = bsheight(p->right);
	d = max(m, n);
	p->height = d + 1;
}
// Inserting credits in a node
void bstree::insertCredits(unsigned int x,short int ac,short int cc, nodeptr &p) {
	if (p == NULL) {
		cout << "Id não existe.\n";
	}
	else {
		//nodeptr *z = find(x, p);
		find(x, p)->app_credit += ac;
		find(x, p)->card_credit += cc;
		//z->app_credit += ac;
		//z->card_credit += cc;
	}

}
// Finding an card_id
nodeptr bstree::find(unsigned int x, nodeptr &p)
{
	if (p == NULL)
	{
//		cout << "Sorry! card_id not found\n" << endl;
		return NULL;
	}
	else
	{
		if (x < p->card_id)
		{
			return find(x, p->left);
		}
		else
		{
			if (x>p->card_id)
			{
				return find(x, p->right);
			}
			else
			{
				return p;
			}
		}
	}
}
// Copy a tree
void bstree::copy(nodeptr &p, nodeptr &p1)
{
	makeempty(p1);
	p1 = nodecopy(p);
}
// Make a tree empty
void bstree::makeempty(nodeptr &p)
{
	nodeptr d;
	if (p != NULL)
	{
		makeempty(p->left);
		makeempty(p->right);
		d = p;
		free(d);
		p = NULL;
	}
}
// Copy the nodes
nodeptr bstree::nodecopy(nodeptr &p)
{
	nodeptr temp;
	if (p == NULL)
	{
		return p;
	}
	else
	{
		temp = new node;
		temp->card_id = p->card_id;
		temp->left = nodecopy(p->left);
		temp->right = nodecopy(p->right);
		return temp;
	}
}

// Deleting a node
void bstree::del(unsigned int x, nodeptr &p)
{
	nodeptr d;
	if (p == NULL)
	{
		cout << "Sorry! card_id not found\n" << endl;
	}
	else if (x < p->card_id)
	{
		del(x, p->left);
	}
	else if (x > p->card_id)
	{
		del(x, p->right);
	}
	else if ((p->left == NULL) && (p->right == NULL))
	{
		d = p;
		free(d);
		p = NULL;
		cout << "Element deleted successfully\n" << endl;
	}
	else if (p->left == NULL)
	{
		d = p;
		free(d);
		p = p->right;
		cout << "Element deleted successfully\n" << endl;
	}
	else if (p->right == NULL)
	{
		d = p;
		p = p->left;
		free(d);
		cout << "Element deleted successfully\n" << endl;
	}
	else
	{
		p->card_id = deletemin(p->right);
	}
}

unsigned int bstree::deletemin(nodeptr &p)
{
	unsigned int c;
	cout << "inside deltemin\n" << endl;
	if (p->left == NULL)
	{
		c = p->card_id;
		p = p->right;
		return c;
	}
	else
	{
		c = deletemin(p->left);
		return c;
	}
}

int bstree::max(int value1, int value2)
{
	return ((value1 > value2) ? value1 : value2);
}
int bstree::bsheight(nodeptr p)
{
	int t;
	if (p == NULL)
	{
		return -1;
	}
	else
	{
		t = p->height;
		return t;
	}
}

nodeptr bstree::srl(nodeptr &p1)
{
	nodeptr p2;
	p2 = p1->left;
	p1->left = p2->right;
	p2->right = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(bsheight(p2->left), p1->height) + 1;
	return p2;
}
nodeptr bstree::srr(nodeptr &p1)
{
	nodeptr p2;
	p2 = p1->right;
	p1->right = p2->left;
	p2->left = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(p1->height, bsheight(p2->right)) + 1;
	return p2;
}
nodeptr bstree::drl(nodeptr &p1)
{
	p1->left = srr(p1->left);
	return srl(p1);
}
nodeptr bstree::drr(nodeptr &p1)
{
	p1->right = srl(p1->right);
	return srr(p1);
}

int bstree::nonodes(nodeptr p)
{
	int count = 0;
	if (p != NULL)
	{
		nonodes(p->left);
		nonodes(p->right);
		count++;
	}
	return count;
}