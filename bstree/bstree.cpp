#include "bstree.h"
//Tree constructor
bstree::bstree() {
	root = NULL;
}
// Inserting a student
bool bstree::insert(unsigned int x, unsigned short int ac, unsigned short int cc, student* &p) {
	/*Method Description:
	This method checks if there is any student in tree and look for the best place to insert the new student, keeping
	the tree balanced with srr, srl,... methods.
	It recieves an integer card ID in x, to insert in tree, and credit values (ac = app credits, cc = card credits)
	You must pass tree root pointer as parameter.
	You can get a bool return form this function. True if insertion was successful or false if it was not.
	Example:
	code: bool test = insert(123456789, 10, 10, bst.root);
	This example insert a new student with card_id=123456789, with 10 app credits and 10 card credits, in a tree object called bst.
	Root pointer from this object is bst.root.
	*/
	bool out = false;
	if (p == NULL) {
		p = new student;
		p->card_id = x;
		p->app_credit = ac;
		p->card_credit = cc;
		p->left = NULL;
		p->right = NULL;
		p->height = 0;
		if (p == NULL || (find(x, p)->app_credit + ac) > 256 || (find(x, p)->card_credit + cc) > 256)
		{
			out = false;; //return false if insertion fails for lack of space or overflow in credit
						  //cout << "Out of Space\n" << endl;
		}
		out = true;; //return true if insertion succeed
	}
	else
	{
		if (x<p->card_id)
		{
			insert(x, ac, cc, p->left);
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
			out = false; //return false if insertion fails for element already exists
						  //cout << "Element Exists\n" << endl;
		}
	}
	if (out){
		unsigned short int m, n, d;
		m = bsheight(p->left);
		n = bsheight(p->right);
		d = max(m, n);
		p->height = d + 1;
	}
	return out;
}
// Inserting credits in a student
bool bstree::insertCredits(unsigned int x, unsigned short int ac, unsigned short int cc, student* &p) {
	/*Method Description:
	This method checks if the student if card Id x exists. If true, find student and insert credit values ac and cc.
	How to use:
	The method recieves a card ID in x, and integer values in ac and cc. 
	You must also pass tree root pointer.
	You can get a bool return form this function. True if insertion was successful or false if it was not.
	Example:
	code: bool test = insertCredits(123456789,10,10,bst.root);
	This example insert a new student with card_id=123456789, with 10 app credits and 10 card credits, in a tree object called bst.
	Root pointer from this object is bst.root.*/
	bool out = false;
	if (p == NULL || (find(x, p)->app_credit + ac) > 256 || (find(x, p)->card_credit + cc) > 256) {
		out = false;; //return false if credit insertion fais for "id not found" or total credits > 256.
					  //cout << "Id not found.\n";
	}
	else {
		find(x, p)->app_credit += ac;
		find(x, p)->card_credit += cc;
		out = true;; //return true if credit insertion succeed
	}
	return out;
}
//Debit credits
bool bstree::debitCredits(unsigned int x, char t, student* &p) {
	/*Method Description:
	This method checks if there is enough credits in selected credits to debit. If true, debit and return true.
	How to use:
	The method recieves an integer card ID in x, and a option in char t of which credit to debit. Put char 'a' to debit
	from app credits, or char 'c' to debit from card credits. Any other char will not debit from anywhere. You must
	pass tree root pointer.
	You can get a bool return form this function. True if debit was successful or false if it was not.
	Example:
	code: bool test = debitCredits(123456789,'a',bst.root); 
	This example debit credits from student with card_id, from app credits, in a tree object called bst.
	Root pointer from this object is bst.root.
	*/
	bool out = false;
	switch (t)
	{
	case 'a'://debit from app credits
		if (p == NULL || (find(x, p)->app_credit - 1) <0) {
			out = false; //return false if credit insertion fais for "id not found" or remaining credits < 0.
						  //cout << "Id not found.\n";
		}
		else {
			find(x, p)->app_credit -= 1; //debit value from app credits in x
			out = true; //return true to unlock turnstail 
		}
		break;
	case 'c':
		if (p == NULL || (find(x, p)->card_credit - 1) <0) {
			out = false; //return false if credit insertion fais for "id not found" or remaining credits < 0.
						 //cout << "Id not found.\n";
		}
		else {
			find(x, p)->card_credit -= 1; //debit value from app credits in x
			out = true; //return true to unlock turnstail 
		}
		break;
	default://credit option invalid, not debited, return false
		out = false;
		break;
	}
	return out;
}
// Finding an card_id
student* bstree::find(unsigned int x, student* &p) {
	/*Method Description:
	This method search for a student with card ID x, and return a pointer to it.
	How to use:
	The method recieves an integer card ID in x to search for. You must	pass tree root pointer.
	Example:
	code: student *a = find(123456789,bst.root);
	This example search student with 123456789 card_id and returns a student pointer which will be stored in a.
	Root pointer from this object is bst.root.
	You can get student variables using "a->app_credits" , for example, or simply "find(123456789,bst.root)->app_credits".
	*/
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
	/*Method Description:
	This method performs a simple copy from a tree to another.
	Example:
	bstree bst1, bst2;
	copy(bst1.root,bst2.root);
	This example cleans bst2 and copy all nodes from bst1 to bst2.
	*/
	makeempty(p1);
	p1 = nodecopy(p);
}
// Make a tree empty
void bstree::makeempty(student* &p)
{
	/*Method Description:
	This method cleans a bstree deletin all students.
	Example:
	bstree bst1;
	makeempty(bst1.root);
	This example cleans bst1 deleting all students, and now its root pointer points to NULL.
	*/
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
	/*Method Description:
	This method performs a simple copy from a node and returns a pointer to the new created node.
	Example:
	bstree bst1;
	student *a;
	a=nodecopy(bst1.find(123456789,bst1.root);
	This example copy the node 123456789 from bst1 to a new node, and 'a' points to this new node.
	*/
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
void bstree::del(unsigned int x, student* &p) {
	/*Method Description:
	This method delete a student from a tree.
	It recieves an integer card ID in x, and a tree root.
	Example:
	del(123456789,bst.root)
	This example deletes node 123456789 from tree bst.
	*/
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
	/*Method Description:
	This method delete the minimum value of the tree. It recieves a pointer to student. 
	It is used in del method.
	*/
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
	/*Method Description:
	This method returns the maximum value between do integer.
	It recieves two integers to compare. 
	It is a suport method used in shift methods.
	*/
	return ((value1 > value2) ? value1 : value2);
}
unsigned short int bstree::bsheight(student* p)
{
	/*Method Description:
	This method returns the height from a tree.
	It recieves a student pointer that must be the tree root.
	Example:
	height = bsheight(bst.root);
	This example stores an integer value of the height of bst tree.
	*/
	unsigned short int t;
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

student* bstree::srl(student* &p1) {
	/*Method Description:
	This method is a suport method used in insert method to shift tree right and left and keep it balanced.
	*/
	student* p2;
	p2 = p1->left;
	p1->left = p2->right;
	p2->right = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(bsheight(p2->left), p1->height) + 1;
	return p2;
}
student* bstree::srr(student* &p1) {
	/*Method Description:
	This method is a suport method used in insert method to shift tree two times right and keep it balanced.
	*/
	student* p2;
	p2 = p1->right;
	p1->right = p2->left;
	p2->left = p1;
	p1->height = max(bsheight(p1->left), bsheight(p1->right)) + 1;
	p2->height = max(p1->height, bsheight(p2->right)) + 1;
	return p2;
}
student* bstree::drl(student* &p1) {
	/*Method Description:
	This method is a suport method used in insert method to shift tree right and left and keep it balanced.
	*/
	p1->left = srr(p1->left);
	return srl(p1);
}
student* bstree::drr(student* &p1) {
	/*Method Description:
	This method is a suport method used in insert method to shift two times right and keep it balanced.
	*/
	p1->right = srl(p1->right);
	return srr(p1);
}

// Inorder Printing
void bstree::inorder(student* p)
{
	/*Method Description:
	This method just prints the tree in crescent order.
	Used to test visually if balancing is correct. If order is correct, than balancing was correct.
	It must recieve a root pointer as parameter.
	Example:
	inorder(bst.root);
	*/
	if (p != NULL)
	{
		inorder(p->left);
		cout << p->card_id << "\t";
		inorder(p->right);
	}
}

