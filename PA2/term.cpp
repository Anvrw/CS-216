/* File: term.cpp
 * Course: CS216-00x
 * Project: Project 2
 * Purpose: the implementation of member functions for the Term class.
 *
 */
#include <iostream>
#include <algorithm>
#include "term.h"

// default constructor
Term::Term()
{
    weight = 0;
    query = "";
}

// initializes a term with the given query string and weight
Term::Term(string query, long weight)
{
    if (weight < 0)
    {    
        cout << "The weight should be a positive number";
        weight = 0;
    }
    else 
    {
        this->weight = weight;
        this->query = query;
    }    
}

// compares the two terms in descending order by weight
// if the order of T1 and T2 in descending order by weight, return 1;
// if T1 and T2 are of the same weight, return 0;
// otherwise return -1
int Term::compareByWeight(Term T1, Term T2){
    if (T1.weight > T2.weight){
        return 1;
    }
    else if (T1.weight == T2.weight){
        return 0;
    }
    else{
        return -1;
    }
}

// compares the two terms in lexicographic order but using only the first r characters of each query
int Term::compareByPrefix(Term T1, Term T2, int r){

    string string1 = T1.query;
    string string2 = T2.query;

    transform(string1.begin(), string1.end(), string1.begin(), ::tolower);
    transform(string2.begin(), string2.end(), string2.begin(), ::tolower);
    
    if (r < 0)
    {
        cout << "The length of the prefix should be a positive number!" << endl;
        return 2;
    }   
     
    if (string1.substr(0,r) < string2.substr(0,r)){
        return 1;
    }
    else if (string1.substr(0,r) == string2.substr(0,r)){
        return 0;
    }
    else {

        return -1;

    }
}

// operator overloading for operators "<" and "<<"
// they both are defined as friend functions

// define the operator "<" for Term class
// T1 < T2 if and only if the query of T1 is less than the query of T2 (based on string comparison)
bool operator<(Term T1, Term T2){
    string str1 = T1.query;
    string str2 = T2.query;

    transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
    transform(str2.begin(), str2.end(), str2.begin(), ::tolower);
    

    if (str1 < str2){
        return true;
    }
    else {
        return false;
    }
}

// define the operator "<<" for Term class
// to send the Term object to the cout, in the following format:
// the weight, followed by a tab key, followed by the query
ostream& operator<<(ostream &out, const Term& t){
    out << t.weight << "\t" << t.query << endl;
}
