/** 
 * Implementation file for  a pointer-based weighted digraph
 * @param <E> the data type of the graph; the less than operator must be
 * defined on the concrete type for E
 * @author Duncan, YOUR NAME
 * @see Graph.h
 * <pre>
 * Date: 99-99-9999
 * CSC 3102 Programming Project # 3
 * Instructor: Dr. Duncan 
 *
 * DO NOT REMOVE THIS NOTICE (GNU GPL V2):
 * Contact Information: duncanw@lsu.edu
 * Copyright (c) 2023 William E. Duncan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
 * </pre>
 */

#include "Graph.h"
#include <queue>
#include <stack>
#include <cstdlib>

using namespace std;

/* nested Vertex class definitions */

template<typename E>
Graph<E>::Vertex::Vertex(E s)
{
   pNextVertex = NULL;
   data = s;
   inDeg = 0;
   outDeg = 0;
   pEdge = NULL;
   processed = 0;
}

/* nested Edge class definitions */
template<typename E>
Graph<E>::Edge::Edge(double wt)
{
   destination = NULL;
   weight = wt;
   pNextEdge = NULL;
}

/* Outer Graph class definitions */
template <typename E>
Graph<E>::Graph()
{
   first = NULL;
   order = 0;
   cmp = [](E a, E b) -> int{return a < b? -1 : (a == b? 0 : 1);};      
}

template <typename E>
Graph<E>::Graph(std::function<int(E,E)> fn)
{
   first = NULL;
   order = 0;
   if (cmp == nullptr) 
        cmp = fn;
}

template <typename E> 
Graph<E>::~Graph() noexcept
{
   Edge* edgePtr;
   Vertex* vertexPtr;
   vertexPtr = first;
   while (first)
   {
      vertexPtr = first;
      while(vertexPtr->pEdge)
      {
         edgePtr = vertexPtr->pEdge;
         vertexPtr->pEdge = edgePtr->pNextEdge;
         delete edgePtr;
      }
      first = vertexPtr->pNextVertex;
      delete vertexPtr;
   }
}

template <typename E>
Graph<E>::Graph(const Graph<E>& other)
{
    first = NULL;
    order = 0;
    cmp = other.cmp;
    Vertex* tmp = other.first;
    Edge* edgPtr = NULL;
    while (tmp)
    {
        insertVertex(tmp->data);
        edgPtr = tmp->pEdge;
        while (edgPtr)
        {
            insertEdge(tmp->data,edgPtr->destination->data, edgPtr->weight);
            edgPtr = edgPtr->pNextEdge;
        }
    }        
}

template <typename E>
Graph<E>::Graph(Graph<E>&& other) noexcept
{
    first = other.first;
    order = other.order;
    cmp = other.cmp;
    other.first = nullptr;
    other.order = 0;   
    other.cmp = nullptr;
}

template <typename E>
Graph<E>& Graph<E>::operator=(const Graph<E>& other)
{
    if (this != &other)
    {
        first = NULL;
        order = 0;
        cmp = other.cmp;
        Vertex* tmp = other.first;
        Edge* edgPtr = NULL;
        while (tmp)
        {
            insertVertex(tmp->data);
            edgPtr = tmp->pEdge;
            while (edgPtr)
            {
                insertEdge(tmp->data,edgPtr->destination->data,edgPtr->weight);
                edgPtr = edgPtr->pNextEdge;
            }
        }  
    }
    return *this;
}

template <typename E>
Graph<E>& Graph<E>::operator =(Graph<E>&& other) noexcept
{
    if (this != &other)
    {
        Edge* edgePtr;
        Vertex* vertexPtr;
        vertexPtr = first;
        while (first)
        {
            vertexPtr = first;
            while(vertexPtr->pEdge)
            {
                edgePtr = vertexPtr->pEdge;
                vertexPtr->pEdge = edgePtr->pNextEdge;
                delete edgePtr;
            }
            first = vertexPtr->pNextVertex;
            delete vertexPtr;
        }
        first = other.first;
        order = other.order;
        cmp = other.cmp;
        other.first = nullptr;
    }
    return *this;
}

