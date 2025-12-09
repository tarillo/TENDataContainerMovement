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
        << 2000 << "' height='" 
        << 1400 << "'>\n";
    file << "<rect width='100%' height='100%' fill='white'/>\n";
    file << "<text x='" << 100
            << "' y='" << 100
            << "' dominant-baseline='middle' text-anchor='left' "
                "font-size='40'>"
            << "Enter a Manifest: "
            << "</text>\n";
    file << "</svg>";
}

void outputFirstScreen(const vector<vector<string>>& gridColors, vector<vector<Container>> grid, stringstream& statement, int rows, int cols, int cellSize) {
    std::ofstream file("grid.svg");
    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
            << 2000 << "' height='" 
            << 1400 << "'>\n";
        file << "<rect width='100%' height='100%' fill='white'/>\n";
        file << statement.str();
        file << "<rect x='" << 700
                    << "' y='" << 400 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << "#00ae00"
                    << "' stroke='black'/>\n";

        for (int r = 0; r < rows; r++) {
            file << "<text x='" << 650 
                << "' y='" << 1250 - r * cellSize
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='30'> "
                << std::setw(2) << std::setfill('0') << r + 1
                << "</text>\n";
            for (int c = 0; c < cols; c++) {
                if (r == 0) {
                    file << "<text x='" << 750 + c * cellSize 
                        << "' y='" << 1350
                        << "' dominant-baseline='middle' text-anchor='middle' "
                            "font-size='30'>"
                        << std::setw(2) << std::setfill('0') << c + 1
                        << "</text>\n";
                }
                int x = 700 + c * cellSize;
                int y = 1200 - r * cellSize;

                // Draw rectangle
                file << "<rect x='" << x 
                    << "' y='" << y 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << gridColors[r][c] 
                    << "' stroke='black'/>\n";
                if(grid[r][c].description != "UNUSED" && grid[r][c].description != "NAN") {
                // Draw text centered
                file << "<text x='" << x + cellSize/2 
                    << "' y='" << y + cellSize/2 
                    << "' dominant-baseline='middle' text-anchor='middle' "
                        "font-size='30'>"
                    << grid[r][c].description.substr(0,4)
                    << "</text>\n";
                }
            }
        }
        file << "</svg>";
}

