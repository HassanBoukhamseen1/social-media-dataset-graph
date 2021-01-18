/**
 * "graph.cpp"
 * @author Hassan Boukhamseen, Priyank Jain, Rishi Puranik
 * Implementation of Graph Class
 */

#include "graph.h"
#include <iostream>
#include <queue>
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <map>

using std::priority_queue;
using std::queue;
using std::cout;
using std::endl;
using std::ifstream;
using std::greater;
using std::map;


/**
 * @brief extractVertecies will return a vector of strings containing all the vertices in the given file.
 * 
 * @param file 
 * @return vector<string> 
 */
vector<string> Graph::extractVertices(string file) {
    string vertex;
    string delimiter = " ";
    ifstream file_(file);
    if (file_.is_open()) {
        while (getline(file_, vertex)) {
            vertices.push_back(vertex.substr(0,vertex.find(delimiter)));
        }
        file_.close();
    } else {
        cout << "file not open" << endl;
    }
    return vertices;
}

/**
 * @brief extractEdges will return a vector of string pairs, containing all the edges in the given file.
 * 
 * @param file 
 * @return vector<pair<string, string>> 
 */
vector<pair<string, string>> Graph::extractEdges(string file) {
    string vertex1;
    string vertex2;
    ifstream file_(file);
    if (file_.is_open()) {
        while (file_ >> vertex1 >> vertex2) {
            edges.push_back(pair<string, string> (vertex1, vertex2));
        }
        file_.close();
    } else {
        cout << "file not open" << endl;
    }
    return edges;
}


/**
 * @brief extractEdgeWeights will return an integer vector containing the weights of all the edges in a given file.
 * 
 * @param file 
 * @return vector<int> 
 */
vector<int> Graph::extractEdgeWeights(string file) {
    vector<int> weights;
    ifstream file_(file);
    string vertex;
    string v;
    string featStr = " ";
    unordered_map<string, string> features;
    if (file_.is_open()) {
        while (getline(file_, vertex)) {
            v = vertex.substr(0, vertex.find(" ")); 
            featStr = vertex.substr(vertex.find(" "), vertex.find("\n")); 
            features[v] = featStr;
        }
        file_.close();
        int common_features;
        for (auto edge:edges) {
            common_features = 0;
            for (int c = 0; c < 448; c++) {
                if (features[edge.first][c] == '1' && features[edge.second][c] == '1') {common_features++;}
            }
            setEdgeWeight(edge.first, edge.second, 224 - common_features);
            weights.push_back(224 - common_features);
        }
        return weights;
    } else {cout << "file not open" << endl;}
    return weights;
}

/**
 * @brief addVertex will add a vertex into the vertices vector based on the value of the key string.
 * 
 * @param key 
 */
void Graph::addVertex(string key) {
    adj[key] = vector<pair<string,int>> ();
    vertices.push_back(key);
}

/**
 * @brief removeVertex will remove a vertex from the vertices vector based on the value of the key string.
 * 
 * @param key 
 */
void Graph::removeVertex(string key) {
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    adj.erase(key);
    vector<string>::iterator it;
    it = find(vertices.begin(), vertices.end(), key);
    vertices.erase(it);
    for (i = adj.begin(); i != adj.end(); ++i) {
        int index = 0;
        for (auto vertex : adj[i->first]) {
            if (vertex.first == key) {
                adj[i->first].erase(adj[i->first].begin() + index);
                index++;
            }
        }
    }
}

/**
 * @brief addEdge will add a weighted edge, between v1 and v2, to the adjacency map.
 * 
 * @param v1 
 * @param v2 
 * @param weight 
 */
void Graph::addEdge(string v1, string v2, int weight) {
    adj[v1].push_back(pair<string,int> (v2, weight));
    adj[v2].push_back(pair<string,int> (v1, weight));
}

/**
 * @brief removeEdge will remove the edge specified by its vertices, v1 and v2.
 * 
 * @param v1 
 * @param v2 
 */
void Graph::removeEdge(string v1, string v2) {
    int index1 = 0;
    for (auto v: adj[v1]) {
        if (v.first == v2) {adj[v1].erase(adj[v1].begin() + index1);}
        index1++;
    }
    int index2 = 0;
    for (auto v: adj[v2]) {
        if (v.first == v1) {adj[v2].erase(adj[v2].begin() + index2);}
        index2++;
    }
    vector<pair<string, string>>::iterator it;
    it = find(edges.begin(), edges.end(), pair<string, string> (v1, v2));
    edges.erase(it);
}

/**
 * @brief setEdgeWeight will modify the edge specified by v1 and v2, setting it to the given weight parameter.
 * 
 * @param v1 
 * @param v2 
 * @param weight 
 */
void Graph::setEdgeWeight(string v1, string v2, int weight) {
    int index1 = 0;
    for (auto v : adj[v1]) {
        if (v.first == v2) {adj[v1][index1].second = weight;}
        index1++;
    }
    int index2 = 0;
    for (auto v : adj[v2]) {
        if (v.first == v1) {adj[v2][index2].second = weight;}
        index2++;
    }
}

/**
 * @brief printGraph prints the graph.
 * 
 */
void Graph::printGraph() {
    cout << "Graph layout: " << endl;
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i) {
        cout << "Vertex: " << i->first << "-> {";
        for (auto vertex: adj[i->first]) {
            cout << "<" << vertex.first << ", " << vertex.second << ">";
        }
        cout << "}\n" << endl;
    }
}

