/**
 * "graph.h"
 * @author Hassan Boukhamseen, Priyank Jain, Rishi Puranik
 * Header file for Graph Class
 */


#ifndef graph_h
#define graph_h
#include <vector>

#include <unordered_map>
#include <map>
#include <string>
#include <stdio.h>
using std::vector;
using std::pair;
using std::unordered_map;
using std::map;
using std::string;

class Graph {
private:
    unordered_map<string, vector<pair<string,int>>> adj; //a map that maps a vertex to a list of its neighbours 
    vector<string> vertices; // a vector of the extracted vertices from the text files
    vector<pair<string, string>> edges; // a vector of the extracted edges from the text files
public:
    vector<string> extractVertices(string file);
    vector<pair<string, string>> extractEdges(string file);
    vector<int> extractEdgeWeights(string file);
    void addVertex(string key);
    void removeVertex(string key);
    void addEdge(string v1, string v2, int weight=224);
    void removeEdge(string v1, string v2);
    void setEdgeWeight(string v1, string v2, int weight);
    void printGraph();
    string BFS(string src);
    void DJK(string src);
    int DJK(string src, string dest);
    vector<string> shortestPath(string src, string dest);
    vector<string> landMarkPath(string src, string lm, string dest);
    vector<string> getPath(map<string, string> prev, string src, string dest, vector<string> path);
    unordered_map<string, vector<pair<string,int>>> getAdj();
    vector<string> getVertices();
    vector<pair<string, string>> getEdges();
};
#endif /* graph_hpp */