void outputGridScreen(const vector<vector<string>>& gridColors, vector<vector<Container>> grid, stringstream& currStep, string startCoords, string& endCoords, int rows, int cols, int cellSize, int visualEndRow) {
    std::ofstream file("grid.svg");
    
    if(startCoords != "park" && endCoords != "park") {
        pair<int,int> source = {stoi(startCoords.substr(1, startCoords.find(",") - 1)), stoi(startCoords.substr(startCoords.find(",") + 1, startCoords.find("]") - startCoords.find(",") - 1))};
        pair<int,int> dest = {stoi(endCoords.substr(1, endCoords.find(",") - 1)), stoi(endCoords.substr(endCoords.find(",") + 1, endCoords.find("]") - endCoords.find(",") - 1))};
    
        currStep << "Move from <tspan fill ='#00ae00'>[" << std::setw(2) << std::setfill('0') << source.first << "," << std::setw(2) << std::setfill('0') << source.second << "]</tspan> to "
                    << "<tspan fill ='#ff0000'>[" << std::setw(2) << std::setfill('0') << dest.first << "," << std::setw(2) << std::setfill('0') << dest.second << "]</tspan></text>\n";
    }
    else if(startCoords == "park") {
        pair<int,int> dest = {stoi(endCoords.substr(1, endCoords.find(",") - 1)), stoi(endCoords.substr(endCoords.find(",") + 1, endCoords.find("]") - endCoords.find(",") - 1))};
    
        currStep << "Move from <tspan fill ='#00ae00'>" << startCoords << "</tspan> to "
                    << "<tspan fill ='#ff0000'>[" << std::setw(2) << std::setfill('0') << dest.first << "," << std::setw(2) << std::setfill('0') << dest.second << "]</tspan></text>\n";
    }
    else {
        pair<int,int> source = {stoi(startCoords.substr(1, startCoords.find(",") - 1)), stoi(startCoords.substr(startCoords.find(",") + 1, startCoords.find("]") - startCoords.find(",") - 1))};
    
        currStep << "Move from <tspan fill ='#00ae00'>[" << std::setw(2) << std::setfill('0') << source.first << "," << std::setw(2) << std::setfill('0') << source.second << "]</tspan> to "
                    << "<tspan fill ='#ff0000'>" << endCoords << "</tspan></text>\n";
    }

        file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
            << 2000 << "' height='" 
            << 1400 << "'>\n";
        file << "<rect width='100%' height='100%' fill='white'/>\n";
        file << currStep.str();
        if(startCoords == "park") {
            file << "<rect x='" << 700
                    << "' y='" << 400 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << "#00ae00"
                    << "' stroke='black'/>\n";
        }
        else if(endCoords == "park") {
            file << "<rect x='" << 700
                    << "' y='" << 400 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << "#ff0000"
                    << "' stroke='black'/>\n";
        }

        for (int r = 0; r < rows; r++) {
            file << "<text x='" << 650 
                << "' y='" << 1250 - r * cellSize
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='30'> "
                << std::setw(2) << std::setfill('0') << r + 1
                << "</text>\n";
            for (int c = 0; c < cols; c++) {
                if (r == 0) {
                    file << "<text x='" << 750 + c * cellSize 
                        << "' y='" << 1350
                        << "' dominant-baseline='middle' text-anchor='middle' "
                            "font-size='30'>"
                        << std::setw(2) << std::setfill('0') << c + 1
                        << "</text>\n";
                }
                int x = 700 + c * cellSize;
                int y = 1200 - r * cellSize;

                // Draw rectangle
                file << "<rect x='" << x 
                    << "' y='" << y 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << gridColors[r][c] 
                    << "' stroke='black'/>\n";
                if(grid[r][c].description != "UNUSED" && grid[r][c].description != "NAN") {
                // Draw text centered
                file << "<text x='" << x + cellSize/2 
                    << "' y='" << y + cellSize/2 
                    << "' dominant-baseline='middle' text-anchor='middle' "
                        "font-size='30'>"
                    << grid[r][c].description.substr(0,4)
                    << "</text>\n";
                }
            }
        }
        file << "</svg>";
}

void outputNewManifest(vector<vector<Container>> grid, const string& filename) {
    ofstream file(filename + "OUTBOUND.txt");

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            Container box = grid[i][j];
            file << "[" << std::setw(2) << std::setfill('0') << box.x + 1 << "," << std::setw(2) << std::setfill('0') << box.y + 1 << "], "
                << "{" << std::setw(5) << std::setfill('0') << box.weight << "}, "
                << box.description << "\n";
        }
    }
    file.close();

}



