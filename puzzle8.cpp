#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
using namespace std;

class anode{
    public:
    int dept, gn, hn;//holds the depth, g(n), h(n)
    //must change this for different sizes
    vector<vector<int>> state = {{0,0,0},{0,0,0},{0,0,0}};//holds the current puzzle
    anode(int d, int g, int h, vector<vector<int>> s)
        :dept(d),gn(g),hn(h)
    {
        for(int i = 0; i < s.size(); ++i){
            for(int j = 0;j < s.size(); ++j){
                state.at(i).at(j) = (s.at(i).at(j));
            }
        }
    }
};
void clone(anode &dest, anode copi){
    dest.dept = copi.dept;
    dest.gn = copi.gn;
    dest.hn = copi.hn;
    dest.state = copi.state;
}
class Compare{
    public:
        bool operator() (anode m,anode n){
            return (m.gn+m.hn) > (n.gn+n.hn);
        }
};
void printnode(vector<vector<int>> nodes){
    for(int i = 0; i < nodes.size(); ++i){
        for(int j = 0; j < nodes.at(i).size(); ++j){
            cout << nodes.at(i).at(j) << " ";
        }
        cout << endl;
    }
}
//Printing trace for Manhattan distance and misplaced tiles
void printtrace(int g, int h, vector<vector<int>> nodes){
    cout << "The best state to expand with a g(n) = " << g << " and h(n) = "
        << h << " is..." << endl;
    printnode(nodes);
    cout<< "Expanding this node..." << endl;
}
//Printing trace for uniform cost search
void printuni(int cost, vector<vector<int>> nodes){
    cout << "The best state to expand with a total cost of " << cost
        << " is..." << endl;
    printnode(nodes);
    cout<< "Expanding this node..." << endl;
}
//Movement operator up
bool moveblankup(vector<vector<int>> &node){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) == 0){
                if(i == 0){
                    return false;
                }
                node.at(i).at(j) = node.at(i-1).at(j);
                node.at(i-1).at(j) = 0;
                return true;
            }
        }
    }
}
//Movement operator down
bool moveblankdown(vector<vector<int>> &node){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) == 0){
                if(i == node.size()-1){
                    return false;
                }
                node.at(i).at(j) = node.at(i+1).at(j);
                node.at(i+1).at(j) = 0;
                return true;
            }
        }
    }
}
//Movement operator to the left
bool moveblankleft(vector<vector<int>> &node){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) == 0){
                if(j == 0){
                    return false;
                }
                node.at(i).at(j) = node.at(i).at(j-1);
                node.at(i).at(j-1) = 0;
                return true;
            }
        }
    }
}
//Movement operator to the right
bool moveblankright(vector<vector<int>> &node){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) == 0){
                if(j == node.at(i).size()-1){
                    return false;
                }
                node.at(i).at(j) = node.at(i).at(j+1);
                node.at(i).at(j+1) = 0;
                return true;
            }
        }
    }
}

bool checkrepeat(vector<vector<int>> node, vector<vector<int>> node2){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) != node2.at(i).at(j)){
                return false;
            }
        }
    }
    return true;
}
//checking for repeats in uniform cost
bool notrepeat2(queue<anode> list, anode node){
    anode temp(0,0,0,node.state);
    while(!list.empty()){
        clone(temp,list.front());
        list.pop();
        if(checkrepeat(temp.state, node.state)){
            return false;
        }
    }
    return true;
}
//checking for repeats in a*
bool notrepeat(priority_queue<anode,vector<anode>,Compare> list, anode node){
    anode temp(0,0,0,node.state);
    while(!list.empty()){
        clone(temp,list.top());
        list.pop();
        if(checkrepeat(temp.state, node.state)){
            return false;
        }
    }
    return true;
}

//Compares solutions
bool isSolution(vector<vector<int>> node, vector<vector<int>> solution){
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) != solution.at(i).at(j)){
                return false;
            }
        }
    }
    return true;
}

//uniform search
void uniformsearch(vector<vector<int>> node){
    int numex = 0,maxq = 0, tempq = 1;
    anode curr(0,0,0,node);
    vector<vector<int>> solution = {{1,2,3},{4,5,6},{7,8,0}};
    queue<anode> listnodes;
    queue<anode> visited;
    listnodes.push(curr);
    while(!listnodes.empty()){
        clone(curr, listnodes.front());
        listnodes.pop();
        tempq--;
        if(isSolution(curr.state, solution)){
           cout << "Solution found!" << endl;
            printnode(curr.state);
            cout << "Total number of nodes this algorithm expanded: " << numex
                << endl << "The max number of nodes in the queue at anytime was: "
                << maxq << endl << "The depth of the solution was at: " << curr.dept
                << endl;
            return;
        }
        printuni(curr.gn, curr.state);
        anode temp(curr.dept+1, curr.gn+1, curr.hn,curr.state);
        if(moveblankup(temp.state) && notrepeat2(visited,temp)){ //generates the possible expansions
            listnodes.push(temp);//loads them onto the queue if they are valid
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankdown(temp.state) && notrepeat2(visited,temp)){
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankleft(temp.state) && notrepeat2(visited,temp)){
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankright(temp.state) && notrepeat2(visited,temp)){
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(tempq > maxq){
            maxq = tempq;
        }
    }
}
//calculates the hn for misplaced
int misplacedcount(vector<vector<int>> node){
    vector<vector<int>> solution = {{1,2,3},{4,5,6},{7,8,0}};
    int count = 0;
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) != solution.at(i).at(j)){
                count++;
            }
        }
    }
    return count;
}

