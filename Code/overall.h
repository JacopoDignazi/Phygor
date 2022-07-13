#ifndef OVERALL_H
#define OVERALL_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"
#include "chrono.h"

#define N_th_all 20

#define N_opt 4



bool option[N_opt]={}; 


	char sel='a';
	char s_sel= '1';
	int check;
	string text;
	string s_text;
	string text_l;
	string text_s_l;
	string text_e;
	string text_v;
	int year;
	bool value;
	Theory* context=GLOBAL;
	Law* law=no_law;
	Law* s_law= no_law;
	Law* axL[N_law];
	Exp* exp=no_exp;
	Exp* s_exp= no_exp;
	Law* axE[N_law];
	LEM* lem=no_lem;
	LEM* s_lem= no_lem;


Theory* SCIENZA= new Theory("SCIENZA");

Theory* theory[N_th_all];
int T_count=0;

Theory* search_theory(string _name){
	for(int i=0; i<=T_count && !!*theory[i]; i++){
		if (theory[i]->name == _name) return theory[i];
	};
	return no_theory;
}

Law* search_law(string name){
	return GLOBAL->search_law(name);
}

Exp* search_exp(string name){
	return GLOBAL->search_exp(name);
}

Theory* make_theory(string name){
	T_count++;
	theory[T_count]= new Theory(name);
	return theory[T_count];
}

int display_theory(){
	cout<<endl;
	for(int i=0; i<=T_count; i++){
		cout<<endl<<theory[i]->name;
	}
	cout<<endl;
	return 0;
}

#endif
