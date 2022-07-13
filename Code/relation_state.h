#ifndef RELATION_STATE_H
#define RELATION_STATE_H

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

#define B_C_A 3
#define INTER 5
#define A_C_B 7
#define AindB 13
#define BindA 17
#define AsaxB A_C_B*B_C_A
#define SAMEOBJ 37
#define NOTINV  19



int ax_cmp(Law* ax_a[N_law],Law* ax_b[N_law],Theory* t=GLOBAL,int y=TODAY){
	int intersecting=1;
	int a_subset_b= A_C_B;		/////////significa a ha più assiomi, e che a è deducibile da b+altroev
	int b_subset_a= B_C_A;		/////////significa b ha più assiomi, e che b è deducibile da a+altroev
	for(int i=0;i<N_law && !!*ax_a[i]; i++){
		for(int j=0; j<N_law; j++){
			if(!*ax_b[j]){ b_subset_a =1; break;}/////significa che c'è un ax_a[i] != tutti gli ax_b[j]			
			if(*ax_b[j] == ax_a[i]) {intersecting=INTER; break;}///////quindi b non è deducibile da a
		}														//////cioé b non è sottoinsieme di a
	}
	for(int i=0;i<N_law && !!*ax_b[i]; i++){
		for(int j=0; j<N_law; j++){
			if(!*ax_a[j]){ a_subset_b =1; break;}			
			if(*ax_a[j] == ax_b[i]) {                 break;}
		}
	}
	return intersecting*a_subset_b*b_subset_a;
}


int Theory::inf_rel(Law* a, Law* b, int y=TODAY){
	if(!involve(a,y) || !involve(b,y) ) return NOTINV;		
	if(*a==b) return SAMEOBJ;
	Law* ax_a[N_law];
	Law* ax_b[N_law];
	a->Put_axioms(ax_a,this,y);
	b->Put_axioms(ax_b,this,y);
	int a_induce_b=  AindB;
	int b_induce_a=  BindA;
	int ax_val=ax_cmp(ax_a,ax_b,this,y);
	if(!a->is_deducted_from(b,this,y) ) b_induce_a=1;
	if(!b->is_deducted_from(a,this,y) ) a_induce_b=1;	
	
	return ax_val*a_induce_b*b_induce_a;	
	
}

int Theory::inf_rel(Exp* a, Exp* b, int y=TODAY){
	if(!involve(a,y) || !involve(b,y) ) return NOTINV;	
	if(*a==b) return SAMEOBJ;
	Law* ax_a[N_law];
	Law* ax_b[N_law];
	a->Put_axioms(ax_a,this,y);
	b->Put_axioms(ax_b,this,y);
	int ax_val=ax_cmp(ax_a,ax_b,this,y);	
/*	if(!a->is_deducted_from(b,this,y) ) b_induce_a=1;
	if(!b->is_deducted_from(a,this,y) ) a_induce_b=1;	*/

	return ax_val;
}

int Theory::inf_rel(Law* a, Exp* b, int y=TODAY){
	if(!involve(a,y) || !involve(b,y) ) return NOTINV;		
	Law* ax_a[N_law];
	Law* ax_b[N_law];
	a->Put_axioms(ax_a,this,y);
	b->Put_axioms(ax_b,this,y);
	int ax_val=ax_cmp(ax_a,ax_b,this,y);
/*	if(!a->is_deducted_from(b,this,y) ) b_induce_a=1;
	if(!b->is_deducted_from(a,this,y) ) a_induce_b=1;	*/	
	
	return ax_val;
}


Law* Theory::search_law_ax(Law* ax[N_law], char rel, int y=TODAY, int n=1){
	Law* array_ax[N_law];
	int val=0;
	for(int i=0; i<N_law && !!*law[i]; i++){
		law[i]->Put_axioms(array_ax,this,y);		
		val=ax_cmp(ax,array_ax,this,y);				
		if(val==0) continue;																	
		if(rel=='s'){
			if(!(val%AsaxB)){							///////////stessi assiomi uguali																
				if(n<=1) return law[i];
				--n; continue;
			}
		}
		if(rel=='d'){					////////law[i](b) dedotto da l(a): trova leggi che includono tutti quegli assiomi
			if(!(val%B_C_A) && val%AsaxB){////quindi A piùgrande di B	///////////array_ax include ax
																
				if(n<=1) return law[i];
				--n; continue;
			}
		}
		if(rel=='i'){					////////l dedotto da law[i]: trova leggi cui parte degli assiomi sono tutti quelli cercati
			if(!(val%A_C_B) && val%AsaxB){////quindi B piùgrande di A	//////////ax include array_ax
				if(n<=1) return law[i];						
				--n; continue;
			}
		}
	}
	return no_law;
}

Exp* Theory::search_exp_ax(Law* ax[N_law], char rel, int y=TODAY, int n=1){
	Law* array_ax[N_law];
	int val=0;
	for(int i=0; i<N_exp && !!*exp[i]; i++){
		exp[i]->Put_axioms(array_ax,this,y);
		val=ax_cmp(ax,array_ax,this,y);
		if(rel=='s'){
			if(!(val%AsaxB)){							///////////stessi assiomi uguali
				if(n<=1) return exp[i];
				--n; continue;
			}
		}
		if(rel=='d'){
			if(!(val%B_C_A) && val%AsaxB){							///////////array_ax include ax
				if(n<=1) return exp[i];
				--n; continue;
			}
		}
		if(rel=='i'){
			if(!(val%A_C_B) && val%AsaxB){							//////////ax include array_ax
				if(n<=1) return exp[i];
				--n; continue;
			}
		}
	}
	return no_exp;
}

