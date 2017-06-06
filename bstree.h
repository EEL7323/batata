#include "student.h"
#include <iostream>
#include <cctype>
#include <stdlib.h>

using namespace std;
class bstree
{
public:
	student *root;
	bool insert(unsigned int, unsigned short int, unsigned short int, student* &);
	bool insertCredits(unsigned int, unsigned short int, unsigned short int, student* &);
	bool debitCredits(unsigned int, char, student* &);
	void del(unsigned int, student* &);
	unsigned int deletemin(student* &);
	student* find(unsigned int, student* &);
	void makeempty(student* &);
	void copy(student* &, student* &);
	student* nodecopy(student* &);
	unsigned short int bsheight(student*);
	student* srl(student* &);
	student* drl(student* &);
	student* srr(student* &);
	student* drr(student* &);
	int max(int, int);
	void inorder(student*);
	bstree();
};