template <typename E> 
void Graph<E>::insertVertex(E obj)
{
   assert(isVertex(obj) == false);
   Vertex* locPtr = first;
   Vertex* predPtr = NULL;
   while (locPtr != NULL && cmp(obj,locPtr->data) > 0)
   {
      predPtr = locPtr;
      locPtr = locPtr->pNextVertex;
   }
   /*key already exist. */
   if (locPtr != NULL && cmp(obj,locPtr->data)==0)
   {
      locPtr->data = obj;
      return;
   }
   /* insert before first vertex */
   Vertex* newPtr = new Vertex(obj);
   newPtr->pNextVertex = NULL;
   newPtr->data = obj;
   newPtr->inDeg = 0;
   newPtr->outDeg = 0;
   newPtr->processed = 0;
   newPtr->pEdge = NULL;   
   if (predPtr == NULL)
      first = newPtr;
   else
      predPtr->pNextVertex = newPtr;
   newPtr->pNextVertex = locPtr;
   order++;
}

template <typename E> 
void Graph<E>::deleteVertex(E key)
{
   if (isEmpty())
      return;
   Vertex* predPtr = NULL;
   Vertex* walkPtr = first;  
   Vertex* tmp;
   /* does vertex exist */
   while (walkPtr != NULL && cmp(key,walkPtr->data) > 0)
   {  
      predPtr = walkPtr;
      walkPtr = walkPtr->pNextVertex;
   }
   if (walkPtr == NULL || cmp(key,walkPtr->data) != 0)
      return;
   /* vertex found. Test degree */
   if ((walkPtr->inDeg > 0) || (walkPtr->outDeg > 0))
      return;
   /* OK to delete */
   if (predPtr == NULL)
   {
      tmp = first;
      first = walkPtr->pNextVertex;
      delete tmp;
   }
   else
   {
      tmp = predPtr->pNextVertex;
      predPtr->pNextVertex = walkPtr->pNextVertex;
      delete tmp;
   }
   order--;
}

template <typename E> 
void Graph<E>::insertEdge(E fromKey, E toKey, double weight)
{

   if (isEmpty())
      return;
   /* check whether originating vertex exists */
   Vertex* tmpFrom = first;
   while (tmpFrom != NULL && cmp(fromKey,tmpFrom->data) > 0)
      tmpFrom = tmpFrom->pNextVertex;
   if (tmpFrom == NULL || cmp(fromKey,tmpFrom->data) != 0)
      return; 
   /* locate destination vertex */
   Vertex* tmpTo = first;
   while (tmpTo != NULL && cmp(toKey,tmpTo->data) > 0)
      tmpTo = tmpTo->pNextVertex;
   if (tmpTo == NULL || cmp(toKey,tmpTo->data) != 0)
      return;
   /*check if edge already exists */
   Edge* tmpEdge = tmpFrom->pEdge;
   while (tmpEdge != NULL && tmpEdge->destination != tmpTo)
      tmpEdge = tmpEdge->pNextEdge;
   if (tmpEdge != NULL && tmpEdge->destination != NULL)
      return;
   tmpFrom->outDeg++;
   tmpTo->inDeg++;
   Edge* newEdge = new Edge(weight);
   newEdge->destination = tmpTo;
   newEdge->weight = weight;
   newEdge->pNextEdge = NULL;
   if (tmpFrom->pEdge == NULL)
   {
      tmpFrom->pEdge = newEdge;
      return;
   }
   Edge* pred = NULL;
   tmpEdge = tmpFrom->pEdge;
   while (tmpEdge != NULL && tmpEdge->destination != tmpTo
   &&cmp(newEdge->destination->data,tmpEdge->destination->data)>0)
   {
      pred = tmpEdge;
      tmpEdge = tmpEdge->pNextEdge;
   }
   if (pred == NULL)
      tmpFrom->pEdge = newEdge;
   else
     pred->pNextEdge = newEdge;
   newEdge->pNextEdge = tmpEdge;
}