vector<vector<string>> vectorFormat(const vector<vector<Container>> grid, pair<int,int> source, pair<int,int> dest) {
    vector<vector<string>> formattedManifest;

    for (int i = 0; i < grid.size(); i++) {
        vector<string> formattedRow;
        for (int j = 0; j < grid[i].size(); j++) {
            int weightNum = grid[i][j].weight;
            if (weightNum == -1 || grid[i][j].isIllegal) {
                formattedRow.push_back("#000000");
            } else if (i+1 == source.first && j+1 == source.second) {
                formattedRow.push_back("#00ae00");
            } else if (i+1 == dest.first && j+1 == dest.second) {
                formattedRow.push_back("#ff0000");
            } else if (weightNum == 0 && grid[i][j].isEmpty) {
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
        if (manifestFile == "exit" || manifestFile == "quit") {
            break;
        }

        Manifest manifest;

        auto start_time = chrono::steady_clock::now();

        manifest.loadManifest(manifestFile);    // validates file opening and loads data into containers
        manifest.buildGrid();

        // display number of containers loaded
        string shortManifestName = manifestFile.substr(0, manifestFile.find_last_of("."));
        cout <<  shortManifestName << " has " << manifest.getContainerCount() << " containers" << endl;
        cout << "Computing a solution..." << endl;

        // first log after starting program
        log.addLogEntry("Manifest " + manifestFile + " is opened, there are " + to_string(manifest.getContainerCount()) + " containers on the ship.");
        
        Problem problem(manifest.get());
        problem.manifest = &manifest;

        Tree tree(&problem);
        aStar::SearchStats stats;

        Node* root = tree.root;
        pair<int,int> initialSource = {9, 1};
        string initialSourceStr = "park";
        pair<int,int> initialDest = {-1, -1};
        stringstream solveWait;
        solveWait << "<text x='" << 100
                    << "' y='" << 100
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>"; 
        solveWait << "Manifest " << manifestFile << " is opened, there are " << to_string(manifest.getContainerCount()) << " containers on the ship.</text>";
        solveWait << "<text x='" << 100
                    << "' y='" << 150
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>"; 
        solveWait << "Computing a solution...</text>";

        vector<vector<string>> gridColors = vectorFormat(root->state, initialSource, initialDest);
        outputFirstScreen(gridColors, root->state, solveWait, 8, 12, 100);


        aStar solver;
        cout << "Starting A*..." << endl;
        Node* goal = solver.search(&tree, &problem, stats);
        auto stop_time = chrono::steady_clock::now();


        auto total_time = chrono::duration_cast<chrono::milliseconds>(stop_time - start_time).count();
        
        // CASE handling if no solution found
        if (goal == nullptr) {
            cout << "No solution found." << endl;
            log.addLogEntry("No balance solution found for manifest " + manifestFile + ".");
            return 0;
        }

        cout << "\nSolution has been found, it will take" << endl;

        vector<Node*> steps = solver.getSolutionPath(goal);

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
        stringstream fullAction;
        Node* currNode;
        if(steps.size() != 0) {
            currNode = steps.at(0)->parent; // start from root
            stringstream introText;
            gridColors = vectorFormat(currNode->state, initialSource, initialDest);
            introText << "<text x='" << 100
                        << "' y='" << 100
                        << "' dominant-baseline='middle' text-anchor='left' "
                            "font-size='40'>"; 
            introText << "Solution has been found, it will take " << "</text>\n"; 
            introText << "<text x='" << 100
                        << "' y='" << 150
                        << "' dominant-baseline='middle' text-anchor='left' "
                            "font-size='40'>"; 
            introText   << steps.size() << " move(s) (not including from then to default crane location)  " << "</text>\n";
            introText << "<text x='" << 100
                        << "' y='" << 200
                        << "' dominant-baseline='middle' text-anchor='left' "
                            "font-size='40'>"; 
            introText << "   " << goal->cost << " minutes (not including from then to default crane location) " << "</text>\n";
            introText << "<text x='" << 100
                        << "' y='" << 250
                        << "' dominant-baseline='middle' text-anchor='left' "
                            "font-size='40'>"; 
            introText << "Hit ENTER when ready for first move" << "</text>\n";

            outputFirstScreen(gridColors, currNode->state, introText, 8, 12, 100);

            cin.ignore();

            cout << "\nSteps in solution path:" << endl;
        }
        for (int i = 0; i < (int)steps.size(); ++i) {
            if(i == 0) {
                //is out of order in code due to data being added in function but is in order in visualization
                fullAction << "<text x='" << 100
                    << "' y='" << 100 + (((steps.size()*2)+1)*40)
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
                fullAction << "Hit ENTER when done" << "</text>\n";
                fullAction << "<text x='" << 100
                    << "' y='" << 100 + (((steps.size()*2)+2)*40)
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
                fullAction << "Hit 'i' to add a note" << "</text>\n";

                fullAction << "<text x='" << 100 << "' y='" << 100
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='30'>";
                fullAction << "Step " << (i*2) + 1 << " of " << (steps.size()*2)+1 << ": ";
                // strip ANSI color codes before parsing coords
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

                string startCoords = logEntry.substr(logEntry.find("[") , logEntry.find("]") - logEntry.find("[") + 1);;
                pair<int,int> source = {stoi(startCoords.substr(1, startCoords.find(",") - 1)), stoi(startCoords.substr(startCoords.find(",") + 1, startCoords.find("]") - startCoords.find(",") - 1))};

                vector<vector<string>> gridColors = vectorFormat(currNode->state, initialSource, source);
                outputGridScreen(gridColors,currNode->state, fullAction, initialSourceStr, startCoords, 8, 12, 100, (i*2)+1);
                log.addLogEntry(initialSourceStr + " was moved to " + startCoords);

                string noteOrContinue;
                getline(cin, noteOrContinue);
                if (noteOrContinue == "i" || noteOrContinue == "I") {
                    cout << "Enter your note: ";
                    string note;
                    getline(cin, note);
                    log.writeNote(note);

                    cout << "Note added to log." << endl;
                    cout << "Hit ENTER when done \n\n" << endl;
                    cin.ignore();
                }
            }
            fullAction << "<text x='" << 100 << "' y='" << 100 + (((i*2)+1)*40)
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='30'>";
            fullAction << "Step " << (i*2) + 2 << " of " << (steps.size()*2)+1 << ": ";
            cout << "Step " << i + 1 << " of " << steps.size() << ": " << steps[i]->action << endl;
            cout << "Hit ENTER when done / Hit 'i' to add a note \n" << endl;

            // removes color for log entry
            string logEntry = steps[i]->action;
            size_t posStrip = 0;
            while ((posStrip = logEntry.find("\x1b[", posStrip)) != string::npos) {
                size_t mEnd = logEntry.find('m', posStrip);
                if (mEnd == string::npos) break;
                logEntry.erase(posStrip, mEnd - posStrip + 1);
            }
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

            pair<int,int> source = {stoi(startCoords.substr(1, startCoords.find(",") - 1)),
                                   stoi(startCoords.substr(startCoords.find(",") + 1, startCoords.find("]") - startCoords.find(",") - 1))};
            pair<int,int> dest = {stoi(endCoords.substr(1, endCoords.find(",") - 1)),
                                 stoi(endCoords.substr(endCoords.find(",") + 1, endCoords.find("]") - endCoords.find(",") - 1))};

            vector<vector<string>> gridColors = vectorFormat(currNode->state, source, dest);
            outputGridScreen(gridColors, currNode->state, fullAction, startCoords, endCoords, 8, 12, 100, (i*2)+2);
            log.addLogEntry(startCoords + " was moved to " + endCoords);
            
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
            currNode = steps[i];
            if(i == steps.size()-1) {
                fullAction << "<text x='" << 100 << "' y='" << 100 + (((i*2)+2)*40)
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='30'>";
                fullAction << "Step " << (i*2) + 3 << " of " << (steps.size()*2)+1 << ": ";
                string logEntry = currNode->action;
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

                string endCoords = logEntry.substr(logEntry.rfind("[") , logEntry.rfind("]") - logEntry.rfind("[") + 1);
                pair<int,int> dest = {stoi(endCoords.substr(1, endCoords.find(",") - 1)), stoi(endCoords.substr(endCoords.find(",") + 1, endCoords.find("]") - endCoords.find(",") - 1))};

                vector<vector<string>> gridColors = vectorFormat(currNode->state, dest, initialSource);
                outputGridScreen(gridColors, currNode->state, fullAction, endCoords, initialSourceStr, 8, 12, 100, (i*2)+3);
                log.addLogEntry(endCoords + " was moved to " + initialSourceStr);

                string noteOrContinue;
                getline(cin, noteOrContinue);
                if (noteOrContinue == "i" || noteOrContinue == "I") {
                    cout << "Enter your note: ";
                    string note;
                    getline(cin, note);
                    log.writeNote(note);

                    cout << "Note added to log." << endl;
                    cout << "Hit ENTER when done \n\n" << endl;
                    cin.ignore();
                }
            } else {
                fullAction << "<text x='" << 100 << "' y='" << 100 + (((i*2)+2)*40)
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='30'>";
                fullAction << "Step " << (i*2) + 3 << " of " << (steps.size()*2)+1 << ": ";
                string logEntry = steps[i+1]->action;
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

                string nextStartCoords = endCoords;
                string nextEndCoords = logEntry.substr(logEntry.find("[") , logEntry.find("]") - logEntry.find("[") + 1);
                pair<int,int> source = {stoi(nextStartCoords.substr(1, nextStartCoords.find(",") - 1)),
                                       stoi(nextStartCoords.substr(nextStartCoords.find(",") + 1, nextStartCoords.find("]") - nextStartCoords.find(",") - 1))};
                pair<int,int> dest = {stoi(nextEndCoords.substr(1, nextEndCoords.find(",") - 1)),
                                     stoi(nextEndCoords.substr(nextEndCoords.find(",") + 1, nextEndCoords.find("]") - nextEndCoords.find(",") - 1))};

                vector<vector<string>> gridColors = vectorFormat(currNode->state, source, dest);
                outputGridScreen(gridColors, currNode->state, fullAction, nextStartCoords, nextEndCoords, 8, 12, 100, (i*2)+3);
                log.addLogEntry(nextStartCoords + " was moved to " + nextEndCoords);

                string noteOrContinue;
                getline(cin, noteOrContinue);
                if (noteOrContinue == "i" || noteOrContinue == "I") {
                    cout << "Enter your note: ";
                    string note;
                    getline(cin, note);
                    log.writeNote(note);

                    cout << "Note added to log." << endl;
                    cout << "Hit ENTER when done \n\n" << endl;
                    cin.ignore();
                }
            }
        }

        // write updated manifest to file
        //manifest.writeManifestToFile(shortManifestName + "OUTBOUND.txt", goal->state, steps);
        // add log of solution completion
        stringstream endText;
        endText << "<text x='" << 100
                    << "' y='" << 100
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
        endText << "I have written an updated manifest file to the desktop named " << "</text>";
        endText << "<text x='" << 100
                    << "' y='" << 150
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
        endText << shortManifestName << "OUTBOUND.txt" << "</text>\n";
        endText << "<text x='" << 100
                    << "' y='" << 200
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
        endText << "Don't forget to email it to the captain." << "</text>\n";
        endText << "<text x='" << 100
                    << "' y='" << 250
                    << "' dominant-baseline='middle' text-anchor='left' "
                        "font-size='40'>";
        endText << "Hit ENTER when you are done" << "</text>\n";
        if(steps.size() != 0) {
            vector<vector<string>> endGridColors = vectorFormat(goal->state, initialSource, initialDest);
            outputFirstScreen(endGridColors, currNode->state, endText, 8, 12, 100);
            log.addLogEntry("Finished a Cycle. Manifest " + manifestFile + " was written to desktop, and a reminder pop-up to operator to send file was displayed.");
            outputNewManifest(goal->state, shortManifestName);
            
        }
        else {
            log.addLogEntry("Finished a Cycle. Manifest " + manifestFile + " was written to desktop, and a reminder pop-up to operator to send file was displayed.");
            outputNewManifest(root->state, shortManifestName);
        }

        string noteOrContinue;
        getline(cin, noteOrContinue);
        if (noteOrContinue == "i" || noteOrContinue == "I") {
            cout << "Enter your note: ";
            string note;
            getline(cin, note);
            log.writeNote(note);

            cout << "Note added to log." << endl;
            cout << "Hit ENTER when done \n\n" << endl;
            cin.ignore();
        }
    }

    //implicitly should call destructor to save log before program ends
    return 0;
}
