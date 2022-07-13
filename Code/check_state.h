#ifndef CHECK_STATE_H
#define CHECK_STATE_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"
#include "axiom.h"
#include "relation_state.h"
#include "chrono.h"

#define NOinf		29
#define NOthesis	41
#define NOstrum		43
#define NOlem		NOthesis*NOstrum
#define ISciclic	47

int Theory::map_state(Law* l, char formally='e', int y=TODAY){
	int no_inf= NOinf;
	int no_thesis=NOthesis;	
	int no_strum=NOstrum;
	int is_ciclic=ISciclic;
	
	if(!involve(l,y)) return NOTINV;

	if(l->is_ciclic(this,y) == false) is_ciclic=1;  

	if(!!* search_law_ax(l,'i',formally,y) ) no_inf=1; 
	else if(!!* search_law_ax(l,'d',formally,y) ) no_inf=1;
	else if(!!* search_law_ax(l,'s',formally,y) ) no_inf=1;

	if(!!* search_lem_ax(l,'d','t',formally,y) ) no_thesis=1; 
	else if(!!* search_lem_ax(l,'s','t',formally,y) ) no_thesis=1;

	if(!!* search_lem_ax(l,'d','s',formally,y) ) no_strum=1;
	else if(!!* search_lem_ax(l,'s','s',formally,y) ) no_strum=1;	
	
	return no_inf*no_thesis*no_strum*is_ciclic;
}

int Theory::map_state(Exp* e, char formally='e', int y=TODAY){	
	int no_inf= NOinf;
	int no_observ=NOlem;
	int is_ciclic=ISciclic;
	if(!involve(e,y)) return NOTINV;
	
	if(e->is_ciclic(this,y) == false) is_ciclic=1;
	
/*	if(!!* search_law_ax(l,'d',formally,y) ) no_inf=1;
	else if(!!* search_law_ax(l,'s',formally,y) ) no_inf=1;
	
	if(!!* search_lem_ax(l,'d','t',formally,y) ) no_thesis=1;
	else if(!!* search_lem_ax(l,'s','t',formally,y) ) no_thesis=1;
	
	if(!!* search_lem_ax(l,'d','s',formally,y) ) no_strum=1;
	else if(!!* search_lem_ax(l,'s','s',formally,y) ) no_strum=1;*/

	for(int i=0; i<N_str && !!* e->built_from[i]; i++){
		if(this->involve(e->built_from[i],y) == false) continue;
		no_inf=1; 
		break;
	}
	
	LEM* lem_f=no_lem;
	for(int j=1; !!* (lem_f=search_lem(e,j) ); j++){
		if(*lem_f > y) continue;
		no_observ=1;
		break;	
	}
		
	return no_inf*no_observ*is_ciclic;
}

int Theory::map_state(LEM* relation, int y=TODAY){
	if(!involve(relation,y)) return NOTINV;
	int direct_ciclic=ISciclic;	
	int inf_val=inf_rel(relation->thesis, relation->observation, y);//torna ACB BCA BindA AindB
	if(!relation->is_ciclic(this,y) ) direct_ciclic=1;
	return inf_val*direct_ciclic;
}

/*
int Theory::map_state(Law* a, Law* b, int y=TODAY){
	int a_induce_b=  13;
	int b_induce_a=  11;
	/*int a_include_b= 2; competenza di ax_cmp
	int b_include_a=  3;
	int intersecting=  5;*/
/*	int ax_val=ax_cmp(a,b,this,y);
	if(!involve(a,y) || !involve(b,y) ) return -1;
	if(!a->is_deducted_from(b,this,y) ) b_induce_a=1;
	if(!b->is_deducted_from(a,this,y) ) a_induce_b=1;	
	return ax_val*a_induce_b*b_induce_a;
}

int Theory::map_state(Exp* a, Exp* b, int y=TODAY){
//	exp non può indurre exp
	/*int a_include_b= 2; competenza di ax_cmp
	int b_include_a=   3;
	int intersecting=  5;*/
