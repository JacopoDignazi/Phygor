#ifndef SPEC_EXP_H
#define SPEC_EXP_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"


bool Exp::operator==(Exp* e){ if(e->name==this->name && e->year==this->year) return 1; return 0;}
bool Exp::operator!(){	if (this==no_exp || this==NULL) return 1;	return 0;}
bool Exp::operator<(int k){if (year<k) return 1; return 0;}
bool Exp::operator<=(int k){if (year<=k) return 1; return 0;}
bool Exp::operator>(int k){if (year>k) return 1; return 0;}
bool Exp::operator>=(int k){if (year>=k) return 1; return 0;}
		bool Exp::operator<(Exp* o){return year<o->year;};
		bool Exp::operator<=(Exp* o){return year<=o->year;};
		bool Exp::operator>(Exp* o){return year>o->year;};
		bool Exp::operator>=(Exp* o){return year>=o->year;};


Exp::Exp(string _name, int _year, Theory* context=GLOBAL){
	name=_name;
	year=_year;
	for(int i=0; i<N_str; i++){
		built_from[i]=no_law;
	}
	if(!!*context) context->add(this); 
	if(context!= GLOBAL) GLOBAL->add(this);

}

int Exp::sort_built(){
	bool done=false;
	Law* temp=no_law;
	int i=0;
	while(!done){
		done=true; 
		i=0;
		while(i<N_str-1 && !!(*built_from[i]) && !!(*built_from[i+1]) ){
			if(*built_from[i]>built_from[i+1]){
				temp=built_from[i];
				built_from[i]=built_from[i+1];
				built_from[i+1]=temp; 
				done=false;
				break;
			}
			i++;
		}
	}

	for(int i=0; i<N_lem && !!*GLOBAL->lem[i]; i++){		//potrebbe essere un global->sort_assumption()
			if( * GLOBAL->lem[i]->observation == this ) {
			GLOBAL->lem[i]->sort_assumption();
		}
	}
	return 1;
}

int Exp::Set_up(Law* assumption){
	for(int i=0; i<N_str; i++){
		if(built_from[i]==assumption) return -2;
		if(!(*built_from[i])){
			built_from[i]=assumption;	
			sort_built();	
			return i;};
	}
	return -1;
}







#endif

