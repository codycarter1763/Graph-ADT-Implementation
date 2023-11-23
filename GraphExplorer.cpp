/**
 * A test bed for a weighted digraph abstract data type implementation
 * and implementations of elementary classical graph algorithms that use the
 * ADT
 * @see Graph.h, Graph.cpp, City.h
 * @author Duncan, Cody Carter
 * <pre>
 * usage: GraphExplorer <graphFileName>
 * <graphFileName> - a text file containing the description of the graph
 * in DIMACS file format
 * Date: 11-21-2023
 * course: csc 3102
 * programming project 3
 * Instructor: Dr. Duncan
 * </pre>
 */

#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <limits>
#include <iomanip>
#include <string>
#include <queue>
#include <functional>
#include <vector>
#include <algorithm>
#include "City.h"
#include "Graph.h"
#include "Graph.cpp"


using namespace std;

#define INFINITY numeric_limits<double>::max()
#define NIL -1

/* function prototypes */   
Graph<City> readGraph(string filename);
int menu();
int** genIncMatUDG(const Graph<City>& g);
bool isStronglyConnected(const Graph<City>& g);
bool topSortOutDeg(const Graph<City> &g, int*& linearOrder);
double  kruskalMST(const Graph<City>& g, int* parent);
string trim(string text);

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      cout<<"Usage: GraphDemo <filename>"<<endl;
      exit(1);
   }
   Graph<City> g;
   City aCity1, aCity2;
   int menuReturnValue, i, j,k; 
   //Define a lambda function f:(const City& c) -> void that displays
   //the key followed by the label (name of the city) in two-column format
   //and advances the cursor to the next line. This function will be used
   //as an argument to bfsTraverse and dfsTraverse and to display the 
   //topological listing of the vertices.
   //auto f = ...;
   auto f = [](const City& c) -> void
   {
       cout<<c.getKey()<<" "<<c.getLabel()<<endl;
   };
   

   g = readGraph(argv[1]);
   int s = g.size();
   menuReturnValue = -1;

   int** iMat = nullptr;  // Declare outside the switch statement

   while (menuReturnValue != 0)
   {
      menuReturnValue = menu();
      switch(menuReturnValue)
      {
            case 1:
            cout << endl;
            iMat = genIncMatUDG(g);
            if (iMat == nullptr)
            {
                cout << "The graph in " << argv[1] << " has no edges." << endl;
            }
        
            else
            {
                cout << "|V| x |E| {-1,0,1}-Incidence Matrix of g in " << argv[1] << endl;
                cout << "--------------------------------------------------------" << endl;
                for (i = 0; i < g.size(); i++)
                {
                    for (j = 0; j < g.countEdges(); j++)
                    {
                        cout << setw(3) << iMat[i][j];
                    }
                    cout << endl;
                }
            }
            cout << endl;
            for (i = 0; i < (int)g.size(); i++)
            {
                delete[] iMat[i];
            }
            delete[] iMat;
            break;

           
          case 2: //breadth-first-search/depth-first-search traversal of G
          {
              cout<<endl;
              cout<<"Breadth-First-Search Traversal of the Graph In "<<argv[1]<<endl;
              cout<<"==============================================================="<<endl;
              //invoke the bfsTraverse function
              // Output should be aligned in two-column format as illustrated below:
              // 1     Charlottetown
              // 4     Halifax
              // 2     Edmonton

            g.bfsTraverse(f); 
         
              cout<<"==============================================================="<<endl;            
              cout<<endl;
              cout<<"Depth-First-Search Traversal of the Graph In "<<argv[1]<<endl;
              cout<<"==============================================================="<<endl;
              //invoke the dfsTraverse function
              // Output should be aligned in two-column format as illustrated below:
              // 1     Charlottetown
              // 4     Halifax
              // 2     Edmonton            
            g.dfsTraverse(f);
            
              cout<<"==============================================================="<<endl;
              cout<<endl;
              cout<<endl;
          }
          break;   
          case 3: //Check Strong Connectivity of G
          {
              cout<<endl;
              cout<<"The digraph "<<argv[1]<<" is ";
              if (isStronglyConnected(g))
              {
                  cout<<"strongly connected."<<endl;
              }
              else
              {
                  cout<<"not strongly connected."<<endl;
              }
              cout<<endl<<endl;
          }
          break;
          case 4:
{
    cout << endl;
    int* top = nullptr;
    
        if (topSortOutDeg(g, top))
        {
            cout << "Topological Sorting of The Graph In " << argv[1] << endl;
            cout << "===============================================================" << endl;
            // Write code to display the topological listing of the vertices
            // in the top array. The should be displayed in two columns
            // format. For example:
            // 6   Redwood City
            // 15  Hayward
            // 14  Dublin
            // 13  San Lorenzo
            // :   :
            // :   :
           for (int i = 0; i < g.size(); ++i) 
           {
                City city = g.retrieveVertex(City(top[i]));
                cout << top[i] << "\t" << city.getLabel() << endl;

            }
            cout << "===============================================================" << endl;
        }
        else
            cout << "No topological ordering possible. The digraph in " << argv[1] << " contains a directed cycle." << endl;
        delete[] top;
        cout << endl << endl; 
}
break;
          case 5: //kruskalMST
          {
              cout<<endl<<"The Minimum Spanning Tree of the graph in "<<argv[1]<<"."<<endl;
              int edgesInMST = 0;
              int *mst = new int[(int)g.size()];
              double totalWeight = kruskalMST(g,mst);
              string cityName1, cityName2;
              cout<<endl;
              for (i=1; i<=g.size(); i++)
              {
                  if (mst[i-1] < 1)
                      cityName1 = "NONE";
                  else
				  {
					  edgesInMST++;
                      cityName1 = g.retrieveVertex(City(mst[i-1])).getLabel();
				  }
                  cityName2 = g.retrieveVertex(City(i)).getLabel();                  
                  cout<<i<<"-("<<trim(cityName2)<<")"<<" parent["<<i<<"] <- "<<mst[i-1]<<" ("<<trim(cityName1)<<")"<<endl;
              }
              cout<<endl<<"The weight of the minimum spanning tree/forest is "<<fixed<<setprecision(2)<<totalWeight<<" miles."<<endl;
			  cout<<"Spanning Tree Edge Density is "<<fixed<<setprecision(2)<<100.0*edgesInMST/g.countEdges()<<"%."<<endl<<endl;
              delete[] mst;
          }
      }
   }
   return 0;
}