/**
 * @brief BFS performs a breadth-first search from the given start vertex, src.
 * 
 * @param src 
 */
string Graph::BFS(string src) {
    string output =  "BFS output: ";
    unordered_map<string, bool> visited;
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i) {visited[i->first] = false;}
    visited[src] = true;
    queue<string> q;
    q.push(src);
    while (!q.empty()) {
        string vertex = q.front();
        output += vertex + ", ";
        q.pop();
        for (auto v: adj[vertex]) {
            if (!visited[v.first]) {
                visited[v.first] = true;
                q.push(v.first);
            }
        }
    }
    return output;
}

/**
 * @brief returns the weights of the shortest path from a src vertex to all the pther vertices
 * 
 * @param src 
 */
void Graph::DJK(string src) {
    cout << "Dijkstra's output: ";
    unordered_map<string, int> dist;
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i) {dist[i->first] = 1000000000;}
    dist[src] = 0;
    priority_queue<pair<int, string>, vector<pair<int, string>> , greater<pair<int, string>>> q; 
    q.push(pair<int, string> (0, src));
    while (!q.empty()) {
        string vertex = q.top().second;
        q.pop();
        for (auto v: adj[vertex]) {
            if (dist[v.first] > dist[vertex] + v.second) {
                dist[v.first] = dist[vertex] + v.second;
                q.push(pair<int, string> (dist[v.first], v.first));
            }
        }
    }
    unordered_map<string,int>::iterator it;
    for (i = adj.begin(); i != adj.end(); ++i) {
        if (i->first == src) {continue;}
        cout << "Distance from user " << src << " to user " << i->first << " = " << dist[i->first] << endl;
    }
}

/**
 * @brief DJK finds the shortest path between vertex src and vertex dest, using Dijkstra's Algorithm.
 *        DJK returns an integer corresponding to the weight of the path between src and dest.
 * 
 * @param src 
 * @param dest 
 * @return int 
 */
int Graph::DJK(string src, string dest) {
    unordered_map<string, int> dist;
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i) {dist[i->first] = 1000000000;}
    dist[src] = 0;
    priority_queue<pair<int, string>, vector<pair<int, string>> , greater<pair<int, string>>> q; 
    q.push(pair<int, string> (0, src));
    while (!q.empty()) {
        string vertex = q.top().second;
        q.pop();
        for (auto v: adj[vertex]) {
            if (dist[v.first] > dist[vertex] + v.second) {
                dist[v.first] = dist[vertex] + v.second;
                q.push(pair<int, string> (dist[v.first], v.first));
            }
        }
    }
    return dist[dest];
}

/**
 * @brief an implementation of Dijkstra's algorithm that returns the path from the src vertec to the destination vertex
 * 
 * @param src 
 * @param dest 
 * @return vector<string> 
 */
vector<string> Graph::shortestPath(string src, string dest) {
    vector<string> path;
    if (adj[src].empty() || adj[dest].empty()) {
        return path;
    }
    unordered_map<string, int> dist;
    map<string, string> prev;
    unordered_map<string, vector<pair<string,int>>>::iterator i;
    for (i = adj.begin(); i != adj.end(); ++i) {
        dist[i->first] = 1000;
        prev[i->first] = "";
    }
    dist[src] = 0;
    priority_queue<pair<int, string>, vector<pair<int, string>> , greater<pair<int, string>>> q; 
    q.push(pair<int, string> (0, src));
    while (!q.empty()) {
        string vertex = q.top().second;
        q.pop();
        for (auto v: adj[vertex]) {
            if (dist[v.first] > dist[vertex] + v.second) {
                dist[v.first] = dist[vertex] + v.second;
                prev[v.first] = vertex;
                q.push(pair<int, string> (dist[v.first], v.first));
            }
        }
    }
    path.push_back(dest);
    path = getPath(prev, dest, src, path);
    return path;
}


/**
 * @brief takes a map of all the visited vertices from the shortestPath method and traces
 *      the path of the vertices from the source to the destination
 * 
 * @param prev
 * @param dest 
 * @param src 
 * @param path 
 * @return vector<string> 
 */
vector<string> Graph::getPath(map<string, string> prev, string dest, string src, vector<string> path) {
    if (dest == src) {return path;}
    path.push_back(prev[dest]);
    dest = prev[dest];
    return getPath(prev, dest, src, path);
}

/**
 * @brief LandmarkPath returns the path between vertices src and dest, through vertex lm.
 *        This is done with two separate calls to the DJK method.
 * 
 * @param src 
 * @param lm 
 * @param dest 
 * @return int 
 */
vector<string> Graph::landMarkPath(string src, string lm, string dest) {
    if (DJK(src, lm) != 1000000000 && DJK(lm, dest) != 1000000000) {
        vector<string> path1 = shortestPath(src, lm);
        vector<string> path2 = shortestPath(lm, dest);
        for (auto i = path1.begin(); i != path1.end(); ++i) {
            if (*i == lm) {continue;}
            path2.push_back(*i);
        }
        return path2;
    }
    vector<string> empty;
    return empty;
}

unordered_map<string, vector<pair<string,int>>> Graph::getAdj() {return this->adj;}

vector<string> Graph::getVertices() {return this->vertices;}

vector<pair<string, string>> Graph::getEdges() {return this->edges;}
