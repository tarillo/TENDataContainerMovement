#include <iostream>
#include <vector>
#include <string>
#include "Manifest.h"
#include "Problem.h"
#include "Node.h"
#include "Tree.h"
#include "astar.h"
#include "log.h"


void outputMainScreen() {
    std::ofstream file("grid.svg");

    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
        << 1400 << "' height='" 
        << 1200 << "'>\n";
    file << "<rect width='100%' height='100%' fill='white'/>\n";
    file << "<text x='" << 100
            << "' y='" << 100
            << "' dominant-baseline='middle' text-anchor='left' "
                "font-size='40'>"
            << "Enter a Manifest: "
            << "</text>\n";
    file << "</svg>";
}

void outputGridScreen(const vector<vector<string>>& gridColors, Manifest& manifest, Node* currStep, pair<int,int> source, pair<int,int> dest, int rows, int cols, int cellSize) {
    std::ofstream file("grid.svg");
    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
            << 1400 << "' height='" 
            << 1200 << "'>\n";
        file << "<rect width='100%' height='100%' fill='white'/>\n";
        file << "<text x='" << 100
                << "' y='" << 100
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='30'>"
                << "Move from "
                << "<tspan fill ='green'>[" << source.first << "," << source.second << "]</tspan> to "
                << "<tspan fill ='red'>[" << dest.first << "," << dest.second << "]</tspan>"
                << "</text>\n";
        for (int r = 0; r < rows; r++) {
            file << "<text x='" << 150 
                << "' y='" << 1050 - r * cellSize
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='30'>"
                << r
                << "</text>\n";
            for (int c = 0; c < cols; c++) {
                if (r == 0) {
                    file << "<text x='" << 250 + c * cellSize 
                        << "' y='" << 1100
                        << "' dominant-baseline='middle' text-anchor='middle' "
                            "font-size='30'>"
                        << c
                        << "</text>\n";
                }
                int x = 200 + c * cellSize;
                int y = 1000 - r * cellSize;

                // Draw rectangle
                file << "<rect x='" << x 
                    << "' y='" << y 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << gridColors[r][c] 
                    << "' stroke='black'/>\n";
                if(manifest.getCurrContainer(r*cols+c).description != "UNUSED" && manifest.getCurrContainer(r*cols+c).description != "NAN") {
                // Draw text centered
                file << "<text x='" << x + cellSize/2 
                    << "' y='" << y + cellSize/2 
                    << "' dominant-baseline='middle' text-anchor='middle' "
                        "font-size='20'>"
                    << manifest.getCurrContainer(r*cols+c).description.substr(0,4)
                    << "</text>\n";
                }
            }
        }
        file << "</svg>";
}

vector<vector<string>> vectorFormat(const vector<vector<int>> grid, Manifest& manifest, pair<int,int> source, pair<int,int> dest, Node* currStep) {
    vector<vector<string>> formattedManifest;
    for (int i = 0; i < grid.size(); i++) {
        vector<string> formattedRow;
        for (int j = 0; j < grid[i].size(); j++) {
            int weightNum = grid[i][j];
            if (weightNum == -1) {
                formattedRow.push_back("#000000");
            } else if (i == source.first && j == source.second) {
                formattedRow.push_back("#00ff00");
            } else if (i == dest.first && j == dest.second) {
                formattedRow.push_back("#ff0000");
            } else if (weightNum == 0 && manifest.grid[i][j]->isEmpty) {
                formattedRow.push_back("#ffffff");
            }
            else {
                formattedRow.push_back("#b0b060");
            }
        }
        formattedManifest.push_back(formattedRow);
    }
    return formattedManifest;
}

/*

*/
using namespace std;

