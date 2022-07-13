#ifndef BASE_H  
#define BASE_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#define N_inf 10
#define N_lem 30
#define N_th 10
#define N_str 10
#define N_math 10
#define N_law 30
#define N_exp 10

#define Y_BEG 1500
#define TODAY 2020
#define AUTO_ADD_LEMTO 'y'



class Math;
class Law;
class Exp; 
class LEM;
class Theory;

enum INF_S { ERROR=0, axiom=1, theorem=2 };

enum LEM_S{	research , hypotesis,	cons_exp, 	fals_exp, 		  cons_law,      fals_law};
								//	verifica,	falsificazione,	  spiegazione,   leggefalsa
enum LEM_V{ fals, cons, undef}; 


string lems_string(LEM_S s){
	if(s==research ) return " osservazione ";
	if(s==hypotesis) return " ipotesi ";
	if(s==cons_exp ) return " validita' ";
	if(s==fals_exp ) return " falsificazione ";
	if(s==cons_law ) return " spiegazione ";
	if(s==fals_law ) return " legge falsificata da ";
}

string lemv_string(LEM_V v){
	if(v==cons) return " consistente con ";
	if(v==fals) return " falsificata da ";
	if(v==undef) return " unclear ";
}

string infs_string(INF_S is){
	if (is==ERROR) return " errore ";
	if (is==axiom) return " assioma ";
	if (is==theorem) return " teorema ";
}


LEM_V val(bool v){if (v) return cons; return fals;}


#endif
