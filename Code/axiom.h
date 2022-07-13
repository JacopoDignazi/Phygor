#ifndef AXIOM_H
#define AXIOM_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"

#define SPACE_BAR_LEFT 4 
#define SPACE_BEG_INTR 4
#define SPACE_INTERNAL 9
#define LENGHT_ORIZ SPACE_INTERNAL-SPACE_BEG_INTR
#define LENGHT_VERT 2
#define MX_NEST 18

static int  check_counter;
static Law* checker_line[N_law];
static bool teller_ciclic;
static Law* searched=no_law;
static bool teller_found;


/////////////////////////////////////////NB posso fare funzioni che sortano e mappano per inclusione di assiomi				


int Law::rec_ax(Law* f_ax[N_law], int& count, Theory* context=GLOBAL, int year=TODAY, char print='n'){
	if(count >= N_law) {
		if(print=='s') cout<<"\n\n\n  conteggio ax= "<<count<<" spazio in memoria finito"; 
		return 0;
	}
	int check_counter_beg=check_counter;
	if(!context->involve(this,year) ) return 0;
	if(!!*searched && *searched==this) teller_found=true; 	
	///////////////////////////////////////controlli sulla checker_line
	for(int i=0; i< check_counter; i++){
		if(checker_line[i] == this){
			if(print=='s'){
				cout<<endl<<endl<<"  inferenza ciclica"<< (i==0? " ":" annidata in ")<<checker_line[0]->name
				<<" :"<<checker_line[i]->name<<" ==> "<<name;
			}
			teller_ciclic=true;
			return -2;
		}
	}
	checker_line[check_counter]=this;
	++check_counter;
	///////////////////////////////////////algoritmo
	if(this->inf_state(context,year) == axiom ) {
		for(int j=0; j<N_law; j++){	
			if(!*f_ax[j]) {
				f_ax[count]=this;
				++count;
				break;
			}					
			if(*f_ax[j] == this) break;
		}
	}
	else{
		for(int i=0; i<N_law && !!*built_from[i]; i++){	
			if(built_from[i] == this) {//////////////controllo preventivo su ciclicità triviali
				if(print=='s'){
					cout<<endl<<endl<<" inferenza ciclica diretta ("
					<<name<<" ==> "<<built_from[i]->name<<")";
				}
				teller_ciclic=true;
				continue;
			}
			built_from[i]->rec_ax(f_ax, count, context, year);
		}
	}
	///////////////////////////////////////risistemazione checkerline 	
	int s_c=0;
	for(int j=check_counter_beg; j<check_counter; j++){
		if(*checker_line[j] == this) {checker_line[j]=no_law; ++s_c;}
	}
	for(int i=0; !!*built_from[i]; i++){
		for(int j=check_counter_beg; j<check_counter; j++){
			if(*checker_line[j] == built_from[i]) {checker_line[j]=no_law; ++s_c;}
		}
	}
	check_counter-=s_c;
	for(int i=check_counter_beg; i<N_law; i++){
		if(!*checker_line[i]){
			int j=i;
			while(j<N_law){
				if(!!*checker_line[j]){
					checker_line[i]=checker_line[j];
					break;
				}
				j++;
			}
		}
	}
	return 1;
}

int Law::Put_axioms(Law* f_ax[N_law], Theory* context=GLOBAL, int year=TODAY){
	int c=0;
	for(int j=0; j<N_law; j++) f_ax[j]=no_law;
	for(int i=0; i<N_law; i++) checker_line[i]=no_law;
	check_counter=0;
	rec_ax(f_ax,c,context,year);	
	return c;
}

int Law::rec_display_ax(bool bars[MX_NEST], Theory* context=GLOBAL, int year=TODAY, int space_left= SPACE_BAR_LEFT, int nesting=0){
	int check_counter_beg=check_counter;
	if(nesting == 0) {
		cout<<endl<<endl<<endl;
		cout.width(SPACE_BAR_LEFT); cout<<" ";
		cout<<left<<this->name;
	}
	if(nesting != 0) cout<<name;

	if(nesting>=MX_NEST) {cout<<"\n\n\n livello di nesting: "<<nesting<<" su massimo di "<<MX_NEST; return -1;}
	if(!context->involve(this,year) ){
		cout<<" : non presente in "<<context->name<<" nel "<<year;
		return -1;
	}
	///////////////////////////////////////controlli sulla checker_line
	for(int i=0; i< check_counter; i++){
		if(checker_line[i] == this){
			cout<<" : inferenza ciclica"<< (i==0? " in ":" annidata in ") <<checker_line[0]->name
			<< " ("<<checker_line[i]->name<<" ==>"<<(i==0? " ":" [...]==> ")<<name<<")";
			return -2;
		}
	}
	checker_line[check_counter]=this;
	++check_counter;
	///////////////////////////////////////algoritmo
	if(this->inf_state(context,year) == axiom ) {
		cout<<" "<<this->year;	
	}
	else{
		++nesting;
		bars[nesting-1]=true;
		for(int i=0; i<N_law && !!*built_from[i] ; i++){	
			for(int j=0; j<LENGHT_VERT;j++){
				cout<<endl;
				for(int k=0; k<nesting; k++){
					cout.width(k==0? SPACE_BEG_INTR+SPACE_BAR_LEFT:SPACE_INTERNAL); 
					cout<<right<<(bars[k]? "|":" ");
				}
			}
			for(int k=0; k<LENGHT_ORIZ; k++) cout<<left<<"-";
			bars[nesting-1]=true;
			if(!*built_from[i+1]) bars[nesting-1]=false;
			built_from[i]->rec_display_ax(bars,context,year,space_left,nesting);
		}
		
	}
	///////////////////////////////////////risistemazione checkerline 	
	int s_c=0;
	for(int j=check_counter_beg; j<check_counter; j++){////////tolgo this
		if(*checker_line[j] == this) {checker_line[j]=no_law; ++s_c;}
	}
	for(int i=0; !!*built_from[i]; i++){///////tolgo le sue inducenti prime
		for(int j=check_counter_beg; j<check_counter; j++){
			if(*checker_line[j] == built_from[i]) {checker_line[j]=no_law; ++s_c;}
		}
	}
	check_counter-=s_c;
	for(int i=check_counter_beg; i<N_law; i++){////////////////riempio i vuoti
		if(!*checker_line[i]){
			int j=i;
			while(j<N_law){
				if(!!*checker_line[j]){
					checker_line[i]=checker_line[j];
					break;
				}
				j++;
			}
		}
	}
	return 1;
}

