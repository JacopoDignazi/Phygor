#ifndef LEM_H
#define LEM_H

#include "base.h"
#include "law.h"
#include "exp.h"


class LEM{
	Law* thesis;	
	Exp* observation;
	Law* assumption[N_str];

	LEM_V  lem_value;	
	LEM_S curr_state;		//NB volendo posso far lem_state come funzione da lem_value
	int year;
	bool safe;
	
	
	friend Theory;
	friend Exp::sort_built();
	
	public:

		LEM(Law*,Exp*, bool, Theory*, char);			
		
		int law_year(){return thesis->year;};
		int exp_year(){return observation->year;};
		LEM_V value(Theory*,int);
		LEM_S state(Theory*,int);
		INF_S inf_state(Theory*,int);
		string value_string(Theory*,int);
		string state_string(Theory*,int);
		string inf_string(Theory*,int);
		
		int Put_axioms(Law**, char, Theory*, int);
		bool is_ciclic(Theory*,int);
		bool is_deducted_from(Law*,char,Theory*,int);
		
		LEM(){safe=false;};
		int sort_assumption();
		bool operator!(){return !safe;};
		bool operator==(LEM* other);
		bool operator<(LEM* other){if (this->year < other->year) return 1; return 0;};
		bool operator>(LEM* other){if (this->year > other->year) return 1; return 0;};
		bool operator<=(LEM* other){if (this->year <= other->year) return 1; return 0;};
		bool operator>=(LEM* other){if (this->year >= other->year) return 1; return 0;};
		bool operator<(int year){if (this->year < year) return 1; return 0;};
		bool operator>(int year){if (this->year > year) return 1; return 0;};
		bool operator<=(int year){if (this->year <= year) return 1; return 0;};
		bool operator>=(int year){if (this->year >= year) return 1; return 0;};
};

LEM* no_lem=new LEM();
#endif
