#ifndef SPEC_THEORY_H
#define SPEC_THEORY_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"

bool Theory::operator!(){if (this==no_theory) return 1; return 0;};

Theory::Theory(string _name){
	name=_name;
	for(int i=0; i<N_law; i++) law[i]=no_law;
	for(int i=0; i<N_exp; i++) exp[i]=no_exp;
	for(int i=0; i<N_lem; i++) lem[i]=no_lem;
//	for(int i=0; i<N_math; i++) ground[i]=NULL;
}

int Theory::add(Law* l){
	for(int i=0; i < N_law; i++){
		if(!(*law[i])) {law[i]=l; sort_law(); return 1;}
		if(law[i]==l) {return 2;}
	};
	return 0;
}

int Theory::add(Exp* e){
	for(int i=0; i<N_exp; i++){
		if(!(*exp[i])) {exp[i]=e; sort_exp(); return 1;}
		if(exp[i]==e) {return 2;}
	}
	return 0;
}

int Theory::add(LEM* r, char add=AUTO_ADD_LEMTO){
	for(int i=0; i<N_exp; i++){
		if(!(*lem[i])) {
			lem[i]=r; sort_lem(); return 1;}
		if(lem[i]==r) {return 2;}
	}
	if(add=='y'){
		this->add(r->thesis);
		this->add(r->observation);
	}		
	return 0;
}


int Theory::display_all(char mod='b'){
	for(int i=0; i<N_law && !!(*law[i]); i++) {cout<<endl<<law[i]->name<<"\t"<<law[i]->year<<"\t";}
	cout<<endl;
	for(int i=0; i<N_exp && !!(*exp[i]); i++) {cout<<endl<<exp[i]->name<<"\t"<<exp[i]->year<<"\t";}
	cout<<endl;
	for(int i=0; i<N_lem && !!(*lem[i]); i++) {
		cout<<endl<<" "<<i<<") "<<lem[i]->thesis->name<<"\t"<<lem[i]->observation->name<<"\t" <<lem[i]->year; 
		cout<<"\t\t"<<lem[i]->lem_value<<"\t"<<(int)(lem[i]->curr_state);
		cout<<endl; for(int j=0; j<N_str && !!(*(lem[i]->assumption[j])); j++ ){
			cout<<lem[i]->assumption[j]->name; 
		}
		cout<<endl;		
	}
	cout<<endl;
}

int Theory::sort_exp(){
	bool done=false;
	Exp* temp=no_exp;
	int i=0;
	while(!done){
		done=true; 
		i=0;
		while(i<(N_exp-1) && !!(*exp[i]) && !!(*exp[i+1]) ){
			if(*exp[i]>exp[i+1]){
				temp=exp[i];
				exp[i]=exp[i+1];
				exp[i+1]=temp; 
				done=false;
				break;
			}
			i++;
		}
	}
	return 1;
}

int Theory::sort_lem(){
	bool done=false;
	LEM* temp=no_lem;
	int i=0;
	while(!done){
		done=true;
		i=0; 
		while(i<N_lem-1 && !!(*lem[i]) && !!(*lem[i+1]) ){
			if(*lem[i]>lem[i+1]){
				temp=lem[i];
				lem[i]=lem[i+1];
				lem[i+1]=temp; 
				done=false;
				break;
			}
			if(lem[i]->year==lem[i+1]->year){
				if(lem[i]->thesis->year > lem[i+1]->thesis->year){
					temp=lem[i];
					lem[i]=lem[i+1];
					lem[i+1]=temp; 
					done=false;
					break;
				}
				if(lem[i]->observation->year > lem[i+1]->observation->year){
					temp=lem[i];
					lem[i]=lem[i+1];
					lem[i+1]=temp; 
					done=false;
					break;
				}
			}
				
			i++;
		}
	}
	return 1;
}

int Theory::sort_law(){
	bool done=false;
	Law* temp=no_law;
	int i=0;
	while(!done){
		done=true; 
		i=0;
		while(i<N_law-1 && !!(*law[i]) && !!(*law[i+1]) ){
			if(*law[i]>law[i+1]){
				temp=law[i];
				law[i]=law[i+1];
				law[i+1]=temp; 
				done=false;
				break;
			}
			i++;
		}
	}
	return 1;
}


