/*
 * Course: CS216-00x
 * Project: Lab5 (as part of Project 1)
 * Purpose: it reads data from an input file which passes as command line argument
 *          then stores in an object of IMDB class (to represent imdb database)
 *          it then allowes user to search for all actors in either one of two movies
 *          two movies are from the user-input, then performs case insensitive matching
 *          and match the movie titles from the database
 * Author: (Andrew Tackett)
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <set>
#include <map>
#include <sstream>
#include "imdb.h"

using namespace std;

void analyzeMovies(const IMDB& imdb);
void analyzeCoActors(const IMDB& imdb);

int main(int argc, char* argv[])
{
    // Check whether the number of command line arguments is exactly one 
    if (argc != 2)
    {
        cout << "Warning: need exactly one command line argument." << endl;
        cout << "Usage: " << argv[0] << " inputfile_name" << endl;
        return 1;
    }

    ifstream in_file;
    in_file.open(argv[1]);

    // Check whether the input file can be open successfully or not
    if (!in_file.good())
    {
        cout << "Warning: cannot open file named " << argv[1] << "!" << endl;
        return 2;
    }

    // Read data from the input file, assume "good data" from the input file
    // each line of the input file: actor name, movie1, movie2, ..., movien
    // read one line at a time then tokenize it:
    // create a pair <actor_name, the set of movieTitles which the actor was in>
    //        and insert to IMDB object
    IMDB cs216_imdb;

    while (!in_file.eof())
    {
        string line;
        getline(in_file, line);
        string actorName, movieTitle;
        // check if the text line is not empty then tokenize it
        if (line.length() != 0)
        {
            istringstream iss(line);
            getline(iss, actorName,',');

            // Extract extra white space
            iss>>ws;

            // Create a new set of movies associated with name
            set<string> movies;

            while (getline(iss, movieTitle, ','))
            {
                movies.insert(movieTitle);
                // extract white space 
                iss>>ws;
            }
            // insert the pair to IMDB object: cs216_imdb
            cs216_imdb.insert_an_actor(actorName, movies);
        }
    }
    // close the input file
    in_file.close();

    int option1;
    while (true)
    {
        //This is the first menu that gives the option of Actors and Co-Actors.
        cout << "This application stores information about Actors and their Movies, please choose your option (Enter Q or q to quit):" << endl;
        cout << "1. Actors in Movies" << endl;
        cout << "2. Actors and co-actors" << endl;
        cin >> option1;
        cin.ignore(256, '\n');

        if (cin.fail())
        {
            //It will check the option to see if it is a "1", "2", or "Quit"
            string input_to_check;
            cin.clear();
            cin >> input_to_check;
            if (input_to_check == "Q" || input_to_check == "q")
                break;
            else
            {
                cout << "Invalid option!" << endl;
                continue;
            }
        }

        switch (option1)
        {
            //Calls either functions based on what input is made by the user
            case 1: analyzeMovies(cs216_imdb);
                    break;
            case 2: analyzeCoActors(cs216_imdb);
                    break;
            default:
                    cout << "Invalid option!" << endl;
        }
    }
    cout << "Thank you for using my program, bye..." << endl;
    return 0;
}



// this function asks the user to input two movie titles
// then finds the matched two movies from the database
// and displays all the actors in either of the two movies
// if no matched movie then display "Invalid movie title" and do nothing
// note that using const call by reference is for efficiency purpose:
//           no copy of IMDB object is made and the argument has been protected
void analyzeMovies(const IMDB& imdb)
{
    // Ask the user to input two movie titles to search for
    string movie1, movie2;
    cout << "Please input the first movie title: ";
    getline(cin,movie1);
    cout << "Please input the second movie title: ";
    getline(cin,movie2);
    // Check if both movie1 and movies can be matched with movies in the database
    // If either one is not, quit the program
    // Otherwise display all the actors in either of two movies
    //                   which are actually the union of two sets
    string matchedMovie1, matchedMovie2;
    matchedMovie1 = imdb.matchExistingMovie(movie1);
    matchedMovie2 = imdb.matchExistingMovie(movie2);
    string option2;
    set<string> all;
    if ((matchedMovie1.length() > 0) && (matchedMovie2.length() > 0))
    {
        //Checks to see if the two entered movies are in the database
        cout << "Your input matches the following two movies: " <<endl;
        cout << matchedMovie1 << endl;
        cout << matchedMovie2 << endl;
        cout << "Both movies are in the database, please continue..." << endl;

            //Prints the submenu, which gives the 3 options, these activate the while loop
             cout << "Please input your menu option (Enter Q or q to quit)" << endl;
             cout << "A -- to print all the actors in either of the two movies." << endl;
             cout << "C -- to print all the common actors in both of the movies." << endl;
             cout << "O -- to print all the actors who are in one movie, but not in both." << endl;
             cin >> option2;
        
         //The while loop will be active until the Quit (Q  or q) is made 
         while(option2 != "Q" && option2 != "q"){
             if(option2 == "A" || option2 == "a"){
              // to store the union of actors_in_movie1 and actors_in_movie2
             all = imdb.find_actors_in_a_movie(matchedMovie1);
             set<string> actors_in_movie2 = imdb.find_actors_in_a_movie(matchedMovie2);
                for (auto i = actors_in_movie2.begin(); i != actors_in_movie2.end(); i++)
                all.insert(*i);
                
                cout << "All the actors in either of the two movies:" << endl;
                for (auto i = all.begin(); i != all.end(); i++){
                cout << *i << endl;
                }

                
             }
             else if(option2 == "C" || option2 == "c"){
                 // to store the intersection of actors_in_movie1 and actors_in_movie2
                 set<string> common;
                 set<string> actors_in_movie1 = imdb.find_actors_in_a_movie(matchedMovie1);
                 set<string> actors_in_movie2 = imdb.find_actors_in_a_movie(matchedMovie2);
                 
                 for (auto i = actors_in_movie1.begin(); i != actors_in_movie1.end(); i++){
                     // Will use .count to search for the actors that are the same in both movies. It will use .insert to fill the
                     // set<string> common with the common actors in both movies.
                     if (actors_in_movie2.count(*i) == 1){
                         common.insert(*i);
                     }
                 }
                 for (auto i = actors_in_movie2.begin(); i != actors_in_movie2.end(); i++){
                     if (actors_in_movie1.count(*i) == 1){
                         common.insert(*i);
                     }
                 }

                 cout << "Common actors in both movies: " << endl;
                 // Prints the common actors in both movies.
                 for (auto i = common.begin(); i != common.end(); i++){
                     cout << *i << endl;
                 }
                 
                
             }
             else if (option2 == "O" || option2 == "o"){
                 // to store the symmetric_difference of actors_in_a_movie1 and actors_in_a_movie2.
                 set<string> oneMovie;
                 set<string> actors_in_movie1 = imdb.find_actors_in_a_movie(matchedMovie1);
                 set<string> actors_in_movie2 = imdb.find_actors_in_a_movie(matchedMovie2);

                    // Will use .count to search for the actors that are the same in both movies. It will use .insert to fill the
                    // set<string> common with the common actors in both movies.
                     for (auto i = actors_in_movie1.begin(); i != actors_in_movie1.end(); i++){
                        if (actors_in_movie2.count(*i) == 0){
                             oneMovie.insert(*i);
                        }
                    }
                 
                    for (auto i = actors_in_movie2.begin(); i != actors_in_movie2.end(); i++){
                        if (actors_in_movie1.count(*i) == 0){
                            oneMovie.insert(*i);
                        }
                    }
                 
                    cout << "Actors only in one of two movies: " << endl;
                    // Prints the actors that are only in one movie.
                    for (auto i = oneMovie.begin(); i != oneMovie.end(); i++){
                        cout << *i << endl;
                    }
             }
             else {
                    cout << "Invalid option." << endl;
                }
                // Prints menu at the end of the while loop.
                cout << "Please input your menu option (Enter Q or q to quit)" << endl;
                cout << "A -- to print all the actors in either of the two movies." << endl;
                cout << "C -- to print all the common actors in both of the movies." << endl;
                cout << "O -- to print all the actors who are in one movie, but not in both." << endl;
                cin >> option2;
        }
    }
    else
    {
        //Prints this is the movie title isn't in the database.
        cout << "Invalid movie title." << endl;
        return;
    }
}

// The function asks the user to type an actor's name
// then check if the user-input name is valid(it is valid if it is in the database stored in the passing in object
// if it is valid, display in each movie which this actor is in, the co-actors who appeared in the same movie
void analyzeCoActors(const IMDB& imdb)
{
    //This is the second option that is given from the menu, it is for co-actors.
    string actor_name;
    cout << "Finding the co-actors of the actor by typing his/her name: ";
    getline(cin, actor_name);
    if (!imdb.isExistingActor(actor_name))
    {
        cout << "The actor name you entered is not in the database." << endl;
        return;
    }   

    set<string> movies_of_actor;
    movies_of_actor = imdb.find_movies_for_an_actor(actor_name);
    for (auto i = movies_of_actor.begin(); i != movies_of_actor.end(); i++)
    {
        cout << "The co-actors of " << actor_name << " in the movie \"" << *i << "\" are: " << endl;
        // Display all the co-actors in one movie
        set<string> coactors = imdb.find_actors_in_a_movie(*i);
        for (auto j = coactors.begin(); j != coactors.end(); j++)
        {
            if(*j != actor_name){
                cout << *j << endl;
            }
        }    
        cout << "***********************************" << endl;
    }
}    


