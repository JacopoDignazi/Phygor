#ifndef CHRONO_H
#define CHRONO_H

#include "base.h"
#include "law.h"
#include "exp.h"
#include "lem.h"
#include "theory.h"
#include "law_s.h"
#include "exp_s.h"
#include "lem_s.h"
#include "theory_s.h"



int Theory::print_chrono(int ymin=Y_BEG, int ymax=TODAY){
	int year=0;
	int i=0;
	int j=0;
	int k;
	LEM* lem_f= no_lem;
	while(!!*law[i] || !!*exp[j] ){
		if(!!*law[i] && (!*exp[j] || law[i]->year <= exp[j]->year) ){
			if(year != law[i]->year){
				year=law[i]->year;
				cout.width(8+20+5); cout<<endl<<right<<"|"; cout.width(20+5); cout<<right<<"|";
				cout.width(7);	cout<<endl<<right<<year<<":";
			}
			else{
				cout<<endl;
				cout.width(8);
				cout<<right<<" ";
			}
			cout.width(20);	cout<<right<<law[i]->name;
			cout.width(5) ; cout<<right<<"|";
			///////////////////////////scrittura legge e inferenze
			k=1;
			while( !!*(lem_f=search_lem(law[i],year,'t',k)) ){
				if(k>1) {
					cout<<endl;
					cout.width(8+20+5); cout<<"|";
				}
				if(lem_f->thesis->year == lem_f->observation->year){
					///////////////////////////scrittura lem equianno
					cout.width(20);	cout<<internal<<lem_f->state_string(this,year);
					cout.width(5) ; cout<<right<<"|"; cout.width(5); cout<<" ";
					cout<<left<<lem_f->observation->name;
				}
				else{
					////////////////////scrittura lem di annolegge 
					cout.width(20);cout<<internal<<lem_f->state_string(this,year);
					cout.width(5) ; cout<<right<<"|"; cout.width(5); cout<<" ";
					cout<<lem_f->observation->name<<" "<<lem_f->observation->year;
				}				
				
				++k;
			}
			if(k==1){
				cout.width(20);	cout<<internal<<lems_string(hypotesis);
				cout.width(5) ; cout<<right<<"|";
				//////////////////parte in cui scrivo se la legge è un'ipotesi (nessun lem
			}
			++i;
			cout<<endl;
			cout.width(8+20+5); cout<<right<<"|";cout.width(20+5); cout<<right<<"|";
			continue;
		}
		if(!!*exp[j] && (!*law[i] || exp[j]->year < law[i]->year)){
			if(year != exp[j]->year){
				year=exp[j]->year;
				cout.width(8+20+5); cout<<endl<<right<<"|"; cout.width(20+5); cout<<right<<"|";
				cout.width(7); cout<<endl<<right<<year<<":";
			}
			else{
				cout<<endl;
				cout.width(8);
				cout<<right<<" ";
			}
			k=1;
			while( !!*(lem_f=search_lem(exp[j],year,k)) ){
				if(lem_f->thesis->year == lem_f->observation->year) {
					if(k==1) {
						cout.width(20+5); cout<<right<<"|"; cout.width(20+5); cout<<right<<"|";
					}
					++k;
					continue;
				}
				if(k>1) {
					cout<<endl;
					cout.width(8);
					cout<<right<<" ";
				}
				cout.width(20);	cout<<right<<lem_f->thesis->name<<" "<<lem_f->thesis->year;  //20+5
				cout.width(20); cout<<internal<<lem_f->state_string(this,year);
				cout.width(5) ; cout<<right<<"|"; cout.width(5); cout<<" ";
				cout<<left<<exp[j]->name;
				////////////////////scrittura lem di annoesper (ev più di uno)
				++k;
			}		
			if(k==1){
				cout.width(20+5); cout<<right<<"|"; 
				cout.width(20); cout<<internal<<lems_string(research);
				cout.width(5) ; cout<<right<<"|"; cout.width(5); cout<<" ";
				cout<<left<<exp[j]->name;
				///////////////////parte per esperimento senza legge
			}
			++j;
			cout<<endl;
			cout.width(8+20+5); cout<<right<<"|"; cout.width(20+5); cout<<right<<"|";
		}
	}
	
}
			
	
/*	for(int y=ymin; y<=ymax; y++){		
		while(!!(*law[i])){
			if(law[i]->year > y) break;
			if(law[i]->year==y ){
				lem_f= search_lem(law[i],y);
				cout<<"\n Anno "<<y<<"\t";
				cout<<law[i]->inf_string(this,y)<<"  "<<law[i]->name<<"\t\t";
				if(!!* lem_f && !!*(lem_f->observation) ){
					cout<<lems_string(lem_f->curr_state);
					cout<<"\t\t"<<(lem_f->observation->name);
				}
				else cout<<lems_string(hypotesis);
				lem_f=no_lem;
			}
			i++;
		}
		while(!!(*exp[j])){
			if(exp[j]->year>y) break;
			if(exp[j]->year==y ){
				lem_f= search_lem(exp[j],y);
				if(!!* (lem_f) && !!*(lem_f->thesis) ) {
					if( lem_f->thesis->year ==y ) break;
					cout<<"\n anno "<<y<<"\t";
					cout<<lem_f->thesis->name<<"\t\t";
					cout<<lem_f->state_string(this,y);
				}
				else{ cout<<"\n anno "<<y<<"\t\t\t\t"<<lems_string(research);}
				cout<<"\t\t"<<exp[j]->name;
			}
			j++;
		}
	}*/
/*	for(int y=ymin; y<=ymax; y++){
		if(lem[i]->year==y ){
			cout<<endl<<"anno "<<y;
			while(!!(*lem[i])){
				if(lem[i]->year>y) break;
				if(lem[i]->year==y ){
					cout<<"\t"<<lem[i]->thesis->name<<" : "<<lem[i]->inf_state(y,this);
					cout<<"\t\t"<<lem[i]->lem_state(y,this)<<"  \t";
					cout<<lem[i]->observation->name<<endl<<endl;
				}
				i++;
			}
		}
	}
	*/


#endif
