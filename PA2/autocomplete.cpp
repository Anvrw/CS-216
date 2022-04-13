/* File: autocomplete.h
 * Course: CS216-00x
 * Project: Lab9 (as part of Project 2)
 * Purpose: the implementation of member functions for the class named Autocomplete.
 *
 */
 


#include <iostream>
#include "autocomplete.h"



// default constructor
Autocomplete::Autocomplete()
{    
}


// inserts the newterm to the sequence
// note that a SortingList<Term> object, named terms
//          represents the sequence
void Autocomplete::insert(Term newterm)
{
    terms.insert(newterm);
	
}


// sort all the terms by query in the lexicographical order  
void Autocomplete::sort()
{

    terms.sort();
	
}

// binary search helper function
// as private function
// since it serves binary_search member function only
int Autocomplete::binary_searchHelper(SortingList<Term> terms, string key, int left_index, int right_index)
{   

    int middle = (left_index + right_index) / 2;
    Term keyTerm(key, 0);
     
    if (right_index < left_index){ //not found
        return -1;
    }
    else if(Term::compareByPrefix(keyTerm, terms[middle], key.length()) == 0){
        return middle; //did find the actor
    }
    else if (Term::compareByPrefix(keyTerm, terms[middle], key.length()) > 0){
        return binary_searchHelper(terms, key, left_index, middle - 1); //didn't find the actor
    }
    else {
        return binary_searchHelper(terms, key, middle + 1, right_index);
    }  

}


// return the index number of the term whose query
// prefix-matches the given prefix, using binary search algorithm
// Note that binary search can only be applied to sorted sequence
// Note that you may want a binary search helper function
int Autocomplete::binary_search(string prefix)
{
    return binary_searchHelper(terms, prefix, 0, terms.size()-1);
}

// first: the index of the first query whose prefix matches
//          the search key, or -1 if no such key
// last: the index of the last query whose prefix matches 
//         the search key, or -1 if no such key
// key: the given prefix to match
// hints: you can call binary_search() first to find one matched index number,
//        say hit, then look up and down from hit, to find first and last respectively
void Autocomplete::search(string key, int& firstIndex, int& lastIndex)
{
    int found_pos = binary_search(key);
    if (found_pos == -1)
    {
        firstIndex = -1;
        lastIndex = -1;
    }
    else
    {
        
        firstIndex = found_pos;
        lastIndex = found_pos;
        int r = key.length();
        
        // starting with found_pos
        // look forwards and backwards to find other possible prefix-matched terms
        int i = found_pos;

        // looking forwards
        while (i >= 0){
        i--;
            if (Term::compareByPrefix(terms[found_pos],terms[i], r) != 0){
                firstIndex = i+1;
                break;
            }
        }

        i = found_pos;
        // looking backwards;
        while (i <= terms.size()-1){
        i++;
            if (Term::compareByPrefix(terms[found_pos],terms[i], r) != 0){
                lastIndex = i-1;
                break;
            }
        }
    }
}

// returns all terms that start with the given prefix, in descending order of weight
SortingList<Term> Autocomplete::allMatches(string prefix)
{
    int first = -1;
    int last = -1;
    Term newSort(prefix, 0);
    SortingList<Term> matches;

    search(prefix, first, last);

    if (first != -1 && last != -1){
        for (int i = first; i != last + 1; i++){
            matches.insert(terms[i]);
        }
        matches.bubble_sort(newSort.compareByWeight);
    }
    return matches;

}

void Autocomplete::print()
{
    terms.print();
}