/**
 * Removes leading and trailing whitespace characters from the specified string
 * @param text a string
 * @return the specified string without leading and trailing whitespace characters
 */
string trim(string text)
{
    const auto startIndex = text.find_first_not_of(" \t\r\n");
    if (startIndex == string::npos) text = "";
    const auto endIndex = text.find_last_not_of(" \t\r\n");
    text = text.substr(startIndex, endIndex - startIndex+1);
    return text;
}

/**
   This function reads a text file formatted as described in the project 
   description.
   @param filename the name of the DIMACS formatted graph file.
   @return an instance of a graph.
*/
Graph<City> readGraph(string filename)
{
   fstream inFile;
   char temp;
   string tmp;
   int key,v1, v2, size=0, nEdges=0;
   double weight;
   inFile.open(filename.c_str());
   if (inFile.fail())
   {
      cerr<<"Error opening "<<filename<<endl;
      exit(1);
   }
   Graph<City> newGraph;
   while (inFile >> temp)
   {
      if (temp == 'p')
      {
         inFile>>size;
         inFile>>nEdges;
      }
      if (temp == 'c')
      {
         getline(inFile,tmp);
      }
      if (temp == 'n')
      {
         inFile>>key;
         getline(inFile,tmp);
         newGraph.insertVertex(City(key,tmp.c_str()));
      }

      if (temp == 'e')
      {
         inFile>>v1;
         inFile>>v2;
         inFile>>weight;
         newGraph.insertEdge(City(v1),City(v2),weight);
      }
   }
   inFile.close();
   return newGraph;
}

/**
   This function displays the menu interface for the weighted
   digraph application.
   @return an integer representing the menu option selected.
*/
int menu()
{
   string option; 
   do
   {
      cout<<"  BASIC WEIGHTED GRAPH APPLICATION   "<<endl;
      cout<<"======================================================="<<endl;
      cout<<"[1] Generate {-1,0,1} Incidence Matrix of G"<<endl;
      cout<<"[2] BFS/DFS Traversal of G"<<endl;
      cout<<"[3] Check Strong Connectedness of G"<<endl;
      cout<<"[4] Topological Ordering of V(G)"<<endl;
      cout<<"[5] Kruskal's Minimum Spanning Tree/Forest in G"<<endl;
      cout<<"[0] Quit"<<endl;
      cout<<"======================================================="<<endl;
      cout<<"Select an option: ";
      getline(cin,option);
      option = trim(option);
      if (option < "0" || option > "5")
         cout<<"Invalid option...Try again"<<endl<<endl;
      else
         return atoi(option.c_str());      
   }while(true);
}

