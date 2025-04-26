#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool backtrack(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsAssigned,
    size_t day
); // Helper function for schedule()
bool assign(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsAssigned,
    size_t day,
    std::vector<Worker_T>& todaysWorkers,
    size_t assigned
); // Helper function for backtrack()


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    size_t numDays = avail.size();
    sched.resize(numDays); // Set schedule to have exactly numDays # of spots
    vector<int> shiftsAssigned(avail[0].size(), 0); // Vector to hold # of shifts each worker has been assigned to, initialized to 0
    return backtrack(avail, dailyNeed, maxShifts, sched, shiftsAssigned, 0); // Call helper functino backtrack() to fill schedule starting at day 0
}
// Helper function to fill schedule
bool backtrack(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsAssigned,
    size_t day
) {
    if(day == avail.size()) { // If scheduled all days successfully, return true
        return true; 
    }
    std::vector<Worker_T> todaysWorkers; // Vector holds workers assigned to hold workers scheduled to "today"
    return assign(avail, dailyNeed, maxShifts, sched, shiftsAssigned, day, todaysWorkers, 0); // Call helper function assign() to fill today's shifts
}

bool assign(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    DailySchedule& sched,
    std::vector<int>& shiftsAssigned,
    size_t day,
    std::vector<Worker_T>& todaysWorkers,
    size_t assigned
) { // Helper function recursively assigns workers from todaysWorkers to the current day
    if(assigned == dailyNeed) { // If number of workers assigned to current day equals dailyNeed
        sched[day] = todaysWorkers; //  Save workers assigned today to schedule
        return backtrack(avail, dailyNeed, maxShifts, sched, shiftsAssigned, day + 1); // Recursively call backtrack() on next day 
    }
    size_t numWorkers = avail[0].size(); // Get number of workers
    for(Worker_T w = 0; w < numWorkers; ++w) { // Loop through every worker to see if htey can be assigned
        if(avail[day][w] && shiftsAssigned[w] < maxShifts) { // If worker "w" is available today and still has shifts to work
            if(find(todaysWorkers.begin(), todaysWorkers.end(), w) == todaysWorkers.end()) { // If worker "w" hasn't been assigned today yet
                // Assign worker "w" to today's worker list and increment "w"'s shift count
                todaysWorkers.push_back(w);
                shiftsAssigned[w]++;
                // Recursively call assign() to try and assign next worker after "w" to next spot; if successful schedule achieved, return true
                if(assign(avail, dailyNeed, maxShifts, sched, shiftsAssigned, day, todaysWorkers, assigned + 1)) {
                    return true;
                }
                // Backtrack by removing worker "w" and move to next worker
                todaysWorkers.pop_back();
                shiftsAssigned[w]--;
            }
        }
    }
    return false; // Failed to assign worker successfully
}

