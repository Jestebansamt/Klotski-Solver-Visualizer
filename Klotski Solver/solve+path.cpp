#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define pii pair<int,int>


vector<vector<vector<char>>> solution;

void saveToJS(const string &filename) {
    ofstream jsFile(filename);
    if (!jsFile.is_open()) {
        cerr << "Error al abrir el archivo " << filename << endl;
        return;
    }

    // Inicia el archivo JS con una declaración de variable
    jsFile << "const klotskiMoves = [\n";

    for (size_t k = 0; k < solution.size(); ++k) {
        jsFile << "  [\n";
        for (const auto &row : solution[k]) {
            jsFile << "    [";
            for (size_t j = 0; j < row.size(); ++j) {
                jsFile << "'" << row[j] << "'";
                if (j < row.size() - 1) jsFile << ", ";
            }
            jsFile << "]";
            if (&row != &solution[k].back()) jsFile << ",";
            jsFile << "\n";
        }
        jsFile << "  ]";
        if (k < solution.size() - 1) jsFile << ",";
        jsFile << "\n";
    }

    // Cierra el arreglo en el archivo JS
    jsFile << "];\n";

    jsFile.close();
    cout << "Archivo " << filename << " generado correctamente." << endl;
}


vector<pii> moves = {{1,0},{0,1},{-1,0},{0,-1}};
set<string> isVisited;
int solve = 0;
vector<vector<char>> start = {
    {'A', 'B', 'B', 'C'},
    {'A', 'B', 'B', 'C'},
    {'D', 'E', 'E', 'F'},
    {'D', 'G', 'H', 'F'},
    {'I', ' ', ' ', 'J'},
};

map<char,char> blocksLenght = {{'A','2'}, {'B','4'},{'C','2'},{'D','2'},{'E','2'},{'F','2'},{'I','1'},{'G','1'},{'H','1'},{'J','1'}};


string funtionHash(vector<vector<char>> grid){
    string stringHash = "";
    for(auto&a:grid){
        for(auto&f:a) stringHash+=blocksLenght[f];
    }
    return stringHash;
}



void printBoard(vector<vector<char>> board){
    for(int i=0;i<5;++i){
        for(int j=0;j<4;++j){
            cout << board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

bool isSolve(vector<vector<char>> board){
    return(board[4][1]== 'B' && board[4][2]== 'B' && board[3][1] == 'B' && board[3][2] == 'B');
}



bool isValid(int i, int j, int x, int y, vector<vector<char>> board){
    char blockName = board[i][j];
    return (i+x>= 0 && i+x< 5 && j+y >= 0 && j+y < 4 &&
    (board[i+x][j+y] == ' ' || board[i+x][j+y] == blockName)); 
}


int dfs(vector<pii> block, vector<vector<char>> board){
    if (solve) return 1; 

    isVisited.insert(funtionHash(board));
    char blockName = board[block[0].first][block[0].second];
    vector<pii> moveBlock(block.size());

    // Revisar movimiento del bloque en las cuatro direcciones
    for(auto&[y,x]:moves){
        bool van = 1;
        for(auto&[i,j]:block){
            if(!isValid(i,j,x,y, board)) van = 0;
        }
        if(van){
            vector<vector<char>> newBoard = board;
            int part = 0;

            // Identificar donde queda la ficha despues del movimiento
            for(auto&[i,j]:block){
                moveBlock[part] = {{i+x},{j+y}};
                part++;
            }  
            
            // Actualizar el tablero
            for(int i=0;i<5;++i){
                for(int j=0; j<4; ++j){
                    if(newBoard[i][j] == blockName) newBoard[i][j] = ' ';
                }
            }

            for(auto&[i,j]:moveBlock){
                newBoard[i][j] = blockName;
            }

            // Revisar si el nuevo tablero esta resuelto o mandar otra dfs por cada bloque
            if(isSolve(newBoard)){
                solution.push_back(newBoard);
                solve = 1;
                return 1;
            }else if(isVisited.find(funtionHash(newBoard)) == isVisited.end()){
                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        char blockName = newBoard[i][j];

                        // Encontrar todas las posiciones del bloque
                        vector<pii> newBlock;
                        for (int x = 0; x < 5; ++x) {
                            for (int y = 0; y < 4; ++y) {
                                if (newBoard[x][y] == blockName) {
                                    newBlock.push_back({x, y});
                                }
                            }
                        }

                        if (!newBlock.empty() && dfs(newBlock, newBoard)) {
                            solution.push_back(newBoard);
                            return 1;
                        }
                    }
                }
            }
        }    
    }
    return 0;
}

int main(){
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 4; ++j) {
            char blockName = start[i][j];

            // Encontrar todas las posiciones del bloque
            vector<pii> block;
            for (int x = 0; x < 5; ++x) {
                for (int y = 0; y < 4; ++y) {
                    if (start[x][y] == blockName) {
                        block.push_back({x, y});
                    }
                }
            }
            dfs(block,start);
        }
    }

    solution.push_back(start);
    reverse(solution.begin(), solution.end());
    saveToJS("klotskiVisualizer.js");

    return 0;
}