#include "student.h"
#include <iostream>
#include <cctype>
#include <stdlib.h>

using namespace std;
class bstree
{
public:
	student *root;
	bool insert(unsigned int, short int, short int, student* &);
	bool insertCredits(unsigned int, short int, short int, student* &);
	void del(unsigned int, student* &);
	unsigned int deletemin(student* &);
	student* find(unsigned int, student* &);
	void makeempty(student* &);
	void copy(student* &, student* &);
	student* nodecopy(student* &);
	int bsheight(student*);
	student* srl(student* &);
	student* drl(student* &);
	student* srr(student* &);
	student* drr(student* &);
	int max(int, int);
	void inorder(student*);
	bstree();
};
bstree::bstree() {
	root = NULL;
}
// Inserting a student
bool bstree::insert(unsigned int x, short int ac, short int cc, student* &p){
	if (p == NULL){
		p = new student;
		p->card_id = x;
		p->app_credit = ac;
		p->card_credit = cc;
		p->left = NULL;
		p->right = NULL;
		p->height = 0;
		if (p == NULL)
		{
			return false; //return false if insertion fails for lack of space
			//cout << "Out of Space\n" << endl;
		}
		return true; //return true if insertion succeed
	}
	else
	{
		if (x<p->card_id)
		{
			insert(x,ac,cc,p->left);
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
			insert(x, ac, cc, p->right);
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
			return false; //return false if insertion fails for element already exists
			//cout << "Element Exists\n" << endl;
		}
	}
	short int m, n, d;
	m = bsheight(p->left);
	n = bsheight(p->right);
	d = max(m, n);
	p->height = d + 1;
	return true; //inserção funcionou
}
// Inserting credits in a student
bool bstree::insertCredits(unsigned int x, short int ac, short int cc, student* &p) {
	if (p == NULL) {
		return false; //return false if credit insertion fais for "id not found"
		//cout << "Id not found.\n";
	}
	else {
		find(x, p)->app_credit += ac;
		find(x, p)->card_credit += cc;
		return true; //return true if credit insertion succeed
	}

}
// Finding an card_id
student* bstree::find(unsigned int x, student* &p){
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
void bstree::copy(student* &p, student* &p1)
{
	makeempty(p1);
	p1 = nodecopy(p);
}
// Make a tree empty
void bstree::makeempty(student* &p)
{
	student* d;
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
student* bstree::nodecopy(student* &p)
{
	student* temp;
	if (p == NULL)
	{
		return p;
	}
	else
	{
		temp = new student;
		temp->card_id = p->card_id;
		temp->left = nodecopy(p->left);
		temp->right = nodecopy(p->right);
		return temp;
	}
}

// Deleting a student
void bstree::del(unsigned int x, student* &p){
	student* d;
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

unsigned int bstree::deletemin(student* &p)
{
	unsigned int c;
	//cout << "inside deltemin\n" << endl;
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
int bstree::bsheight(student* p)
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

student* bstree::srl(student* &p1){
	student* p2;
	p2 = p1->left;
	p1->left = p2->right;
	p2->right = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(bsheight(p2->left), p1->height) + 1;
	return p2;
}
student* bstree::srr(student* &p1){
	student* p2;
	p2 = p1->right;
	p1->right = p2->left;
	p2->left = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(p1->height, bsheight(p2->right)) + 1;
	return p2;
}
student* bstree::drl(student* &p1){
	p1->left = srr(p1->left);
	return srl(p1);
}
student* bstree::drr(student* &p1){
	p1->right = srl(p1->right);
	return srr(p1);
}

// Inorder Printing
void bstree::inorder(student* p)
{
	if (p != NULL)
	{
		inorder(p->left);
		cout << p->card_id << "\t";
		inorder(p->right);
	}
}

