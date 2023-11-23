/**
 * Describe the a weighted digraph and its fundamental operations
 * @param <E> the data type
 * @author Duncan
 * <pre>
 * Date: 99-99-9999
 * Course: csc 3102
 * Instructor: Dr. Duncan
 * Programming Project # 3
 * Note: DO NOT MODIFY THIS FILE
 * <pre>
 */

#include <string>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <functional>

using namespace std;

#ifndef GRAPH_H
#define GRAPH_H

using namespace std;

/**
 *  exception class 
 */
class GraphException
{
public:
    /**
     * constructor
     * @param aMessage description of why the exception occurred
     */
   GraphException(const string& aMessage)
   {
      message = aMessage;
   }
   /**
    * Give the reason the exception occurred
    * @return reason the exception occurred
    */
   string what() const
   {
      return message;
   }
private:
    /**
     * Information about the exception
     */
   string message;
};

template <typename E>
class Graph
{
private:
    /* nested vertex class */
    class Edge;
    class Vertex
    {  
    public:
        /**
         * constructs a vertex with a given data value.
         * @param s the data to store in the vertex
         */
        Vertex(E s);
    private:
        /**
         * A pointer to the next vertex
         */
        Vertex* pNextVertex;
        /**
         * the item stored in this node
         */
        E data;
        /**
         * the in-degree
         */
        int inDeg;
        /**
         * the out-degree
         */
        int outDeg;
        /**
         * A pointer to the edge list of this vertex
         */
        Edge* pEdge;
        /**
         * An auxiliary variable used for algorithms that mark this vertex
         * as having been accessed/visited
         */
        int processed;
        /**
         * To grant the Graph class access to private members
         */
        friend class Graph<E>;
    };
    /* Nested Edge class*/
    class  Edge
    {
    public:
        /**
         * constructs an Edge with a given weight.
         * @param s the data to store in the vertex
         */
        Edge(double wt);
    private:
        /**
         * The destination of this edge
         */
        Vertex* destination;
        /**
         * The weight on this edge
         */
        double weight;
        /**
         * A pointer to the next edge on the edge list
         */
        Edge* pNextEdge;
        /**
         * To grant the Graph class access to private members
         */
        friend class Graph<E>;
    };
    /**
     * The number of vertices in this graph
     */
    int order;
    /**
     * A pointer to the first vertex
     */
    Vertex* first;    
    /**
     * A trichotomous integer-value comparator lambda function; that is,
     * it compares two elements of this graph and returns a negative
     * integer when the first is less than the second; 0, when they are equal;
     * otherwise, a positive integer  * 
    */
    std::function<int(E,E)> cmp = nullptr;       
public:
   /**
    * constructs an empty weighted digraph.
   */
   Graph();
   
   /**
    * A parameterized constructor    
    * @param fn - an integer-value binary comparator function   
    */
   Graph(std::function<int(E,E)> fn);       

   /**
    * destructor - returns the weighted digraph memory to the system.
   */
   ~Graph() noexcept;
   
   /**
    * Copy constructor
    * @param other an graph instance
    */
   Graph(const Graph<E>& other);
   
   /**
    * Move constructor
    * @param other a graph instance
    */
   Graph(Graph<E>&& other) noexcept;
   /**
    * Overloaded assignment operator
    * @param other a graph instance
    * @return a reference to a new graph instance equivalent to
    * the specified graph instance
    */
   Graph<E>& operator= (const Graph<E>& other);	

   /**
    * Overloaded move assignment operator
    * @param other a graph instance
    * @return a graph instance equivalent to the previous
    * specified graph instance
    */
   Graph<E>& operator= (Graph<E>&& other) noexcept;
   
   /**
    * This function inserts a new vertex whose data item is data in the
    * weighted graph. If the key of the data already exists the vertex
    * is updated. No two vertices may have the same key.
    * @param data - data stored in a vertex.
    */
   void insertVertex(E data);