bool Theory::involve(Law* l, int y=TODAY){
	if (*l > y) return 0;
	for(int i=0; i<N_law && !!(*law[i]);i++){
		if (*law[i]==l) return 1;
	}
	return 0;
}

bool Theory::involve(Exp* e, int y=TODAY){
	if(*e > y) return 0;
	for(int i=0; i<N_exp && !!(*exp[i]);i++){
		if (*exp[i]==e) return 1;
	}
	return 0;
}

bool Theory::involve(LEM* r, int y=TODAY){
	if(*r > y) return 0;
	for(int i=0; i<N_lem && !!(*lem[i]);i++){
		if (*lem[i]==r && lem) return 1;
	}
	return 0;
}

Law* Theory::search_law(string name){
	for(int i=0; i<N_law && !!* law[i]; i++){
		if (law[i]->name == name) return law[i];
	}
	return no_law;
}

Exp* Theory::search_exp(string name){
	for(int i=0; i<N_exp && !!* exp[i]; i++){
		if (exp[i]->name == name) return exp[i];
	}
	return no_exp;
}

LEM* Theory::search_lem(Law* l,Exp* e, char law_asa= 't', int n=1){
	if(!*e && law_asa != 't') return no_lem;	
	for(int i=0; i<N_lem && !!* lem[i]; i++){
		if(lem[i]->observation==e){
			if(law_asa== 't' && lem[i]->thesis==l) {
				if(n<=1) return lem[i];
				--n;
				continue;
			}
			if(law_asa== 's'){
				for(int j=0; j<N_str && !!* lem[i]->assumption[j]; j++){
					if(lem[i]->assumption[j]==l){
						if(n<=1) return lem[i];
						--n;
						continue;
					}
				}
			}
		}
	}
	LEM* temp= no_lem;
	if(law_asa == 'g'){
		temp=search_lem(l,e,'t'); if(!!*temp) return temp;
		temp=search_lem(l,e,'s'); if(!!*temp) return temp;
	}
	return no_lem;		
}

LEM* Theory::search_lem(Law* l, char law_asa= 't', int n=1){
	for(int i=0; i<N_lem && !!*lem[i]; i++){
		if(law_asa=='t'){
			if(* lem[i]->thesis== l ){
				if(n<=1) return lem[i];
				--n;
				continue;
			}
		}
		if(law_asa=='s'){
			for(int k=0; k<N_str ; k++){
				if( ! *(lem[i]->assumption[k]) ) break;
				if( *(lem[i]->assumption[k]) == l) return lem[i];
			}
		}
	}
	if(law_asa=='g'){ 
		LEM* temp=no_lem;
		if(!!*(temp=search_lem(l,'t',n)) )return temp;
		if(!!*(temp=search_lem(l,'s',n)) )return temp;
	}
	return no_lem;
}

LEM* Theory::search_lem(Exp* e, int n=1){
	for(int i=0; i<N_lem && !!*lem[i]; i++){
		if( lem[i]->observation== e){
			if(n<=1) return lem[i];
			--n;
			continue;
		} 
	}
	return no_lem;
}
/*
LEM* Theory::search_lem(Law* l, int year, char law_asa='t', int n=1){
	for(int i=0; i<N_lem && !!*lem[i] && lem[i]->year <= year ; i++){
		if(lem[i]->year==year){
			if(law_asa=='t'&& *lem[i]->thesis== l){
				if(n<=1) return lem[i];
				--n;
				continue;
			}
			if(law_asa=='s'){
				for(int k=0; k<N_str ; k++){
					if( !*(lem[i]->assumption[k]) ) break;
					if( *(lem[i]->assumption[k]) == l) {
						if(n<=1) return lem[i];
						--n;
						continue;
					}
				}
			}
		}
	}
	if(law_asa=='g'){ 
		LEM* temp=no_lem;
		if(!!*(temp=search_lem(l,year,'t',n)) )return temp;
		if(!!*(temp=search_lem(l,year,'s',n)) )return temp;
	}
	return no_lem;
}

LEM* Theory::search_lem(Exp* e, int year, int n){
	for(int i=0; i<N_exp && !!*lem[i] && *lem[i]<=year; i++){
		if(lem[i]->year==year && lem[i]->observation== e){
			if(n<=1) return lem[i];
			--n;
			continue;
		}
	}
	return no_lem;
}
*/
#endif

