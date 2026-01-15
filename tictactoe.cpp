/******************************************************************************
design a tictactoe game
*******************************************************************************/

#include <bits/stdc++.h>
using namespace std;



enum class Symbol{
    X,
    O,
    EMPTY
};


class Player{
    Symbol symbol;
    string name;
    public:
    Player(Symbol symbol,string name):symbol(symbol),name(name){}
    
    Symbol getSymbol(){
        return symbol;
    }

    string getName(){
        return name;
    }
    
};


class Board{
    protected:
    int n;
    vector<vector<Symbol>>grid;
    
    public:
    Board(int n) : n(n), grid(n, vector<Symbol>(n, Symbol::EMPTY)) {}
    
    bool canPlace(int row,int col){
        if(row<0 or row>=n or col<0 or col>=n)return false;
        return grid[row][col]==Symbol::EMPTY;
    }
    void place(int row,int col, Symbol symbol){
        if(!canPlace(row,col)){
            cout<<"cannot place symbol here:";
            return;
        }
        
        grid[row][col]=symbol;
    }
    
    int getSize(){
        return n;
    }

    const vector<vector<Symbol>>& getGrid(){
        return grid;
        
    }
};

class WinningStrategy{
    public:
    virtual bool checkWinner(Board &board,Symbol symbol,int row,int col)=0;
    virtual ~WinningStrategy() = default;
    
};


class NXNWinningstrategy:public WinningStrategy{
    public:
    bool checkWinner(Board &board,Symbol symbol,int row,int col)override{
        int n = board.getSize();
        bool winner = true;
        const auto &grid = board.getGrid();
        // row wise winner
        for(int j=0;j<n;j++){
            if(grid[row][j]!=symbol){
                winner =false;
                break;
            }
        }
        
        if(winner)return winner;
        
        for(int i=0;i<n;i++){
            if(grid[i][col]!=symbol){
                winner=false;
                break;
            }
        }
        if(winner)return winner;
        
        if(row==col){
            for(int i=0;i<n;i++){
                if(grid[i][i]!=symbol){
                    winner=false;
                    break;
                }
            }
            if(winner)return winner;
        }
         if(row+col== n-1){
            for(int i=0;i<n;i++){
                if(grid[i][n-i-1]!=symbol){
                    winner=false;
                    break;
                }
            }
            if(winner)return winner;
        }
        
        return false;
    }
    
};


class Game{
    Board board;
    queue<Player>players;
    WinningStrategy *winningStrategy;
    int movesPlayed;
    
    public:
    Game(int size,Player& p1,Player& p2,WinningStrategy& strategy): board(size), winningStrategy(&strategy),movesPlayed(0){
    players.push(p1);
    players.push(p2);
    }
    
    void playMove(int row,int col){
        auto currentPlayer = players.front();
        
        
        if(!board.canPlace(row,col)){
            cout<<"invalid move:"<<endl;
            return;
        }
        players.pop();
        board.place(row,col,currentPlayer.getSymbol());
        movesPlayed++;
        
        
        
        if(winningStrategy->checkWinner(board,currentPlayer.getSymbol(),row,col)){
            cout<<"Player "<<currentPlayer.getName()<<" won"<<endl;
            return;
        }
        if(movesPlayed==board.getSize() * board.getSize()){
            cout<<"game draw:"<<endl;
        }    
        players.push(currentPlayer);
    }

    
};


int main()
{
    Player p1(Symbol::X, "Alice");
    Player p2(Symbol::O, "Bob");

    NXNWinningstrategy strategy;
    Game game(3, p1, p2, strategy);

    // Dry run moves
    game.playMove(0, 0); // Alice (X)
    game.playMove(1, 1); // Bob (O)
    game.playMove(0, 1); // Alice (X)
    game.playMove(2, 2); // Bob (O)
    game.playMove(0, 2); // Alice (X) -> WIN

    return 0;
}