/**
 *  Generates a {-1,0,1} vertices x edges incidence matrice for a
 * digraph; the columns representing the edges of the graph are arranged 
 * in lexicographical order, source=1 -> destination=-1 (source,destination).
 * @param g a digraph
 * @return a |V| x |E| 2-D array representing the {-1,0,1} incidence matrix
 * of g such that m[i-1,e-1] = 1 and m[j-1,e-1] = -1 if there is an outgoing
 * edge from vertex i to vertex j representing the eth edge (i->j) in g when the
 * edges are arranged in lexicographical order (source,destination); all otherwise
 * entries in a column is 0 when there isn't an incident edge. The null reference is
 * returned when g contains no edges.
 */
int** genIncMatUDG(const Graph<City>& g)
{
    int numVertices = g.size();
    int numEdges = g.countEdges();

    if (numEdges == 0)
    {
        return nullptr;
    }

    int** incidenceMatrix = new int*[numVertices]; //Dynamically allocates memory for 2D array
    for (int i = 0; i < numVertices; ++i) {
        incidenceMatrix[i] = new int[numEdges](); //Dynamically allocate memory for each row
    }

    // Map vertices to indices 
    vector<City> vertexList;
    for (int i = 1; i <= numVertices; ++i) {
        vertexList.push_back(g.retrieveVertex(i));
    }

    vector<pair<int, int>> edges;
    for (int i = 1; i <= numVertices; ++i) {
        for (int j = 1; j <= numVertices; ++j) {
            if (g.isEdge(City(i), City(j))) {
                edges.push_back(make_pair(i, j));
            }
        }
    }
    sort(edges.begin(), edges.end());


    // Fill in the incidence matrix
    int edgeIndex = 0;
    for (int i = 1; i <= numVertices; ++i) {
        for (int j = 1; j <= numVertices; ++j) {
            if (g.isEdge(vertexList[i - 1], vertexList[j - 1])) //If there is an edge, run this
            {
                incidenceMatrix[i - 1][edgeIndex] = 1; //Outgoing edge
                incidenceMatrix[j - 1][edgeIndex] = -1; //Incoming edge
                ++edgeIndex; //Moves to next column
            }
        }
    }

    return incidenceMatrix;
}

/**
 * Determines whether the specified digraph is strongly connected
 * @param g a digraph
 * @return true if the digraph is strongly connected; otherwise, false
 */
bool isStronglyConnected(const Graph<City>& g)
{
    if (g.isEmpty() || g.size() < 2) {
        return false;
    }

    Graph<City>& nonConstGraph = const_cast<Graph<City>&>(g);
    int startVertex = 1; // Assuming we can start from the first vertex

    // First traversal: Out-directed DFS
    vector<bool> visited(g.size(), false);
    nonConstGraph.dfsTraverse([&](const City& c) {
        visited[c.getKey() - 1] = true;
    });

    // Check if all vertices were visited in the first traversal
    if (any_of(visited.begin(), visited.end(), [](bool v) { return !v; })) {
        return false;
    }

    // Second traversal: Simulating In-directed DFS
    visited.assign(g.size(), false);
    function<void(int)> dfsSimulateReverse;
    dfsSimulateReverse = [&](int vertex) {
        visited[vertex - 1] = true;
        for (int i = 1; i <= g.size(); ++i) {
            if (!visited[i - 1] && g.isEdge(City(i), City(vertex))) {
                dfsSimulateReverse(i);
            }
        }
    };
    dfsSimulateReverse(startVertex);

    // Check if all vertices were visited in the simulated reverse DFS
    return all_of(visited.begin(), visited.end(), [](bool v) { return v; });
}

/**
 * Finds the root vertex of the tree in which the specified is
 * @param parent the parent implementation of disjointed subtrees of a graph
 * @param v a vertex
 * @return the root of the subtree containing v or v if v is a root.
 */
int find(int parent[], int v)
{
    while (true) {
        if (parent[v - 1] == -1) {
            return v;
        }

        v = parent[v - 1];
    }
}
// implement this function

/**
 * Merges two trees in a forest where (i,j) is a bridge between
 * trees.
 * @param i a vertex in one tree (source)
 * @param j a vertex in another tree (destination)
 * @param parent the parent-array representation of a forest
 * where the parent of a root is -1
 */
void trUnion(int i, int j, int parent[])
{
    std::stack<int> stack;

    int tmp = j;
    while (tmp != -1) {
        stack.push(tmp);
        tmp = parent[tmp - 1];
    }

    while (!stack.empty()) {
        int source = stack.top();
        stack.pop();

        if (!stack.empty()) {
            int destination = stack.top();
            parent[source - 1] = destination;
        } else {
            parent[source - 1] = i;
        }
    }
}
// implement this function