int Law::display_axioms(Theory* context=GLOBAL, int year=TODAY, int space_left=SPACE_BAR_LEFT){
	int c=0;
	for(int i=0; i<N_law; i++) checker_line[i]=no_law;
	check_counter=0;
	bool bars[MX_NEST];
	for(int i=0; i<MX_NEST; i++) bars[i]=true;
	rec_display_ax(bars,context,year,space_left,c);
}


int Exp::rec_ax(Law* f_ax[N_law], int& count, Theory* context=GLOBAL, int year=TODAY, char print='n'){
	for(int k=0; k<N_inf && !!*built_from[k];k++){
		built_from[k]->rec_ax(f_ax,count,context,year,print);
	}
	return 0;
}

int Exp::Put_axioms(Law* f_ax[N_law], Theory* context=GLOBAL, int year=TODAY){
	int c=0;
	for(int j=0; j<N_law; j++) f_ax[j]=no_law;
	for(int i=0; i<N_law; i++) checker_line[i]=no_law;
	check_counter=0;
	rec_ax(f_ax,c,context,year);
	return c;
}


int LEM::Put_axioms(Law* f_ax[N_law], char from='t', Theory* context=GLOBAL, int year=TODAY){
	int c=0;
	for(int j=0; j<N_law; j++) f_ax[j]=no_law;
	for(int i=0; i<N_law; i++) checker_line[i]=no_law;
	check_counter=0;
	if(from== 't' || from=='a'){ thesis->rec_ax(f_ax,c,context,year); }
	if(from== 's' || from=='a'){ observation->rec_ax(f_ax,c,context,year);}
	return c;
}


int Theory::Put_axioms(Law* f_ax[N_law], char from='t',char where='h', int year=TODAY){
	int c=0;
	for(int j=0; j<N_law; j++) f_ax[j]=no_law;
	for(int i=0; i<N_law; i++) checker_line[i]=no_law;
	check_counter=0;
	Theory* context=GLOBAL;
	if(where=='h') context=this;
	
	if(from== 't' || from== 'a'){
		for(int i=0; i<N_law && !!*law[i]; i++){
			law[i]->rec_ax(f_ax, c, context, year);
		}
	}
	if(from== 's' || from== 'a'){
		for(int i=0; i<N_exp && !!*exp[i]; i++){
			exp[i]->rec_ax(f_ax, c, context, year);
		}
	}
	return c;
}


bool Law::is_ciclic(Theory* context=GLOBAL, int year=TODAY){
	teller_ciclic=false;
	Law* mhm[N_law];
	for(int i=0; i<N_law; i++) mhm[i]=no_law;
	int c=0;
	this->rec_ax(mhm,c,context,year,'n');
	return teller_ciclic;
}

bool Law::is_deducted_from(Law* l,Theory*t=GLOBAL,int y=TODAY){
	if(t->involve(this,y)==false) return false;	
	if(t->involve(l,y)==false) return false;
	searched=l;
	teller_found=false;
	Law* mhm[N_law];
	for(int i=0; i<N_law; i++) mhm[i]=no_law;
	int c=0;
	this->rec_ax(mhm,c,t,y,'n');
	searched=no_law;
	return teller_found;	
}

bool Exp::is_ciclic(Theory* context=GLOBAL, int year=TODAY){
	if(context->involve(this,year) == false) return false;
	for(int j=0; j<N_str && !!*built_from[j]; j++){
		if(context->involve(built_from[j], year) ){
			if( this->year < built_from[j]->year){
				return true;
			}
		}
	}
	return false;
}

bool Exp::is_deducted_from(Law* l,Theory*t=GLOBAL,int y=TODAY){
	if(t->involve(this,y)==false) return false;	
	if(t->involve(l,y)==false) return false;
	searched=l;
	teller_found=false;
	Law* mhm[N_law];
	for(int i=0; i<N_law; i++) mhm[i]=no_law;
	int c=0;
	this->rec_ax(mhm,c,t,y,'n');
	searched=no_law;
	return teller_found;	
}

bool LEM::is_ciclic(Theory* context=GLOBAL, int year= TODAY){
	if(context->involve(this,year) == false) return false;
	for(int j=0; j<N_str && !!*assumption[j]; j++){
		if( context->involve(assumption[j],year) == false ) continue;
		if(assumption[j]->is_deducted_from(thesis, context, year)) return true;
	}
	return false;
}

bool LEM::is_deducted_from(Law* l, char asa= 't',Theory*t=GLOBAL,int y=TODAY){
	if(t->involve(this,y)==false) return false;	
	if(t->involve(l,y)==false) return false;
	searched=l;
	teller_found=false;
	Law* mhm[N_law];
	for(int i=0; i<N_law; i++) mhm[i]=no_law;
	this->Put_axioms(mhm,asa,t,y);
	searched=no_law;
	return teller_found;	
}


	

#endif
