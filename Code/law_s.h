#ifndef SPEC_LAW_H
#define SPEC_LAW_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"


bool Law::operator==(Law* a){
	if (this->name==a->name && this->year==a->year) return true;
	return false;
};

bool Law::operator!(){	if (this==no_law) return 1;	return 0;}

bool Law::operator<(int k){if (year<k) return 1; return 0;}
bool Law::operator<=(int k){if (year<=k) return 1; return 0;}
bool Law::operator>(int k){if (year>k) return 1; return 0;}
bool Law::operator>=(int k){if (year>=k) return 1; return 0;}
		bool Law::operator<(Law* o){return year<o->year;};
		bool Law::operator<=(Law* o){return year<=o->year;};
		bool Law::operator>(Law* o){return year>o->year;};
		bool Law::operator>=(Law* o){return year>=o->year;};


Law::Law(string _name, int _year, Theory* context=GLOBAL){
	name=_name;
	year=_year;
	for(int i=0; i<N_inf; i++){
		built_from[i]=no_law;
	}
	if(!!*context) context->add(this); 
	if(context!= GLOBAL) GLOBAL->add(this);		
}

int Law::sort_inf(){
	bool done=false;
	Law* temp=no_law;
	int i=0;
	while(!done){
		done=true; 
		i=0;
		while(i<N_inf-1 && !!(*built_from[i]) && !!(*built_from[i+1]) ){
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
}

int Law::Set_up(Law* L){ //this è indotto da L
	for(int j=0; j<N_str; j++){
		if(built_from[j]==L) return -2;
		if(!(*built_from[j])){
			built_from[j]=L;	
			sort_inf();	
			return j+1;
		};
	}
	return -1;
}


INF_S Law::inf_state(Theory* context=GLOBAL, int y=TODAY){
	if(! *this) return ERROR;
	if( context->involve(this, y) == false) return ERROR;
	for(int i=0; i<N_inf && !!*built_from[i]; i++){
		if(context->involve(built_from[i], y)) return theorem;					//è teorema
	}
	return axiom;	
}

string Law::inf_string(Theory* context=GLOBAL, int y=TODAY){
	INF_S is=inf_state(context,y);
	if (is==ERROR) return " errore ";
	if (is==axiom) return " assioma ";
	if (is==theorem) return " teorema ";
}




#endif