template <typename E> 
void Graph<E>::deleteEdge(E fromKey, E toKey)
{
   /* find source vertex */
   Vertex* tmpFrom = first;
   while (tmpFrom != NULL && cmp(fromKey,tmpFrom->data) > 0)
      tmpFrom = tmpFrom->pNextVertex;
   if (tmpFrom == NULL || cmp(fromKey,tmpFrom->data) != 0)
      return;
   /* locate destination vertex */
   Vertex* tmpTo = first; 
   while (tmpTo != NULL && cmp(toKey,tmpTo->data) > 0)
      tmpTo = tmpTo->pNextVertex;
   if (tmpTo == NULL || cmp(toKey,tmpTo->data) != 0)
      return;
   /*check if edge does not exist */
   Edge* tmpEdge = tmpFrom->pEdge;
   Edge* pred = NULL;
   while (tmpEdge != NULL && tmpEdge->destination != tmpTo)
   {
      pred = tmpEdge;
      tmpEdge = tmpEdge->pNextEdge;
   }
   /* if edge does not exist */
   if (tmpEdge == NULL)
      return;
   /* update degrees */
   pred->pNextEdge = tmpEdge->pNextEdge;
   tmpFrom->outDeg--;
   tmpTo->inDeg--;
   delete tmpEdge;
}

template <typename E> 
double Graph<E>::retrieveEdge(E fromKey, E toKey) const
{
   Edge* tmpEdge;
   Edge* pred;
   Vertex* tmpFrom;
   Vertex* tmpTo;
   /* find source vertex */
   tmpFrom = first;
   while (tmpFrom != NULL && cmp(fromKey,tmpFrom->data) > 0)
      tmpFrom = tmpFrom->pNextVertex;
   if (tmpFrom == NULL || cmp(fromKey,tmpFrom->data) != 0)
      throw GraphException("Non-existent edge - retrieveEdge().");
   /* locate destination vertex */
   tmpTo = first; 
   while (tmpTo != NULL && cmp(toKey,tmpTo->data) > 0)
      tmpTo = tmpTo->pNextVertex;
   if (tmpTo == NULL || cmp(toKey,tmpTo->data) != 0)
      throw GraphException("Non-existent edge - retrieveEdge().");
   /*check if edge does not exist */
   tmpEdge = tmpFrom->pEdge;
   while (tmpEdge != NULL && tmpEdge->destination != tmpTo)
      tmpEdge = tmpEdge->pNextEdge;
   /* if edge does not exist */
   if (tmpEdge == NULL)
      throw  GraphException("Non-existent edge - retrieveEdge().");
   return tmpEdge->weight; 
}

template <typename E> 
const E& Graph<E>::retrieveVertex(const E& key) const
{
   Vertex* tmp;
   if (isEmpty())
      throw GraphException("Non-existent vertex in call to retrieveVertex().");
   tmp = first;
   while (tmp != NULL && cmp(key,tmp->data) != 0)
      tmp = tmp->pNextVertex;
   if (tmp == NULL)
      throw GraphException("Non-existent vertex - retrieveVertex().");
   return tmp->data;
}

template <typename E> 
void Graph<E>::bfsTraverse(std::function<void(const E&)> func)
{
   Vertex* walkPtr;
   Vertex* toPtr;
   queue<Vertex*> q;
   Edge* edgeWalk;  
   Vertex* tmp;
   if (isEmpty())
      return;
   walkPtr = first;
   while (walkPtr != NULL)
   {
      walkPtr->processed = 0;
      walkPtr = walkPtr->pNextVertex;
   }
   walkPtr = first;
   while (walkPtr != NULL)
   {
      if (walkPtr->processed < 2)
      {
         if (walkPtr->processed < 1)
         {
            q.push(walkPtr); 
            walkPtr->processed = 1;
         }
      }
      while (!q.empty())
      {
          tmp = q.front();
          q.pop();
          func(tmp->data);
          tmp->processed = 2;
          edgeWalk = tmp->pEdge; 
          while (edgeWalk != NULL)           
          {
              toPtr = edgeWalk->destination;
              if (toPtr->processed == 0)
              {
                  toPtr->processed = 1;
                  q.push(toPtr);
              }
              edgeWalk = edgeWalk->pNextEdge;
          }
      }
      walkPtr = walkPtr->pNextVertex;
   }
}

