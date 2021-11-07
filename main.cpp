#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>

using namespace std;

void select_algorithm(string, int);
void search_algorithm(string, int, int);
int calculate_h(string, int, int);
vector<vector<int>> string_to_blocks(string, int);

int main(){
    cout << "Enter the puzzle's size (8, 15, 24, etc.): ";
    
    int size;
    cin >> size;

    while(ceil((double)sqrt(size + 1)) != floor((double)sqrt(size + 1))){
        cout << "Invalid Input! Enter a valid size (8, 15, 24, etc.): ";
        cin >> size;
    }

    int dim = (int)sqrt(size + 1);

    string puzzle = "";

    string input;
    for(int i = 0; i < dim; i++){
        cout << "Enter Row " << i + 1 << " (Use spaces in-between numbers)(Press enter when done): ";
        for(int j = 0; j < dim; j++){
            cin >> input;
            puzzle += input + " ";
        }
    }
    puzzle.resize(puzzle.length() - 1); // Delete the extra space at the end

    select_algorithm(puzzle, dim);

    return 0;
}

void select_algorithm(string puzzle, int dim){
    cout << "1) Uniform Cost Search" << endl;
    cout << "2) A* with Misplaced Tile Heuristic" << endl;
    cout << "3) A* with Manhattan Distance Heuristic" << endl;
    cout << "Select an algorithm (1, 2 or 3): ";

    int algorithm;
    cin >> algorithm;

    search_algorithm(puzzle, algorithm, dim);
}

void search_algorithm(string puzzle, int algorithm, int dim){
    auto compare = [&algorithm, &dim](string lhs, string rhs){
        return calculate_h(lhs, algorithm, dim) > calculate_h(rhs, algorithm, dim);
    };

    priority_queue<string, vector<string>, decltype(compare)> nodes(compare);
    nodes.push(puzzle);

    cout << calculate_h(puzzle, algorithm, dim) << endl;
}

int calculate_h(string puzzle, int algorithm, int dim){
    int h = 0;
    
    vector<vector<int>> blocks = string_to_blocks(puzzle, dim);

    int temp;
    switch(algorithm){
        case 1:
            h = 0;
            break;
        case 2:
            for(int i = 0; i < dim; i++){
                for(int j = 0; j < dim; j++){
                    temp = i * dim + j + 1;
                    if(blocks[i][j] != temp){
                        h++;
                    }
                }
            }
            break;
        case 3:
            for(int i = 0; i < dim; i++){
                for(int j = 0; j < dim; j++){
                    temp = i * dim + j + 1;
                    if(blocks[i][j] != 0 && blocks[i][j] != temp){
                        int og_row = (blocks[i][j] - 1) / dim;
                        int og_col = (blocks[i][j] - 1) % dim;
                        int row_displacement = abs(og_row - i);
                        int col_displacement = abs(og_col - j);
                        h += row_displacement + col_displacement;
                    }
                }
            }
            break;
    }

    return h;
}

vector<vector<int>> string_to_blocks(string puzzle, int dim){
    vector<vector<int>> blocks;

    vector<int> row;
    string block;
    for(int i = 0; i < puzzle.length(); i++){
        if(puzzle[i] == ' '){
            row.push_back(stoi(block));
            block.clear();
            if(row.size() == dim){
                blocks.push_back(row);
                row.clear();
            }
            continue;
        }
        block += puzzle[i];
    }
    row.push_back(stoi(block));
    blocks.push_back(row);

    return blocks;
}