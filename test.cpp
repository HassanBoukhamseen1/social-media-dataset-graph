#include <cassert>
#include "graph.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "test.h"
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::endl;
using std::ifstream;
using std::find;
using std::vector;

/**
 * @brief extracts all the edges from the text files, then loops over all the edges and asserts that each edge exists
 */
void assertEdges() {
  Graph *graph = new Graph();
  string file = "./data/348.edges";
  vector<pair<string, string>> edges = graph->extractEdges(file);
  for (auto edge: edges) {graph->addEdge(edge.first, edge.second);}
  string vertex1;
  string vertex2;
  ifstream file_(file);
  vector<pair<string, string>>::iterator it;
  if (file_.is_open()) {
      while (file_ >> vertex1 >> vertex2) {
        it = find(edges.begin(), edges.end(), pair<string,string> (vertex1, vertex2));
        assert( it != edges.end()); 
        //if iterator resturn edges.end(), it means an edge in the text file
        //has not been found in the edges vector
      }
      file_.close();
      printf("\x1b[32m All Edge Tests Passed \x1b[0m  \n");
  } else {
      cout << "file not open" << endl;
  }
}

/**
 * @brief extracts all the vertices from the text files, then loops over all the vertices and asserts that each vertex exists
 */
void assertVertecies() {
  Graph *graph = new Graph();
  string file = "./data/348.feat";
  vector<string> vertices = graph->extractVertices(file);
  for (auto vertex: vertices) {graph->addVertex(vertex);}
  string vertex;
  string delimiter = " ";
  ifstream file_(file);
  vector<string>::iterator it;
  if (file_.is_open()) {
      while (getline(file_, vertex)) {
        it = find(vertices.begin(), vertices.end(), vertex.substr(0,vertex.find(delimiter)));
        assert(it != vertices.end());
        //if iterator resturn edges.end(), it means an edge in the text file
        //has not been found in the edges vector
      }
      file_.close();
      printf("\x1b[32m All Vertex Tests Passed \x1b[0m  \n");
  } else {
      cout << "file not open" << endl;
  }
}

/**
 * @brief extracts all the vertices and the edges and creates the graph, then checks that all the components exist in the right place
 */
void assertGraph() {
  Graph *graph = new Graph();
  string Vfile = "./data/0.feat";
  string Efile = "./data/0.edges";
  vector<string> vertices = graph->extractVertices(Vfile);
  vector<pair<string, string>> edges = graph->extractEdges(Efile);
  for (auto vertex: vertices) {graph->addVertex(vertex);}
  for (auto edge: edges) {graph->addEdge(edge.first, edge.second);}
  vector<pair<string, int>>::iterator it;
  vector<int> weights = graph->extractEdgeWeights(Vfile);
  unordered_map<string, vector<pair<string,int>>> adj = graph->getAdj();
  int i = 0;
  for (auto edge: edges) {
    it = find(adj[edge.first].begin(), adj[edge.first].end(), pair<string, int> (edge.second, weights[i]));
    assert(it !=  adj[edge.first].end());
    //asserts that for each edge in the file, there is a corrisponding edge in the adjacency list by checking 
    //the corrisponding index of both vertices in the adjacency list
    it = find(adj[edge.second].begin(), adj[edge.second].end(), pair<string, int> (edge.first, weights[i]));
    assert(it !=  adj[edge.second].end());
    i++;
  }
  printf("\x1b[32m All Graph Layout Tests Passed \x1b[0m  \n");
}

/**
 * @brief extracts all the vertices from the text file, then checks that it is gone after calling the remove method
 */
void assertRemoveVertex() {
  Graph *graph = new Graph();
  string file = "./data/348.feat";
  vector<string> originalVertices = graph->extractVertices(file);
  vector<string>::iterator it;
  for (int i = 0; i < originalVertices.size(); ++i) {
      graph->removeVertex(originalVertices[i]);
      vector<string> vertices = graph->getVertices();
      it = find(vertices.begin(), vertices.end(), originalVertices[i]);
      assert(it == vertices.end());
      //asserts that the iterator returns vertices.end(), which means that the vertex was not found
  }
  printf("\x1b[32m All Vertex Removal Tests Passed \x1b[0m  \n");
}

/**
 * @brief extracts all the edges from the text file, then checks that it is gone after calling the remove method
 */
void assertRemoveEdge() {
  Graph *graph = new Graph();
  string file = "./data/348.edges";
  vector<pair<string, string>> originalEdges = graph->extractEdges(file);
  vector<pair<string, string>>::iterator it;
  for (int i = 0; i < originalEdges.size(); ++i) {
      graph->removeEdge(originalEdges[i].first, originalEdges[i].second);
      vector<pair<string, string>> edges = graph->getEdges();
      it = find(edges.begin(), edges.end(), pair<string, string>
      (originalEdges[i].first, originalEdges[i].second));
      assert(it == edges.end());
      //asserts that the iterator returns edges.end(), which means that the edge was not found
  }
  printf("\x1b[32m All Edge Removal Tests Passed \x1b[0m  \n");
}

