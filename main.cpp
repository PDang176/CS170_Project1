// https://github.com/PDang176/CS170_Project1

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include <iterator>
#include <unordered_set>

using namespace std;

string select_default_puzzle();
void select_algorithm(string, int);
void search_algorithm(string, int, int);
int calculate_f(string, int, int);
int calculate_h(string, int, int);
vector<vector<int>> string_to_blocks(string, int);
bool check_success(string, int);
vector<string> generate_nodes(string, int);
void print_puzzle(string, int);

const vector<string> default_puzzles{
    "0,1 2 3 4 5 6 7 8 0",
    "0,1 2 3 4 5 6 0 7 8",
    "0,1 3 6 5 0 2 4 7 8",
    "0,1 3 6 5 0 7 4 8 2",
    "0,1 6 7 5 0 3 4 8 2",
    "0,7 1 2 4 8 5 6 3 0",
    "0,0 7 2 4 6 1 3 5 8"
};

int main(){
    int mode;
    int dim;
    string puzzle;

    cout << "Enter '1' to use a default puzzle. Enter '2' to create your own puzzle (Any other selection chooses '1'): ";
    cin >> mode;
    cout << endl;

    if(mode == 1){
        dim = 3;
        puzzle = select_default_puzzle();
    }
    else if(mode == 2){
        cout << "Enter the puzzle's size (8, 15, 24, etc.): ";
        
        int size;
        cin >> size;

        while(ceil((double)sqrt(size + 1)) != floor((double)sqrt(size + 1))){
            cout << "Invalid Input! Enter a valid size (8, 15, 24, etc.): ";
            cin >> size;
        }

        dim = (int)sqrt(size + 1);

        puzzle = "0,"; // Starts with the g cost

        string input;
        for(int i = 0; i < dim; i++){
            cout << "Enter Row " << i + 1 << " (Use spaces in-between numbers)(Press enter when done): ";
            for(int j = 0; j < dim; j++){
                cin >> input;
                puzzle += input + " ";
            }
        }
        puzzle.pop_back(); // Delete the extra space at the end

    }

    select_algorithm(puzzle, dim);

    return 0;
}

string select_default_puzzle(){
    
    
void select_algorithm(string puzzle, int dim){
    cout << "1) Uniform Cost Search" << endl;
    cout << "2) A* with Misplaced Tile Heuristic" << endl;
    cout << "3) A* with Manhattan Distance Heuristic" << endl;
    cout << "Select an algorithm (1, 2 or 3): ";

    int algorithm;
    cin >> algorithm;

    cout << endl;

    search_algorithm(puzzle, algorithm, dim);
}

void search_algorithm(string puzzle, int algorithm, int dim){
    auto compare = [&algorithm, &dim](string lhs, string rhs){
        return calculate_f(lhs, algorithm, dim) > calculate_f(rhs, algorithm, dim);
    };

    unordered_set<string> seen;

    priority_queue<string, vector<string>, decltype(compare)> nodes(compare);
    nodes.push(puzzle);

    string node;

    while(1){
        if(nodes.empty()){
            cout << "FAILURE" << endl;
            break;
        }
        node = nodes.top();
        nodes.pop();

        print_puzzle(node, dim);

        if(check_success(node, dim)){
            cout << "SUCCESS" << endl;
            break;
        }

        int g_end = node.find(',');

        seen.insert(node.substr(g_end + 1));

        vector<string> new_nodes = generate_nodes(node, dim);

        string new_node;
        for(int i = 0; i < new_nodes.size(); i++){
            if(seen.count(new_nodes[i]) == 0){
                int g = stoi(node.substr(0, g_end)) + 1;
                new_node += to_string(g) + ',' + new_nodes[i];
                nodes.push(new_node);
            }
            new_node.clear();
        }
    }
}

int calculate_f(string puzzle, int algorithm, int dim){
    int g_end = puzzle.find(',');
    
    int g = stoi(puzzle.substr(0, g_end));
    int h = calculate_h(puzzle, algorithm, dim);

    int f = g + h;

    return f;
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

    int start = puzzle.find(',') + 1; // Skip over the first number indicating g

    vector<int> row;
    string block;
    for(int i = start; i < puzzle.length(); i++){
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

string blocks_to_string(vector<vector<int>> const & blocks, int dim){
    string puzzle = "";

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            puzzle += to_string(blocks[i][j]) + ' ';
        }
    }
    puzzle.pop_back();

    return puzzle;
}

bool check_success(string puzzle, int dim){
    int start = puzzle.find(',') + 1; // Skip over the first number indicating g
    
    string puzzle_check = puzzle.substr(start);

    string success = "";
    for(int i = 0; i < (dim * dim) - 1; i++){
        success += to_string(i + 1) + " ";
    }
    success += '0';

    return (puzzle_check == success);
}

vector<string> generate_nodes(string puzzle, int dim){
    vector<string> nodes;

    vector<vector<int>> blocks = string_to_blocks(puzzle, dim);

    int blank_row;
    int blank_col;

    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            if(blocks[i][j] == 0){
                blank_row = i;
                blank_col = j;
                break;
            }
        }
    }

    vector<vector<int>> temp(blocks);
    
    if(blank_row > 0){ // Up
        temp[blank_row][blank_col] = temp[blank_row - 1][blank_col];
        temp[blank_row - 1][blank_col] = 0;
        nodes.push_back(blocks_to_string(temp, dim));
        temp = blocks;
    }

    if(blank_row < dim - 1){ // Down
        temp[blank_row][blank_col] = temp[blank_row + 1][blank_col];
        temp[blank_row + 1][blank_col] = 0;
        nodes.push_back(blocks_to_string(temp, dim));
        temp = blocks;
    }

    if(blank_col > 0){ // Left
        temp[blank_row][blank_col] = temp[blank_row][blank_col - 1];
        temp[blank_row][blank_col - 1] = 0;
        nodes.push_back(blocks_to_string(temp, dim));
        temp = blocks;
    }

    if(blank_col < dim - 1){ // Right
        temp[blank_row][blank_col] = temp[blank_row][blank_col + 1];
        temp[blank_row][blank_col + 1] = 0;
        nodes.push_back(blocks_to_string(temp, dim));
    }

    return nodes;
}

void print_puzzle(string puzzle, int dim){
    cout << endl;
    int start = puzzle.find(',') + 1;

    int count = 0;
    for(int i = start; i < puzzle.size(); i++){
        cout << puzzle[i];
        if(puzzle[i] == ' '){
            count++;
            if(count == dim){
                cout << endl;
                count = 0;
            }
        }
    }

    cout << endl;
}