int main() {
    Log log;                    // log must begin first when program starts

    while(true) {               // loops to be able to process more than 1 manifest if wanted
        string manifestFile;

        outputMainScreen();
        cout << "Enter a Manifest or type 'exit' to quit: ";
        getline(cin, manifestFile);

        // check input if it a true manifest or an exit
        if (manifestFile == "exit") {
            break;
        }

        Manifest manifest;

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

        aStar solver;
        cout << "Starting A*..." << endl;
        Node* goal = solver.search(&tree, &problem);

        if (goal == nullptr) {
            cout << "No solution found." << endl;
            log.addLogEntry("No balance solution found for manifest " + manifestFile + ".");
            return 0;
        }

        cout << "\nSolution has been found, it will take" << endl; cout << goal->cost << " move(s) \x1b[90m(not including from then to default crane location)\x1b[0m \n  " << goal->cost << " minutes \x1b[90m(not including from then to default crane location)\x1b[0m" << endl;

        vector<Node*> steps = solver.getSolutionPath(goal);

        cout << "   " << steps.size() << " move(s) \x1b[90m(not including from then to default crane location)\x1b[0m \n   " << goal->cost << " minutes \x1b[90m(not including from then to default crane location)\x1b[0m" << endl;

        //TANYA NOTE: need to add crane moves from parked to first move and last move to parked to add to log and output in steps.
        // add from parked cram to first move
        //steps.insert(steps.begin(), "Move crane from \x1b[32mpark\x1b[0m to \x1b[31m[" + to_string(step) + "," + to_string(goal->crane_col) + "]\x1b[0m");

        // adding solution to log
        log.addLogEntry("Balance solution found, it will require " + to_string(steps.size()) + " moves/" + to_string(goal->cost) + " minutes.");

        cout << "Hit ENTER when ready for first move" << endl;
        cin.ignore();

        cout << "\nSteps in solution path:" << endl;
        Node* currNode = steps.at(0)->parent; // start from root
        for (int i = 0; i < (int)steps.size(); ++i) {
            cout << "Step: " << i + 1 << " of " << steps.size() << ": " << steps[i]->action << endl;
            cout << "Hit ENTER when done / Hit 'i' to add a note \n\n" << endl;

            // removes color for log entry
            string logEntry = steps[i]->action;
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

            pair<int,int> source = {stoi(startCoords.substr(1, startCoords.find(",") - 1)), stoi(startCoords.substr(startCoords.find(",") + 1, startCoords.find("]") - startCoords.find(",") - 1))};
            pair<int,int> dest = {stoi(endCoords.substr(1, endCoords.find(",") - 1)), stoi(endCoords.substr(endCoords.find(",") + 1, endCoords.find("]") - endCoords.find(",") - 1))};

            cout << source.first << "," << source.second << " to " << dest.first << "," << dest.second << endl;
            vector<vector<string>> gridColors = vectorFormat(currNode->state, manifest, source, dest, steps[i]);
            outputGridScreen(gridColors, manifest, steps[i], source, dest, 8, 12, 80);
            
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
            // string logEntry = steps[i]->action;
            // size_t pos = 0;
            // while ((pos = logEntry.find("\x1b[32m", pos)) != string::npos) {
            //     logEntry.erase(pos, 5);
            // }
            // pos = 0;
            // while ((pos = logEntry.find("\x1b[31m", pos)) != string::npos) {
            //     logEntry.erase(pos, 5);
            // }
            // pos = 0;
            // while ((pos = logEntry.find("\x1b[0m", pos)) != string::npos) {
            //     logEntry.erase(pos, 4);
            // }

            // //grab start coords and destination coords for log
            // string startCoords = logEntry.substr(logEntry.find("[") , logEntry.find("]") - logEntry.find("[") + 1);;
            // string endCoords = logEntry.substr(logEntry.rfind("[") , logEntry.rfind("]") - logEntry.rfind("[") + 1);;

            log.addLogEntry(startCoords + " was moved to " + endCoords);
            currNode = steps[i];
        }

        // add log of solution completion
        log.addLogEntry("Finished a Cycle. Manifest " + manifestFile + " was written to desktop, and a reminder pop-up to operator to send file was displayed.");
    }

    //implicitly should call destructor to save log before program ends
    return 0;
}
