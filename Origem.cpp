#include "bstree.h"
#include "windows.h"
#include "psapi.h"
#include <iostream>
using namespace std;


int main()
{
	
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));/*
	SIZE_T virtualMemUsedByMe1 = pmc.PrivateUsage;
	SIZE_T PhisicalMemUsedByMe1 = pmc.WorkingSetSize;
	cout << "Virtual memory used now: " << float(virtualMemUsedByMe1) << endl;
	cout << "Phisical Memory used: " << float(PhisicalMemUsedByMe1) << endl;
	*/
	int a, choice, findele, delele;
	bool test;
	bstree bst;

	cout << "\n\t\t\t\tWELCOME TO AVL TREE" << endl;
	cout << "\t\t\t\t:::::::::::::::::::\n" << endl;
/*
	for (unsigned int i = 0; i < 10000; i++)
	{
		bst.insert(i, root);
		bst.insertCredits(i, 10, 10, root);
	}
	SIZE_T virtualMemUsedByMe2 = pmc.PrivateUsage;
	SIZE_T PhisicalMemUsedByMe2 = pmc.WorkingSetSize;
	cout << "Delta Virtual memory used to fill tree: " << float(virtualMemUsedByMe2) - float(virtualMemUsedByMe1) << endl;
	cout << "Delta Phisical Memory used fill tree: " << float(PhisicalMemUsedByMe2) - float(PhisicalMemUsedByMe1) << endl;*/
	SIZE_T virtualMemUsedByMe3 = pmc.PrivateUsage;
	SIZE_T PhisicalMemUsedByMe3 = pmc.WorkingSetSize;
	do
	{
		cout << "\t\t::::::::::::::::::::::::::::::::::::::::::::::::" << endl;
		cout << "\t\t::::Enter 1 to insert a new student:::::::::::::" << endl;
		cout << "\t\t::::Enter 2 to insert credits:::::::::::::::::::" << endl;
		cout << "\t\t::::Enter 3 to memory info. Uncoment to use:::::" << endl;
		cout << "\t\t::::Enter 4 to search a value:::::::::::::::::::" << endl;
		cout << "\t\t::::Enter 5 to delete a value:::::::::::::::::::" << endl;
		cout << "\t\t::::Enter 6 to debit credits from student:::::::" << endl;
		cout << "\t\t::::Enter 7 to display Inorder::::::::::::::::::" << endl;
		cout << "\t\t::::Enter 8 to display Postorder::::::::::::::::" << endl;
		cout << "\t\t::::Enter 9 to display the height of the tree:::" << endl;
		cout << "\t\t::::Enter 0 to exit:::::::::::::::::::::::::::::" << endl;
		cout << "\t\t::::::::::::::::::::::::::::::::::::::::::::::::\n" << endl;

		cout << "\nEnter the choice: ";
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "\n\t\tADDING NEW NODE" << endl;
			cout << "\t\t:::::::::::::\n" << endl;
			cout << "Enter a new id card: ";
			cin >> a;
			cout << "Enter a new card credit value: ";
			int b;
			cin >> b;
			cout << "Enter a new app credit value: ";
			int c;
			cin >> c;
			bst.insert(a,b,c, bst.root);
			cout << "\nThe new value have been added to your tree successfully\n" << endl;
			break;
		case 2:
			cout << "\n\t\tADDING CREDITS" << endl;
			cout << "\t\t:::::::::::::\n" << endl;
			cout << "Enter id card: ";
			cin >> a;
			cout << "Enter card credit: ";
			cin >> b;
			cout << "Enter app credit: ";
			cin >> c;
			bst.insertCredits(a, c, b, bst.root);
			break;
		case 3:
			GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
			virtualMemUsedByMe3 = pmc.PrivateUsage;
			PhisicalMemUsedByMe3 = pmc.WorkingSetSize;
			cout << "Virtual memory used now: " << float(virtualMemUsedByMe3) << endl;
			cout << "Phisical Memory used: " << float(PhisicalMemUsedByMe3) << endl;
			break;
		case 4:
			cout << "\nEnter student to search: ";
			cin >> findele;

			if (bst.find(findele, bst.root) == NULL) {
				cout << "Element not found!\n";
			}
			else {
				cout << "Element found!\n" << "App credit: " << bst.find(findele, bst.root)->app_credit << "\nCard Credit: " << bst.find(findele, bst.root)->card_credit << endl;
			}
			break;
		case 5:
			cout << "\nEnter student to delete: ";
			cin >> delele;
			bst.del(delele, bst.root);
			cout << endl;
			break;
		case 6:
			cout << "\n\t\tDebit credits from student:" << endl;
			cout << "Enter id card: ";
			cin >> a;
			cout << "Enter a option to debit from. 'a' for app credits, 'c' for card credits : \n";
			char k;
			cin.clear();
			cin >> k;
			test = bst.debitCredits(a, 'a', bst.root);
			if (test)
				cout << "Debit ok. Unlock Turnstil.";
			else
				cout << "Debit failed.";
			cout << endl;
			break;
		case 7:
			cout << "\n\t\tIN-ORDER TRAVERSAL" << endl;
			bst.inorder(bst.root);
			cout << endl;
			break;
		case 8:
			cout << "\n\t\tPOST ORDER TRAVERSAL" << endl;
			cout << endl;
			break;
		case 9:
			cout << "\n\t\tHEIGHT\n" << endl;
			cout << "The height of the tree is: " << bst.bsheight(bst.root) << endl;
			break;
		case 0:
			cout << "\n\tThank your for using AVL tree program\n" << endl;
			break;
		default:
			cout << "Sorry! wrong input\n" << endl;
			break;
		}
		system("pause");
		system("cls");
	} while (choice != 0);
	return 0;
}

/*
int main() {
	short int a = 3;

	cout << sizeof(a) << endl;
	system("pause");
	return 0;
}*/