   /**
    * This function deletes a vertex whose data item is data from the
    * weighted digraph. If the key of the data does not exist or the
    * indegree or out degree is positive, the weighted graph remains 
    * unaltered. It the data item exists and the in-degree
    * and out-degree are both 0, the vertex is removed from the graph.
    * @param key - data stored in a vertex. 
    */
   void deleteVertex(E key); 

   /**
    * This function inserts a weighted directed edge between two vertices.
    * If either key does not exist, the graph remains unaltered. If there
    * is already a directed edge between the vertices, its weight is
    * updated. If both keys exist and there isn't an edge between the   
    * vertices, an edge is inserted and the indegree and outdegree of
    * both vertices are updated.
    * @param fromKey - data of the originating vertex.
    * @param toKey - data of the destination vertex.
    * @param weight - weight of the edge between the from and to vertices.
   */
   void insertEdge(E fromKey, E toKey, double weight);

   /**
    * This function removes a weighted directed edge between two vertices.
    * If either key does not exist, the function returns and the graph
    * remains unaltered. If the edge exists, it is removed from the vertex
    * with fromKey to the vertex with toKey. The indegree and outdegree
    * of both vertices are updated.
    * @param fromKey - search key of the originating vertex.
    * @param toKey - search key of the destination vertex.
    */
   void deleteEdge(E fromKey, E toKey);

   /**
    * This function returns the weight of the directed edge between   
    * the vertices with fromKey and toKey if the edge exists. If
    * the directed edge does not exist, an exception is generated.
    * @param fromKey - search key of the originating vertex.
    * @param toKey - search key of the destination vertex.  
    * @throw GraphException if the edge does not exist
    * @return the weight on the edge.
    */
   double retrieveEdge(E fromKey, E toKey) const;

   /**
    * This function returns the item stored in the vertex whose key  
    * is key. If the key does not exist, an exception is generated.
    * @param key - search key of the vertex.
    * @throw GraphException if the vertex does not exist
    * @return the data value store in a vertex.
    */
   const E& retrieveVertex(const E& key) const;
      
   /**
    * This function applies the visit function to the vertices of  
    * the graph in breadth-first-search order.
    * @param func - the function that is applied to the data in each
    * node during a traversal of this graph
    */
   void bfsTraverse(std::function<void(const E&)> ffunc);

   /**
    * This function applies the visit function to the vertices of
    * the graph in postorder depth-first-search order.
    * @param func - the function that is applied to the data in each
    * node during a traversal of this graph
    */
   void dfsTraverse(std::function<void(const E&)> func);

   /**
    * Determines whether the weighted digraph is empty.
    * @return this function returns true if the digraph is empty;
    * otherwise, it returns false if the graph contains at least one vertex.
    */
   bool isEmpty() const;

   /**
    * Returns the order of graph.
    * @return the number of vertices in the graph
    */
   int size() const;

   /**
    * Determines whether an item is in the graph.
    * @param key search key of the vertex.
    * @return true on success; false on failure.
    */ 
   bool isVertex(E key) const;

   /**
    * Determines whether there is a directed edge between two vertices.
    * @param fromKey - search key of the originating vertex.
    * @param toKey - search key of the destination vertex.
    * @return true on success or false on failure.
    */
   bool isEdge(E fromKey, E toKey) const;

   /**
    * Determines the number of edges in the graph.
    * @return the number of edges.
    */
   int countEdges() const;

   /**
    * Determines the number of out-directed edges from the vertex with the key.
    * @param key item stored in the vertex
    * @return out-degree.
    * @throw GraphException if the vertex does not exist
    */
   int outDegree(E key) const;

   /**
    * Determines the number of in-directed edges from the vertex with the key.
    * @param key - item stored in the vertex
    * @return in-degree.
    * @throws GraphException if the vertex does not exist
    */
   int inDegree(E key) const;
};
//GRAPH_H
#endif

