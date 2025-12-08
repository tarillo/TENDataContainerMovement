#include <iostream>
#include <vector>
#include <string>
#include "Manifest.h"
#include "Problem.h"
#include "Node.h"
#include "Tree.h"
#include "astar.h"
#include "log.h"

using namespace std;

int main() {
    Log log;                    // log must begin first when program starts

    while(true) {               // loops to be able to process more than 1 manifest if wanted
        string manifestFile;

        cout << "Enter a Manifest or type 'exit' to quit: ";
        getline(cin, manifestFile);

        // check input if it a true manifest or an exit
        if (manifestFile == "exit") {
            break;
        }

        Manifest manifest;

        auto start_time = chrono::steady_clock::now();

        manifest.loadManifest(manifestFile);    // validates file opening and loads data into containers
        manifest.buildGrid();

        // display number of containers loaded
        string shortManifestName = manifestFile.substr(0, manifestFile.find_last_of(".") + 1);
        cout <<  shortManifestName << " has " << manifest.getContainerCount() << " containers" << endl;
        cout << "Computing a solution..." << endl;

        // first log after starting program
        log.addLogEntry("Manifest " + manifestFile + " is opened, there are " + to_string(manifest.getContainerCount()) + " containers on the ship.");
        
        Problem problem(manifest.grid_to_vector());
        problem.manifest = &manifest;

        Tree tree(&problem);
        aStar::SearchStats stats;

        aStar solver;
        cout << "Starting A*..." << endl;
        Node* goal = solver.search(&tree, &problem, stats);
        auto stop_time = chrono::steady_clock::now();

        auto total_time = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time).count();
        
        if (goal == nullptr) {
            cout << "No solution found." << endl;
            log.addLogEntry("No balance solution found for manifest " + manifestFile + ".");
            return 0;
        }

        cout << "\nSolution has been found, it will take" << endl; cout << goal->cost << " move(s) \x1b[90m(not including from then to default crane location)\x1b[0m \n  " << goal->cost << " minutes \x1b[90m(not including from then to default crane location)\x1b[0m" << endl;

        vector<string> steps = solver.getSolutionPath(goal);

        cout << "   " << steps.size() << " move(s) \x1b[90m(not including from then to default crane location)\x1b[0m \n   " << goal->cost << " minutes \x1b[90m(not including from then to default crane location)\x1b[0m" << endl;
        // cout << "\n===== ASTAR SEARCH DATA =====\n";
        // cout << "Nodes Expanded: " << stats.nodesExpanded << endl;
        // cout << "Max Queue Size: " << stats.maxQueueSize << endl;
        // cout << "Solution Depth (Moves): " << stats.solutionDepthMoves << endl;
        // cout << "Solution Depth (Minutes): " << stats.solutionDepthMinutes << endl;
        // cout << "Total time(CPU): " << total_time << " milliseconds" << endl;
        //TANYA NOTE: need to add crane moves from parked to first move and last move to parked to add to log and output in steps.
        // add from parked cram to first move
        //steps.insert(steps.begin(), "Move crane from \x1b[32mpark\x1b[0m to \x1b[31m[" + to_string(step) + "," + to_string(goal->crane_col) + "]\x1b[0m");

        // adding solution to log
        log.addLogEntry("Balance solution found, it will require " + to_string(steps.size()) + " moves/" + to_string(goal->cost) + " minutes.");

        cout << "Hit ENTER when ready for first move" << endl;
        cin.ignore();

        cout << "\nSteps in solution path:" << endl;
        for (int i = 0; i < (int)steps.size(); ++i) {
            cout << "Step: " << i + 1 << " of " << steps.size() << ": " << steps[i] << endl;
            cout << "Hit ENTER when done / Hit 'i' to add a note \n\n" << endl;
            
            string noteOrContinue;
            getline(cin, noteOrContinue);

            // add option to add a note to log between moves if desired
            if (noteOrContinue == "i" || noteOrContinue == "I") {
                cout << "Enter your note: ";
                string note;
                getline(cin, note);
                log.writeNote(note);

                cout << "Note added to log." << endl;
                cout << "Hit ENTER when done \n\n" << endl;
                cin.ignore();
            }

            // removes color for log entry
            string logEntry = steps[i];
            size_t pos = 0;
            while ((pos = logEntry.find("\x1b[32m", pos)) != string::npos) {
                logEntry.erase(pos, 5);
            }
            pos = 0;
            while ((pos = logEntry.find("\x1b[31m", pos)) != string::npos) {
                logEntry.erase(pos, 5);
            }
            pos = 0;
            while ((pos = logEntry.find("\x1b[0m", pos)) != string::npos) {
                logEntry.erase(pos, 4);
            }

            //grab start coords and destination coords for log
            string startCoords = logEntry.substr(logEntry.find("[") , logEntry.find("]") - logEntry.find("[") + 1);;
            string endCoords = logEntry.substr(logEntry.rfind("[") , logEntry.rfind("]") - logEntry.rfind("[") + 1);;

            log.addLogEntry(startCoords + " was moved to " + endCoords);
        }

        // add log of solution completion
        log.addLogEntry("Finished a Cycle. Manifest " + manifestFile + " was written to desktop, and a reminder pop-up to operator to send file was displayed.");
    }

    //implicitly should call destructor to save log before program ends
    return 0;
}
