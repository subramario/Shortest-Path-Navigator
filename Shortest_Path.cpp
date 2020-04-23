#include <iostream>
#include <list>
#include <vector>
#include <regex>
#include <iterator>
#include <string>
#include <stack>
#include <list>

using namespace std; 

void add_edge(vector<int> adj_list[], int u, int v){ //creates an adjacency list (ie. array of arrays)
    adj_list[u].push_back(v); //add v into list u
    adj_list[v].push_back(u); //add u into list v
}

//Resource used: https://www.eecs.yorku.ca/course_archive/2006-07/W/2011/Notes/BFS_part2.pdf
void BFS_Shortest(int start, int end, int V, vector<int> adj_list[]){ // BFS algorithm to find the shortest path
    vector<int> line; //Simulates a queue data structure 
    bool visited[V]; //Keeps track of all nodes visited
    int prev[V]; //Links path from source node to end node

    for(int i = 0; i<V; i++){
        visited[i] = false; //"false" initialization indicastes no nodes have been traversed yet
        prev[i] = -1; //prev[i] is the direct predeccesor of i -> ie. value node is predecessor of indice node
    }

    visited[start] = true; //initialize start node as visited 
    line.push_back(start); //push start node to queue

    while (!line.empty()){
        int node = line.front(); //peak at front of line --> store value in variable "node"
        line.erase(line.begin()); //simulates dequeue function

        if (adj_list){
            for(unsigned int i = 0; i < adj_list[node].size(); i++){ //Loops through all connected vertices for node in adjacency list
                if (visited[adj_list[node][i]] == false){ //If node has been visited, will not run loop --> saves time
                    
                    visited[adj_list[node][i]] = true;
                    prev[adj_list[node][i]] = node;
                    line.push_back(adj_list[node][i]);

                    if (adj_list[node][i] == end){
                        stack <int> shortest_path; //Use stack data structure to recreate shortest path from start to end
                        int temp = end;
                        shortest_path.push(end);
                        while(prev[temp] != -1){
                            shortest_path.push(prev[temp]);
                            temp = prev[temp];
                        }

                        while(!shortest_path.empty()){ //Popping off the stack reproduces the shortest path from start to end
                            int i = shortest_path.top();
                            if (i == end)
                                cout << i << endl;
                            else
                                cout << i << "-";
                            shortest_path.pop();
                        }   
                              
                        return;
                    }
                }
            }
        }
    }
    cout << "Error: Vertices not connected - shortest path cannot be displayed." << endl;
    return;
}

//main function
int main(int argc, char* argv[]){
    //local variables declared here
   smatch m;
   string input;
   int a,b,c,V = 1;
   vector <int> *adj_list = NULL; //initializing adjacency list

    //matches command input to appropriate loop
   regex vert_rx("^[V]"); 
   regex edge_rx("^[E]");
   regex short_rx("^[s]");

   //extract relevant info from input
   const regex numbers("([0-9]+)+");
   const regex coordinates("([0-9]+[,][0-9]+)+");

   while (!cin.eof()){ //eof allows program to exit gracefully 
        getline(cin, input);

        if (regex_search(input,m,vert_rx)){
            regex_search(input, m, numbers);

            string temp = m[0];
            a = stoi(temp);

            if (a == 0) //Error case for if vertice = 0
                cout << "Error: Graph cannot have zero vertices!" << endl;
            else
                V = a;
        }

        if (regex_search(input,m,edge_rx)){
            adj_list = new vector<int>[V];
            while (regex_search(input,m,coordinates)){
                string s = m.str(0);
                
                string delimiter_1 = ",";
                string delimiter_2 = ">";
                string token_1 = s.substr(0, s.find(delimiter_1));
                string token_2 = s.substr(s.find(",")+1, s.find(delimiter_2));
                b = stoi(token_1);
                c = stoi(token_2);
                
                if (b == c){ //Error case when an edge spec includes the same vertice twice
                    cout << "Error: Cannot have an edge between a vertice and itself!" << endl;
                    break;
                }

                if (b >= V || c >= V){ //Error case when an edge spec includes a vertex higher than total number of vertices
                    cout << "Error: Cannot have an edge between non-existant vertices." << endl;
                    break;
                }

                bool flag_1 = false;
                bool flag_2 = false;


                for (unsigned int i = 0; i < adj_list[b].size(); i++){ //Error case for duplicate edges
                    if (adj_list[b][i] == c){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        flag_1 = true;
                        break;
                    }
                }

                if (flag_1 == true)
                    break;

                for (unsigned int i = 0; i < adj_list[c].size(); i++){ //Error case for duplicate edges
                    if (adj_list[c][i] == b){
                        cout << "Error: System does not allow duplicate edges." << endl;
                        flag_2 = true;
                        break;
                    }
                }

                if (flag_2 == true)
                    break;
                
                if (flag_1 == false && flag_2 == false) //If the edge spec is valid, add it to the adjacency list
                    add_edge(adj_list, b ,c);

                input = m.suffix(); //Used to iterate through remaining edges

            }

        }

        if (regex_search(input,m,short_rx)){
            regex_search(input,m,numbers);
            int start = stoi(m[0]);

            input = m.suffix();
            regex_search(input,m,numbers);
            int end = stoi(m[0]);

            if (start == end){ //Error case when user tries to specify shortest distance between a vertex and itself
                cout << "Error: Cannot have self-loop in an undirected graph!" << endl;
                continue;
            }

            else if (start >= V || end >= V){ //Error case when a user tries to specify shortest distance to a vertex that does not exist
                cout << "Error: Vertices do not exist - shortest path does therefore, not exist." << endl;
                continue;
            }

            else
                BFS_Shortest(start, end, V, adj_list);
        }
    }


   return 0;
} 
    