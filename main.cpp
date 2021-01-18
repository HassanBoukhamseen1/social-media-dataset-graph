#include "graph.h"
#include "graph.cpp"
#include <iostream>
#include <vector>
#include <fstream>

#include <unordered_map>
#include <string>

using std::vector;
using std::pair;
using std::unordered_map;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::cin;


int main() {
    Graph *graph = new Graph();
    string featuresAndVerteciesFile = "./data/107.feat";
    string edgeFile = "./data/107.edges";
    vector<string> vertecies = graph->extractVertices(featuresAndVerteciesFile);
    vector<pair<string, string>> edges = graph->extractEdges(edgeFile);
    for (auto vertex: vertecies) {
        graph->addVertex(vertex);
    }
    for (auto edge: edges) {
        string vertex1 = edge.first;
        string vertex2 = edge.second;
        graph->addEdge(vertex1, vertex2);
    }
    graph->extractEdgeWeights(featuresAndVerteciesFile);
    graph->printGraph();
    cout << graph->BFS("900") << endl;
    string src = "1000";
    string dest = "1232";
    string lm = "901";
    if (graph->DJK(src, dest) != 1000) {cout << "Distance from user " << src << " to user " << dest << " = " << graph->DJK(src, dest) << endl;}
    vector<string> shortest = graph->shortestPath(src, dest);
    vector<string> landMark = graph->landMarkPath(src, lm, dest);
    cout << " shortest path: {";
    for (auto i = shortest.rbegin(); i != shortest.rend() ; i++) {cout << *i << ", ";}
    cout << "}" << endl;
    cout << " landmark path " << "through user " << lm << ":  {";
    for (auto i = landMark.rbegin(); i != landMark.rend() ; i++) {cout << *i << ", ";}
    cout << "}" << endl;
    /*
   Graph *g = new Graph();
   g->addVertex("0");
   g->addVertex("1");
   g->addVertex("2");
   g->addVertex("3");
   g->addVertex("4");
   g->addEdge("0", "1", 3);
   g->addEdge("0", "3", 7);
   g->addEdge("0", "4", 8);
   g->addEdge("4", "3", 3);
   g->addEdge("3", "1", 4);
   g->addEdge("3", "2", 2);
   g->addEdge("1", "2", 1);
   cout << endl;
   g->printGraph();
   vector<string> shortest = g->shortestPath("1", "3");
   cout << "shortest path from vertex 1 to 3:  {";
    for (auto i = shortest.rbegin(); i != shortest.rend() ; i++) {cout << "->" << *i;}
    cout << "}" << endl;
    cout << endl;
    cout << "distance from vertex 1 to 3 = " << g->DJK("1", "3") << endl;
    cout << endl;
   vector<string> landmark = g->landMarkPath("1", "0", "2");
   cout << "landmark path from vertex 1 to vertex 2 through vertex 0: {";
   for (auto i = landmark.rbegin(); i != landmark.rend() ; i++) {cout << *i << ", ";}
   cout << "}" << endl;
   cout << endl;
   cout << g->BFS("3") << endl;
   cout << endl;
*/
    return 0;

}
