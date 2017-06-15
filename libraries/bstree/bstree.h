
#ifndef _BSTREE_h
#define _BSTREE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


// #include <iostream>
//#include <cctype>
//#include <stdlib.h>



class student {
public:
	unsigned int card_id;
	student* left;
	student* right;
	unsigned short int height;
	unsigned short int card_credit;
	unsigned short int app_credit;
	student();
};

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
	int maximum(int, int);
	void inorder(student*);
	bstree();
};

#endif;