/**
 * This method generates a minimum spanning tree using Kruskal's 
 * algorithm. If no such MST exists, then it generates a minimum spanning forest.
 * @param g a weighted directed graph
 * @param parent the parent implementation of the minimum spanning tree/forest
 * @return the weight of such a tree or forest.
 * @throws GraphException when this graph is empty
 * <pre>
 * {@code
 * If a minimum spanning tree cannot be generated,
 * the parent implementation of a minimum spanning tree or forest is
 * determined. This implementation is based on the union-find stragey.
 * }
 * </pre>
 */  
double kruskalMST(const Graph<City>& g, int* parent)
{
    class EdgeType {
    public:
        int source;
        int destination;
        bool chosen;
        double weight;

        EdgeType(int src, int dest, double weight) : source(src), destination(dest), weight(weight), chosen(false) {}
    };

    
        auto greaterThan = [&g](const EdgeType& e1, const EdgeType& e2) -> bool {
            double weight1 = g.retrieveEdge(City(e1.source), City(e1.destination));
            double weight2 = g.retrieveEdge(City(e2.source), City(e2.destination));

            if (weight1 > weight2)
                return true;
            if (weight1 < weight2)
                return false;
            if (e1.source > e2.source)
                return true;
            if (e1.source < e2.source)
                return false;
            if (e1.destination > e2.destination)
                return true;
            if (e1.destination < e2.destination)
                return false;
            return false;
        };

         
    std::priority_queue<EdgeType, std::vector<EdgeType>, decltype(greaterThan)> edgeQueue(greaterThan);

    
    for (int i = 1; i <= g.size(); i++) 
    {
        parent[i - 1] = -1;
    }

    
    for (int i = 1; i <= g.size(); ++i) 
    {
        for (int j = 1; j <= g.size(); ++j) 
        {
            if (i != j && g.isEdge(City(i), City(j))) 
            {
                double weight = g.retrieveEdge(City(i), City(j));
                edgeQueue.push(EdgeType(i, j, weight));
            }
        }
    }

    double totalWeight = 0.0;

    
    while (!edgeQueue.empty())  
    {
        EdgeType edge = edgeQueue.top();
        edgeQueue.pop();
        int sourceRoot = find(parent, edge.source);
        int destinationRoot = find(parent, edge.destination);

        if (sourceRoot != destinationRoot) 
        {
            trUnion(edge.source, edge.destination, parent);
            totalWeight += edge.weight;
            
        }
    }

    return totalWeight;
}

/**
 * Generates a topological labeling of the specified digraph, in reverse order,
 *  using the decrease-and-conquer algorithm that successively selects and r
 * emoves a vertex of out-degree 0 until all the vertices are selected.
 * The graph is explored in lexicographical order when adding a new vertex to the 
 * topological ordering and the graph is not modified. Updates of the degrees
 * and vertices that are selected are tracked using auxiliary data structures.
 * @param g a digraph
 * @param linearOrder a pointer to an externally dynamically allocated array
 * to store the topological ordering of the vertices
 * @return true if a topological ordering of the vertices of the specified digraph
 * exists; otherwise, false.
 */
bool topSortOutDeg(const Graph<City> &g, int*& linearOrder) {
    if (g.isEmpty()) {
        return false;
    }

    int numVertices = g.size();
    std::vector<int> outDegree(numVertices, 0);
    int index = numVertices - 1;
    int exitVertex = -1;

    // Initialize out-degree vector
    for (int i = 1; i <= numVertices; ++i) {
        City city(i); // Assuming City constructor uses an int identifier
        outDegree[i - 1] = g.outDegree(city);
    }

    linearOrder = new int[numVertices];

    for (int i = 1; i <= numVertices; i++) {
        if (outDegree[i - 1] == 0) {
            exitVertex = i;
            linearOrder[index--] = exitVertex;

            // Decrease out-degree of adjacent vertices
            for (int j = 1; j <= numVertices; ++j) {
                if (g.isEdge(City(j), City(exitVertex))) {
                    outDegree[j - 1]--;
                }
            }
            outDegree[i - 1] = -1; // Mark as processed
            i = 0; // Restart the loop to check all vertices again
        }
    }

    if (index >= 0) {
        delete[] linearOrder; // Clean up if cycle detected
        linearOrder = nullptr;
        return false; // Directed cycle detected, topological sort not possible
    }

    
    return true;
}