void misplacAstar(vector<vector<int>> node){
      int numex = 0,maxq = 0, tempq = 1;
    anode curr(0,0,misplacedcount(node),node);
    vector<vector<int>> solution = {{1,2,3},{4,5,6},{7,8,0}};
    priority_queue<anode, vector<anode>, Compare> listnodes;
    priority_queue<anode, vector<anode>, Compare> visited;
    listnodes.push(curr);
    visited.push(curr);
    while(!listnodes.empty()){
        clone(curr, listnodes.top());
        listnodes.pop();
        tempq--;
        if(isSolution(curr.state, solution)){
           cout << "Solution found!" << endl;
            printnode(curr.state);
            cout << "Total number of nodes this algorithm expanded: " << numex
                << endl << "The max number of nodes in the queue at anytime was: "
                << maxq << endl << "The depth of the solution was at: " << curr.dept
                << endl;
            return;
        }
        printtrace(curr.gn,curr.hn,curr.state);
        anode temp(curr.dept+1, curr.gn+1, curr.hn,curr.state);
        if(moveblankup(temp.state) && notrepeat(visited,temp)){ //generates the possible expansions
            temp.hn = misplacedcount(temp.state);
            listnodes.push(temp);//loads them onto the queue if they are valid
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankdown(temp.state) && notrepeat(visited,temp)){
            temp.hn = misplacedcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankleft(temp.state) && notrepeat(visited,temp)){
            temp.hn = misplacedcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankright(temp.state) && notrepeat(visited,temp)){
            temp.hn = misplacedcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(tempq > maxq){
            maxq = tempq;
        }
    }
}

int manhatcount(vector<vector<int>> node){
    vector<vector<int>> solution = {{1,2,3},{4,5,6},{7,8,0}};
    int count = 0, temp = 0;
    for(int i = 0; i < node.size(); ++i){
        for(int j = 0; j < node.at(i).size(); ++j){
            if(node.at(i).at(j) != solution.at(i).at(j)){
                if(node.at(i).at(j) == 0){
                    node.at(i).at(j) = 9;
                }
                count += abs(i-(abs(node.at(i).at(j)-1)/3))
                    + abs(j-(abs(node.at(i).at(j)-1)%3));
                    //distance current piece is from it's solution part
            }
        }
    }
    return count;
}

void manhatAstar(vector<vector<int>> node){
      int numex = 0,maxq = 0, tempq = 1;
    anode curr(0,0,manhatcount(node),node);
    vector<vector<int>> solution = {{1,2,3},{4,5,6},{7,8,0}};
    priority_queue<anode, vector<anode>, Compare> listnodes;
    priority_queue<anode, vector<anode>, Compare> visited;
    listnodes.push(curr);
    visited.push(curr);
    while(!listnodes.empty()){
        clone(curr, listnodes.top());
        listnodes.pop();
        tempq--;
        if(isSolution(curr.state, solution)){
           cout << "Solution found!" << endl;
            printnode(curr.state);
            cout << "Total number of nodes this algorithm expanded: " << numex
                << endl << "The max number of nodes in the queue at anytime was: "
                << maxq << endl << "The depth of the solution was at: " << curr.dept
                << endl;
            return;
        }
        printtrace(curr.gn,curr.hn,curr.state);
        anode temp(curr.dept+1, curr.gn+1, curr.hn,curr.state);//since cost is only 1, increment g(n) by 1
        if(moveblankup(temp.state) && notrepeat(visited,temp)){ //generates the possible expansions
            temp.hn = manhatcount(temp.state);//assigns h(n)
            listnodes.push(temp);//loads them onto the queue if they are valid
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankdown(temp.state) && notrepeat(visited,temp)){
            temp.hn = manhatcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankleft(temp.state) && notrepeat(visited,temp)){
            temp.hn = manhatcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(moveblankright(temp.state) && notrepeat(visited,temp)){
            temp.hn = manhatcount(temp.state);
            listnodes.push(temp);
            visited.push(temp);
            tempq++;
            numex++;
        }
        temp.state = curr.state;
        if(tempq > maxq){
            maxq = tempq;
        }
    }
}

int main(){
    vector<vector<int>> initial = {{1,2,3},{4,0,6},{7,5,8}};
    int puznum =0, filler = 0;
    while(puznum != 1 && puznum != 2){
        cout << "Type '1' to use a default puzzle or '2' to enter your own puzzle:"
            << endl;
        cin >> puznum;
        if(puznum == 2){
            cout <<"Enter your puzzle, use a zero to represent the blank"<<endl;
            for(int i = 0; i < initial.size(); ++i){
                cout << "Enter row "<<i+1
                    << ", use a space or tab between numbers:\t";
                for(int j = 0; j < initial.at(i).size(); ++j){
                    cin >> filler;
                    initial.at(i).at(j) = filler;
                }
            }
        }
    }
    puznum = 0;
    while(puznum != 1 && puznum != 2 && puznum != 3){
        cout << "Enter your algorithm of choice"<< endl << "1. Uniform Cost Search"
            << endl << "2. A* with Misplaced Tile heuristic" << endl
            << "3. A* with the Manhattan Distance heuristic" << endl << endl;
            cin >> puznum;
        if(puznum == 1){
            cout << endl<< endl;
            uniformsearch(initial);
        }
        else if(puznum == 2){
            cout << endl<< endl;
            misplacAstar(initial);
        }
        else if(puznum == 3){
            cout << endl<< endl;
            manhatAstar(initial);
        }
    }
    return 0;
}
