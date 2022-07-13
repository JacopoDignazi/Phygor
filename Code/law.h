#ifndef LAW_H
#define LAW_H
#include "base.h"


class Law{

	string name;
	int year;
	
//	Math* ground;	
	Law* built_from[N_inf];
//	Law* axioms[N_law];

	int sort_inf();
	int rec_ax(Law**, int&, Theory*, int, char);
	int rec_display_ax(bool*,Theory*,int,int,int);
//	int Set_axioms();
	
	friend Exp;
	friend LEM;
	friend Theory;
	
	public:
		Law(string, int, Theory*);

		int Set_up(Law*);
		INF_S inf_state(Theory*, int);
		string inf_string(Theory*, int);
		
//		Math* Get_ground(){return ground;}
		string Get_name(){return name;}
		int Get_year(){return year;}
		
		
		int Put_axioms(Law**, Theory*,int);
		int display_axioms(Theory*,int,int);
		bool is_deducted_from(Law*,Theory*,int);		
		bool is_ciclic(Theory*,int);



		Law(){name="NO_LAW"; year=0;}
		bool operator==(Law* other_law);
		bool operator!();
		bool operator<(int);
		bool operator<=(int);
		bool operator>(int);
		bool operator>=(int);
		bool operator<(Law*);
		bool operator<=(Law*);
		bool operator>(Law*);
		bool operator>=(Law*);
};

Law* no_law= new Law();
#endif
	
