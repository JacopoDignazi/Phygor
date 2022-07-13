#ifndef EXP_H
#define EXP_H

#include "base.h"

class Exp{
	
	string name;
	int year;
	Law* built_from[N_str]; 	

	int sort_built();
	int rec_ax(Law**, int&, Theory*, int, char);
		
	friend LEM;
	friend Theory;

	public:
		Exp(string, int, Theory*);		
		int Set_up(Law*);
	
		int Put_axioms(Law**, Theory*, int);
		bool is_ciclic(Theory*,int);
		bool is_deducted_from(Law*,Theory*,int);
		
		string Get_name(){return name;}
		int Get_year(){return year;}
		
		Exp(){name="NO_EXP"; year=0;}		
		bool operator!();
		bool operator==(Exp*);
		bool operator<(int);
		bool operator<=(int);
		bool operator>(int);
		bool operator>=(int);
		bool operator<(Exp*);
		bool operator<=(Exp*);
		bool operator>(Exp*);
		bool operator>=(Exp*);
};

Exp* no_exp= new Exp();
#endif