/*	int ax_val=ax_cmp(a,b,this,y);
	if(!involve(a,y) || !involve(b,y) ) return -1;
	return ax_val;
}

int Theory::map_state(Law* a, Exp* b, int y=TODAY){
	int a_induce_b= 13;
//	exp non può indurre law
	/*int a_include_b= 2; competenza di ax_cmp
	int b_include_a=   3;
	int intersecting=  5;*/
/*	int ax_val=ax_cmp(a,b,this,y);
	if(!involve(a,y) || !involve(b,y) ) return -1;
	if(!b->is_deducted_from(a,this,y) ) a_induce_b=1;	
	return ax_val*a_induce_b;
}

*/



int Theory::print_uninferencing_laws(int year=TODAY){
	int count=0;
	bool val_state;
	for(int i=0; i<N_law && !!*law[i]; i++){
		if(!involve(law[i],year) )continue;	
		val_state=map_state(law[i],'e',year);			
		if(!(val_state%NOinf) ){
			cout<<endl<<"       |  "<<law[i]->name<<" "<<law[i]->year<<" non ha inferenze";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
}

int Theory::print_undefined_exps(int year=TODAY){
	int count=0;
	bool val_state;
	for(int i=0; i<N_exp && !!*exp[i]; i++){
		if(!involve(exp[i],year) )continue;		
		val_state=map_state(exp[i],'e',year);
		if(!(val_state%NOinf)){
			cout<<endl<<"       |  "<<exp[i]->name<<" "<<exp[i]->year<<": non e' costruito";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
}


int Theory::print_unrelated_laws(char asa='t', int year=TODAY){
	int count=0;
	int val_state;
	for(int i=0; i<N_lem && !!*lem[i]; i++){
		if(!involve(lem[i],year) )continue;
		val_state=map_state(law[i],'e',year);
		if(asa=='a' && !(val_state%NOlem)
		|| asa=='t' && !(val_state%NOthesis)
		|| asa=='s' && !(val_state%NOstrum) ){	
			cout<<endl<<"       |  "<<law[i]->name<<" non ha relazioni lem";
			if(asa=='a') cout<<" in generale, ";
			if(asa=='t') cout<<" come tesi, ";
			if(asa=='s') cout<<" come assunto, ";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
}

int Theory::print_unrelated_exps(int year=TODAY){
	int count=0;
	int val_state;
	for(int i=0; i<N_exp && !!*exp[i]; i++){
		if(!involve(exp[i],year) )continue;		
		val_state=map_state(exp[i],'e',year);
		if(!(val_state%NOlem)){	
			cout<<endl<<"       |  "<<exp[i]->name<<" non ha relazioni lem";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
}


int Theory::print_eq_ax(int year=TODAY){
	int count=0;
	int val_state;
	for(int i=0; i<N_law && !!*law[i]; i++){
		if(*law[i] > year ) continue;
		for(int j=0; j<N_law && !!*law[j]; j++){
			if(i==j) continue;
			if(*law[j] > year ) continue;
			val_state=inf_rel(law[i],law[j],year);
			if(!(val_state%AsaxB)){
				cout<<endl<<"       |  "<<law[i]->name<<" ha gli stessi assiomi di "<<law[j]->name;
				if(this != GLOBAL ) cout<<" nella teoria "<<name;
				if(year != TODAY  ) cout<<" in anno "<<year;
				++count;
			}
		}
	}
	//////////////faccio la stessa cosa con gli exp?
	return count;
}


int Theory::print_ciclic_inference(int year=TODAY){
	int count=0;
	for(int i=0; i<N_law && !!*law[i]; i++){
		if(*law[i] > year) continue;
		if(law[i]->is_ciclic(this,year)){
			cout<<endl<<"       |  "<<law[i]->name<<" ==> [...] ==>"<<law[i]->name;
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
/*	int count=0; //////////////monumento all'ottimizzazione, e all'efficacia di un lavoro più profondo
	Law* i_found[2][N_law];
	bool flag=false;
	for(int i=0; i<N_law && !!*law[i]; i++){
		for(int j=0; j<N_inf && !!*law[i]->built_from[j] ; j++){
			if(*law[i]->built_from[j]== law[i]){
				cout<<endl<<"       |  "<<law[i]->name<<" ==> "<<law[i]->built_from[j]->name;
				++count;
			}
			if(this->involve(law[i]->built_from[j])==false) continue;
	
			for(int k=0; k<N_inf && !!*law[k]; k++){ 
				if(i==k) continue;
				flag=false;
				for(int r=1; r<=count; r++){
					if(*i_found[0][r]== law[i] && *i_found[1][r]==law[k]) flag=true;
					if(*i_found[0][r]== law[k] && *i_found[1][r]==law[i]) flag=true;
				}
				if(flag) continue;	
				if(*law[k]==law[i]->built_from[j]){
					for(int h=0; h<N_inf && !!*law[k]->built_from[h] ; h++){
						if(*law[i]== law[k]->built_from[h]){
							cout<<endl<<"       |  "<<law[i]->built_from[j]->name<<" ==> "<<law[i]->name
							<<"  and  "<<law[k]->built_from[h]->name<<" ==> "<<law[k]->name;
							++count;
							i_found[0][count]=law[i];
							i_found[1][count]=law[k];
						}
					}
				}
			}
			
		}
	}
	return count;*/
}
		
int Theory::print_ciclic_set_up(int year=TODAY){		
	int count=0;
	for(int i=0; i<N_exp && !!*exp[i]; i++){
		if(*exp[i] > year) continue;
		if(exp[i]->is_ciclic(this,year)){
			cout<<endl<<"       |  "<<exp[i]->name<<" ("<<exp[i]->year<<") "
			<<" risulta costruito con un assunzione successiva alla sua data";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
	return count;
}	

int Theory::print_conflicting_lems(int year=TODAY){
	int count=0;
	for(int i=0; i<N_lem && !!*lem[i]; i++){
	if(*lem[i] > year) continue;		
	for(int j=0; j<N_lem && !!*lem[j]; j++){ 
		if(i==j) continue;
		if(*lem[j] > year) continue;
		if(*lem[i]->thesis == lem[j]->thesis && *lem[i]->observation == lem[j]->observation){
			if( lem[i]->lem_value == lem[j]->lem_value ){cout<<" \a LEM DOPPIO ";};
			if(	lem[i]->lem_value != lem[j]->lem_value ){
				cout<<endl<<"       |   ("<<lem[i]->thesis->name<<","
				<<lem[i]->observation->name<<") --> sia vero che falso";
				if(this != GLOBAL ) cout<<" nella teoria "<<name;
				if(year != TODAY  ) cout<<" in anno "<<year;				
				++count;
			}
		}
	} }
	return count;
}


int Theory::print_tautologic_lems(int year=TODAY){
	int count=0;
	int val_state;
	for(int i=0; i<N_lem && !!*lem[i]; i++){
		if(*lem[i] > year ) continue;
		val_state=map_state(lem[i],year);
		if(!(val_state%A_C_B)){				
			cout<<endl<<"       |  "<<lem[i]->thesis->name
			<<" ha una forma uguale o inclusa in "<<lem[i]->observation->name<<", ";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;
			++count;
		}
	}
}

int Theory::print_ciclic_lems(int year=TODAY){
	int count=0;
	int val_state;				
	for(int i=0;i<N_lem && !!*lem[i]; i++){
		if(*lem[i] > year ) continue;
		val_state=map_state(lem[i],year);
		if(!(val_state%AindB) ){		
			cout<<endl<<"       |   nell'"<<lem[i]->observation->name<<": la tesi "
			<<lem[i]->thesis->name<<" confronta con una assunzione ("
			<<"<==[...]<=="<<lem[i]->thesis->name<<")  esplicitamente dedotta dalla tesi, ";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;					
			++count;
		}
		if(!(val_state%B_C_A) ){		
			cout<<endl<<"       |   nell'"<<lem[i]->observation->name<<": la tesi "
			<<lem[i]->thesis->name<<" confronta con assunzioni contenute nella tesi (axT C axA), ";
			if(this != GLOBAL ) cout<<" nella teoria "<<name;
			if(year != TODAY  ) cout<<" in anno "<<year;					
			++count;
		}		
	}
	return count;
}
				
			
			
#endif
