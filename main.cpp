#include <fstream>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

struct Cell {
    std::string text;
    std::string color; // e.g., "#ffcccc" or "white"
};
void outputMainScreen() {
    std::ofstream file("grid.svg");

    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
        << 1400 << "' height='" 
        << 1200 << "'>\n";
    file << "<rect width='100%' height='100%' fill='white'/>\n";
    file << "<text x='" << 100
            << "' y='" << 100
            << "' dominant-baseline='middle' text-anchor='left' "
                "font-size='30'>"
            << "Enter a Manifest: "
            << "</text>\n";
    file << "</svg>";
}
void outputGridScreen(const vector<vector<Cell>>& grid, int rows, int cols, int cellSize) {
    std::ofstream file("grid.svg");
    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
            << 1400 << "' height='" 
            << 1200 << "'>\n";
        file << "<rect width='100%' height='100%' fill='white'/>\n";
        file << "<text x='" << 100
                << "' y='" << 100
                << "' dominant-baseline='middle' text-anchor='left' "
                    "font-size='20'>"
                << "Move piece from here to here"
                << "</text>\n";
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int x = 100 + c * cellSize;
                int y = 300 + r * cellSize;

                // Draw rectangle
                file << "<rect x='" << x 
                    << "' y='" << y 
                    << "' width='" << cellSize 
                    << "' height='" << cellSize 
                    << "' fill='" << grid[r%3][c%3].color 
                    << "' stroke='black'/>\n";

                // Draw text centered
                file << "<text x='" << x + cellSize/2 
                    << "' y='" << y + cellSize/2 
                    << "' dominant-baseline='middle' text-anchor='middle' "
                        "font-size='20'>"
                    << grid[r%3][c%3].text
                    << "</text>\n";
            }
        }
        file << "</svg>";
}
//pass in the manifest
int main() {
    vector<vector<string>> manifest = {
        {("A"), "B", "C"},
        {"D", "E", "F"},
        {"G", "H", "I"},
    };
    std::vector<std::vector<Cell>> grid = {
        {{"A","#ffdddd"}, {"A","white"}, {"A","#ddffdd"}},
        {{"A","white"}, {"A","#ddddff"}, {"A","white"}},
        {{"A","white"}, {"A","white"}, {"A","#ffffcc"}},
    };
    std::vector<std::vector<string>> grid2 = {
        {{"#ffdddd"}, {"white"}, {"#ddffdd"}},
        {{"#ffdddd"}, {"#ddddff"}, {"white"}},
        {{"#ffdddd"}, {"white"}, {"#ffffcc"}},
    };

    int rows = 8;
    int cols = 12;
    int cellSize = 100;
    std::string pause;
    vector<vector<string>> manifestColors;

    outputMainScreen();
    cin.get();
    outputGridScreen(grid, rows, cols, cellSize);

    // while(cin.get())  {
    //     if(manifestColors == grid) {
    //         manifestColors = grid2;
    //     } else {
    //         manifestColors = grid;
    //     }
    //     std::ofstream file("grid.svg");


    //     file << "<svg xmlns='http://www.w3.org/2000/svg' width='" 
    //         << 1400 << "' height='" 
    //         << 1200 << "'>\n";
    //     file << "<rect width='100%' height='100%' fill='white'/>\n";
    //     file << "<text x='" << 100
    //             << "' y='" << 100
    //             << "' dominant-baseline='middle' text-anchor='left' "
    //                 "font-size='20'>"
    //             << "Move piece from here to here"
    //             << "</text>\n";
    //     for (int r = 0; r < rows; r++) {
    //         for (int c = 0; c < cols; c++) {
    //             int x = 100 + c * cellSize;
    //             int y = 300 + r * cellSize;

    //             // Draw rectangle
    //             file << "<rect x='" << x 
    //                 << "' y='" << y 
    //                 << "' width='" << cellSize 
    //                 << "' height='" << cellSize 
    //                 << "' fill='" << manifestColors[r%3][c%3] 
    //                 << "' stroke='black'/>\n";

    //             // Draw text centered
    //             file << "<text x='" << x + cellSize/2 
    //                 << "' y='" << y + cellSize/2 
    //                 << "' dominant-baseline='middle' text-anchor='middle' "
    //                     "font-size='20'>"
    //                 << manifest[r%3][c%3] 
    //                 << "</text>\n";
    //         }
    //     }

    //     file << "</svg>";
    //     file.close();
    // }
}