/**
 * @brief creates a sample graph and checks various BFS outputs that cover the edge cases
 */
void assertBFS() {
  Graph *graph = new Graph();
  string vertices[] = {"A", "B", "C", "D", "E", "F", "G"};
  for (int i = 0 ; i < 7; i++) {graph->addVertex(vertices[i]);}
  graph->addEdge("A", "B");
  graph->addEdge("B", "G");
  graph->addEdge("G", "D");
  graph->addEdge("G", "A");
  graph->addEdge("C", "D");
  graph->addEdge("D", "E");
  //case 1: simple test 
  string test1 = graph->BFS("A");
  string trueOutput1 = "BFS output: A, B, G, D, C, E, ";
  assert(test1 == trueOutput1);
  //case 2: start from somewhere in the middle 
  string test2 = graph->BFS("C");
  string trueOutput2 = "BFS output: C, D, G, E, B, A, " ;
  assert(test2 == trueOutput2);
  string test4 = graph->BFS("G");
  string trueOutput4 = "BFS output: G, B, D, A, C, E, ";
  assert(test4 == trueOutput4);
  string test5 = graph->BFS("D");
  string trueOutput5 = "BFS output: D, G, C, E, B, A, ";
  assert(test5 == trueOutput5);
  //case 3: start from a disconnected component
  string test3 = graph->BFS("F");
  string trueOutput3 = "BFS output: F, ";
  assert(test3 == trueOutput3);
  printf("\x1b[32m All BFS Tests Passed \x1b[0m  \n");
}

void assertShortestPath() {
  Graph *graph = new Graph();
  string vertices[] = {"A", "B", "C", "D", "E", "F", "G"};
  for (int i = 0 ; i < 7; i++) {graph->addVertex(vertices[i]);}
  graph->addEdge("A", "B", 1);
  graph->addEdge("B", "G", 2);
  graph->addEdge("G", "D", 1);
  graph->addEdge("G", "A", 7);
  graph->addEdge("C", "D", 5);
  graph->addEdge("D", "E", 3);
  graph->addEdge("A", "E", 12);
  //case 1: there is a direct edge from A to E, but path is cheaper through B, G, D, E
  vector<string> test1 = graph->shortestPath("A", "E");
  vector<string> trueOutput1 = {"E", "D", "G", "B", "A"};
  assert(test1 == trueOutput1);
  //case 2: no path from source to destination
  vector<string> test2 = graph->shortestPath("G", "F");
  vector<string> trueOutput2 = {};
  assert(test2 == trueOutput2);
  //case 3: simple test
  vector<string> test3 = graph->shortestPath("C", "D");
  vector<string> trueOutput3 = {"D", "C"};
  assert(test3 == trueOutput3);
  //case 4: average case
  vector<string> test4 = graph->shortestPath("B", "D");
  vector<string> trueOutput4 = {"D", "G", "B"};
  assert(test4 == trueOutput4);

  printf("\x1b[32m All Shortest Path Tests Passed \x1b[0m  \n");
}

/**
 * @brief creates a sample graph and checks various LandmarkPath outputs that cover the edge cases
 */
void assertLandmarkPath() {
  Graph *graph = new Graph();
  string vertices[] = {"A", "B", "C", "D", "E", "F", "G"};
  for (int i = 0 ; i < 7; i++) {graph->addVertex(vertices[i]);}
  graph->addEdge("A", "B", 1);
  graph->addEdge("B", "G", 2);
  graph->addEdge("G", "D", 1);
  graph->addEdge("G", "A", 7);
  graph->addEdge("C", "D", 5);
  graph->addEdge("D", "E", 3);
  graph->addEdge("A", "E", 1);
  //case 1: landmark is not in the direct path from the soource to the destination 
  vector<string> test1 = graph->landMarkPath("A", "C", "E");
  vector<string> trueOutput1 = {"E", "D", "C", "D", "E", "A"};
  assert(test1 == trueOutput1);
  vector<string> test2 = graph->landMarkPath("A", "G", "B");
  vector<string> trueOutput2 = {"B", "G", "B", "A"};
  assert(test2 == trueOutput2);
  //case 2: average landmark usage, a shorted path exists, but the path has to include the landmark
  vector<string> test3 = graph->landMarkPath("A", "B","D");
  vector<string> trueOutput3 = {"D", "G", "B", "A"};
  assert(test3 == trueOutput3);
  vector<string> test4 = graph->landMarkPath("B", "G", "E");
  vector<string> trueOutput4 = {"E", "D", "G","B"};
  assert(test4 == trueOutput4);
  //case 3: no path from source to destination
  vector<string> test5 = graph->landMarkPath("B", "G", "F");
  vector<string> trueOutput5 = {};
  assert(test5 == trueOutput5);

  printf("\x1b[32m All Landmark Path Tests Passed \x1b[0m  \n");
}

int main() {
  assertEdges();
  assertVertecies();
  assertGraph();
  assertRemoveVertex();
  assertRemoveEdge();
  assertBFS();
  assertShortestPath();
  assertLandmarkPath();
  return 0;
}