template <typename E> 
void Graph<E>::dfsTraverse(std::function<void(const E&)> func)
{
   Vertex* walkPtr;
   Vertex* toPtr;
   stack<Vertex*> s;
   Edge* edgeWalk;  
   Vertex* tmp;
   if (isEmpty())
      return;
   walkPtr = first;
   while(walkPtr != NULL)
   {
      walkPtr->processed = 0;
      walkPtr = walkPtr->pNextVertex;
   }
   walkPtr = first;
   bool hasUnexploredNbrs;
   while (walkPtr != NULL)
   {
      if (walkPtr->processed < 2)
      {
         if (walkPtr->processed < 1)
         {
            walkPtr->processed = 1;  
            s.push(walkPtr);
         }
         while (!s.empty())
         {
            tmp = s.top();
            edgeWalk = tmp->pEdge;
			hasUnexploredNbrs = false;
            while(edgeWalk != NULL)
            {
               toPtr = edgeWalk->destination;
               if (toPtr->processed == 0)
               {
                  toPtr->processed = 1;
                  s.push(toPtr);
                  edgeWalk = toPtr->pEdge;
				  hasUnexploredNbrs = true;
               }
               else
                  edgeWalk = edgeWalk->pNextEdge;
            }
			if (!hasUnexploredNbrs)
			{
			   tmp = s.top();
			   s.pop();
			   func(tmp->data);
			   tmp->processed = 2;
			}
         }
      }
      walkPtr = walkPtr->pNextVertex;
   }
}

template <typename E> 
bool Graph<E>::isEmpty() const
{
   return first == NULL;  
}

template <typename E> 
int Graph<E>::size() const
{
   return order;  
}

template <typename E> 
bool Graph<E>::isVertex(E key) const
{
   Vertex* tmp;
   if (isEmpty()) 
      return false;
   tmp = first;
   while (tmp != NULL && cmp(key,tmp->data) != 0)
      tmp = tmp->pNextVertex;
   if (tmp == NULL)
      return false;
   return true;
}

/***------------------ BEGIN AUGMENTED FUNCTIONS -------------***/

template <typename E>
bool Graph<E>::isEdge(E fromKey, E toKey) const
{
    Vertex* tmpFromKey = first;

    // Find the vertex with the key 'fromKey'
    while (tmpFromKey != nullptr && cmp(fromKey, tmpFromKey->data) != 0)
    {
        tmpFromKey = tmpFromKey->pNextVertex;
    }

    // If the 'fromKey' vertex doesn't exist, return false
    if (tmpFromKey == nullptr)
    {
        return false;
    }

    // Traverse the edges of the 'fromKey' vertex
    Edge* tmpEdge = tmpFromKey->pEdge;
    while (tmpEdge != nullptr)
    {
        // Check if the destination of the edge has the key 'toKey'
        if (tmpEdge->destination != nullptr && cmp(toKey, tmpEdge->destination->data) == 0)
        {
            return true; // Edge found
        }
        tmpEdge = tmpEdge->pNextEdge;
    }

    return false; // Edge not found
}

template <typename E> 
int Graph<E>::countEdges() const
{
   int countEdge = 0;
   Vertex* tmp = first;
   while (tmp != nullptr)
   {
       countEdge += outDegree(tmp->data);
       tmp = tmp->pNextVertex;
   }
   return countEdge;   
}

/***------------------ END AUGMENTED FUNCTIONS -------------***/

template <typename E> 
int Graph<E>::outDegree(E key) const
{
   Vertex* tmp;
   if (isEmpty())
      throw  GraphException("Non-existent vertex - outDegree().");
   tmp = first;
   while (tmp != NULL && cmp(key,tmp->data) != 0)
      tmp = tmp->pNextVertex;
   if (tmp == NULL)
      throw GraphException("Non-existent vertex - outDegree().");
   return tmp->outDeg;
}

template <typename E> 
int Graph<E>::inDegree(E key) const
{
   Vertex* tmp;
   if (isEmpty())
      throw  GraphException("Non-existent vertex - inDegree().");
   tmp = first;
   while (tmp != NULL && cmp(key,tmp->data) != 0)
      tmp = tmp->pNextVertex;
   if (tmp == NULL)
      throw GraphException("Non-existent vertex - inDegree().");
   return tmp->inDeg;
}
