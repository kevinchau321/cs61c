/*
 * CS61C Summer 2014
 * Name: Kevin Chau
 * Login: cs61c-gq
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "flights.h"
#include "timeHM.h"

 struct flightSys {
	airport_t* firstAirport;
 };

 struct airport {
	char* name;
	struct entry* firstEntry;
	airport_t* nextAirport;
 };

 struct entry {
	airport_t* dest;
	timeHM_t* departTime;
	timeHM_t* arriveTime;
	int cost;
	struct entry *nextEntry;
 };

/*
   This should be called if memory allocation failed.
 */
   static void allocation_failed() {
	fprintf(stderr, "Out of memory.\n");
	exit(EXIT_FAILURE);
   }


/*
   Creates and initializes a flight system, which stores the flight schedules of several airports.
   Returns a pointer to the system created.
 */
   flightSys_t* createSystem() {
	// Replace this line with your code
	flightSys_t* sys = malloc(sizeof(flightSys_t));
	if (!sys) {
		allocation_failed();
	}
	return sys;
   }


/*
   Frees all memory associated with this system; that's all memory you dynamically allocated in your code.
 */
   void deleteSystem(flightSys_t* s) {
	// Replace this line with your code
	free(s);
   }


/*
   Adds a airport with the given name to the system. You must copy the string and store it.
   Do not store "name" (the pointer) as the contents it point to may change.
 */
   void addAirport(flightSys_t* s, char* name) {
	// Replace this line with your code
	airport_t* newAir = malloc(sizeof(airport_t));
	newAir->name = malloc(sizeof(char));
	newAir->firstEntry=NULL;
	strcpy(newAir->name , name);
	if (s->firstAirport == NULL){
		s->firstAirport = newAir;
	} else {
		airport_t* lastAir = s->firstAirport;
		while (lastAir->nextAirport){
			lastAir = lastAir->nextAirport;
		}
		lastAir->nextAirport = newAir;
	}
   }

/*
   Returns a pointer to the airport with the given name.
   If the airport doesn't exist, return NULL.
 */
   airport_t* getAirport(flightSys_t* s, char* name) {
	// Replace this line with your code
	airport_t* currentAir = s->firstAirport;
	while (currentAir) {
		if ( !strcmp( currentAir->name , name) ){
			return currentAir;
		} else {
			currentAir = currentAir->nextAirport;	
		}
	}
	return NULL;
   }

/*
   Print each airport name in the order they were added through addAirport, one on each line.
   Make sure to end with a new line. You should compare your output with the correct output
   in flights.out to make sure your formatting is correct.
 */
   void printAirports(flightSys_t* s) {
	// Replace this line with your code
	airport_t* currentAir = s->firstAirport;
	while (currentAir){
		printf("%s\n", currentAir->name);
		currentAir = currentAir->nextAirport;
	}
   }

/*
   Adds a flight to src's schedule, stating a flight will leave to dst at departure time and arrive at arrival time.
 */
   void addFlight(airport_t* src, airport_t* dst, timeHM_t* departure, timeHM_t* arrival, int cost) {
	// Replace this line with your code
	struct entry* newSched = malloc(sizeof(struct entry));
	newSched->dest = malloc(sizeof(airport_t));
	newSched->dest = dst;
	newSched->departTime = malloc(sizeof(timeHM_t));
	newSched->departTime->hours = departure->hours;
	newSched->departTime->minutes = departure->minutes;
	newSched->arriveTime = malloc(sizeof(timeHM_t));
	newSched->arriveTime->hours = arrival->hours;
	newSched->arriveTime->minutes = arrival->minutes;
	newSched->nextEntry=NULL;
	newSched->cost = cost;
	if (src->firstEntry==NULL){
		src->firstEntry = newSched;
	} else {
		struct entry* lastEnt = src->firstEntry;
		while (lastEnt->nextEntry){
			lastEnt = lastEnt->nextEntry;
		}
		lastEnt->nextEntry = newSched;
	}
   }

/*
   Prints the schedule of flights of the given airport.

   Prints the airport name on the first line, then prints a schedule entry on each 
   line that follows, with the format: "destination_name departure_time arrival_time $cost_of_flight".

   You should use printTime (look in timeHM.h) to print times, and the order should be the same as 
   the order they were added in through addFlight. Make sure to end with a new line.
   You should compare your output with the correct output in flights.out to make sure your formatting is correct.
 */
   void printSchedule(airport_t* s) {
	// Replace this line with your code
	printf("%s\n", s->name);
	struct entry* currSched = s->firstEntry;
	while (currSched != NULL) {
		printf("%s ", currSched->dest->name);
		printTime(currSched->departTime);
		printf(" ");
		printTime(currSched->arriveTime);
		printf(" ");
		printf("$%d\n", currSched->cost);
		currSched = currSched->nextEntry; 
	}
   }


/*
   Given a src and dst airport, and the time now, finds the next flight to take based on the following rules:
   1) Finds the cheapest flight from src to dst that departs after now.
   2) If there are multiple cheapest flights, take the one that arrives the earliest.

   If a flight is found, you should store the flight's departure time, arrival time, and cost in departure, arrival, 
   and cost params and return true. Otherwise, return false. 

   Please use the function isAfter() from time.h when comparing two timeHM_t objects.
 */
   bool getNextFlight(airport_t* src, airport_t* dst, timeHM_t* now, timeHM_t* departure, timeHM_t* arrival, int* cost) {
	// Replace this line with your code
	struct entry* cheapestFlight = NULL;
	struct entry* schedEnt = src->firstEntry;     
	if ( schedEnt == NULL) {
		return false;
	}
	while (schedEnt != NULL) {
		if ( schedEnt->dest == dst && isAfter(schedEnt->departTime, now) ) {
			if (cheapestFlight == NULL){
				cheapestFlight = schedEnt;
			}
			if (schedEnt->cost < cheapestFlight->cost){
				cheapestFlight = schedEnt;
			}
			if (schedEnt->cost == cheapestFlight->cost){
				if (isAfter(cheapestFlight->arriveTime, schedEnt->arriveTime)){
					cheapestFlight = schedEnt;
				}
			}	
		}
		schedEnt = schedEnt->nextEntry;
	} 
	if (cheapestFlight == NULL) {
		return false;
	} else {
		*cost = cheapestFlight->cost;
		departure->hours = cheapestFlight->departTime->hours;
		departure->minutes= cheapestFlight->departTime->minutes;
		arrival->hours = cheapestFlight->arriveTime->hours;
		arrival->minutes = cheapestFlight->arriveTime->minutes;
		return true;
	}
   }

//int main (int argc, char* argv[]){
	//replace with test code
//	printf("starting tests\n");
//	flightSys_t* testSys = createSystem();
//	printf("created air system\n");
//	addAirport(testSys, "SJC");
//	printf("added first airport\n");
//	addAirport(testSys, "SFO");
//	addAirport(testSys, "SEA");
//	addAirport(testSys, "DFW");
//	addAirport(testSys, "SMF");
//	printf("added more airports\n");
//	printAirports(testSys);	
//	airport_t* getSFO = getAirport(testSys, "SFO");
//	airport_t* getSJC = getAirport(testSys, "SJC");
//	airport_t* getNone = getAirport(testSys, "booo");
//	if (getNone == NULL) {
//		printf("successfully did not find nonexistent airport, returned null\n");
//	} else {
//		printf("error!\n");
//	}
//	printf("Got this airport name: %s\n", getSFO->name);
//	printf("Got this airport name: %s\n", getSJC->name);
//	deleteSystem(testSys);
//}