LEM* Theory::search_lem_ax(Law* ax[N_law], char rel, char asa='t', int y=TODAY, int n=1){
	Law* array_ax[N_law];
	int val=0;
	for(int i=0; i<N_lem && !!*lem[i]; i++){
		lem[i]->Put_axioms(array_ax,asa,this,y);
		val=ax_cmp(ax,array_ax,this,y);
		if(rel=='s'){
			if(!(val%AsaxB)){							///////////stessi assiomi uguali
				if(n<=1) return lem[i];
				--n; continue;
			}
		}
		if(rel=='d'){
			if(!(val%B_C_A) && val%AsaxB){							///////////array_ax include ax
				if(n<=1) return lem[i];
				--n; continue;
			}
		}
		if(rel=='i'){
			if(!(val%A_C_B) && val%AsaxB){							//////////ax include array_ax
				if(n<=1) return lem[i];
				--n; continue;
			}
		}
	}
	return no_lem;
}

	
Law* Theory::search_law_ax(Law* l , char rel, char formally='e', int y=TODAY, int n=1){
	Law* ax_a[N_law];
	if(l->Put_axioms(ax_a, this, y) == 0) return no_law;
	Law* law_f=no_law;
	int j=1;
	while(true){
		law_f=search_law_ax(ax_a,rel,y,j);
		if(!*law_f) break;
		if(*law_f == l) {++j; continue;}
		if(formally=='e' || formally=='g'){
			if(rel == 'd' && !!(law_f->is_deducted_from(l,this,y)) ){
				if(n<=1) return law_f;
				--n;
			}
			if(rel == 'i' && !!(l->is_deducted_from(law_f,this,y)) ){
				if(n<=1) return law_f;
				--n;
			}
			if(rel == 's' && (!!(law_f->is_deducted_from(l,this,y)) || !!(l->is_deducted_from(law_f,this,y))) ){
				if(n<=1) return law_f;
				--n;
			}
		}
		if(formally=='i' || formally=='g'){
			if(rel == 'd' && !(law_f->is_deducted_from(l,this,y)) ){
				if(n<=1) return law_f;
				--n;
			}
			if(rel == 'i' && !(l->is_deducted_from(law_f,this,y)) ){
				if(n<=1) return law_f;
				--n;
			}
			if(rel == 's' && (!(law_f->is_deducted_from(l,this,y)) || !(l->is_deducted_from(law_f,this,y))) ){
				if(n<=1) return law_f;
				--n;
			}
		}		
		++j;
	}
	return no_law;
}

Exp* Theory::search_exp_ax(Law* l , char rel, char formally='e', int y=TODAY, int n=1){
	Law* ax_a[N_law];
	if(l->Put_axioms(ax_a, this, y) == 0) return no_exp;
	int i=1;
	Exp* exp_f=no_exp;
	while(true){
		exp_f=search_exp_ax(ax_a,rel,y,i);
		if(!*exp_f) break;
		if(formally=='e' || formally=='g'){
			if(rel == 'd' && !!(exp_f->is_deducted_from(l,this,y)) ){
				if(n<=1) return exp_f;
				--n;
			}
			if(rel == 's' && !!(exp_f->is_deducted_from(l,this,y))  ){
				if(n<=1) return exp_f;
				--n;
			}
		}
		if(formally=='i' || formally=='g'){
			if(rel == 'd' && !(exp_f->is_deducted_from(l,this,y)) ){
				if(n<=1) return exp_f;
				--n;
			}
			if(rel == 's' && !(exp_f->is_deducted_from(l,this,y))  ){
				if(n<=1) return exp_f;
				--n;
			}
		}		
		++i;
	}
	return no_exp;
}

LEM* Theory::search_lem_ax(Law* l , char rel, char asa='t', char formally='e', int y=TODAY, int n=1){
	Law* ax_a[N_law];
	if(l->Put_axioms(ax_a, this, y) == 0) return no_lem;
	int i=1;
	LEM* lem_f=no_lem;

	while(true){
		lem_f=search_lem_ax(ax_a,rel,asa,y,i);
		if(!*lem_f) break;
		if(formally=='e' || formally=='g'){
			if(rel == 'd' && !!(lem_f->is_deducted_from(l,asa,this,y)) ){
				if(n<=1) return lem_f;
				--n;
			}
			if(rel == 's' && !!(lem_f->is_deducted_from(l,asa,this,y)) ){
				if(n<=1) return lem_f;
				--n;
			}
		}
		if(formally=='i' || formally=='g'){
			if(rel == 'd' && !(lem_f->is_deducted_from(l,asa,this,y)) ){
				if(n<=1) return lem_f;
				--n;
			}
			if(rel == 's' && !(lem_f->is_deducted_from(l,asa,this,y)) ){
				if(n<=1) return lem_f;
				--n;
			}
		}				
		++i;
	}
	return no_lem;

	
}

	


#endif
