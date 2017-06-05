class student{
public:
	unsigned int card_id;
	student* left;
	student* right;
	unsigned int height;
	short int card_credit;
	short int app_credit;
	student(){
		card_id = 0;
		left = 0;
		right = 0;
		height = 0;
		card_credit = 0;
	};
};


