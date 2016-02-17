// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "trailblazer.h"
#include "queue.h"
#include "pqueue.h"
#include "foreach.h"
#include <algorithm>

using namespace std;

Vector<Vertex*> reversePath(Vector<Vertex*>& originalPath){
    Vector<Vertex*> reversePath;
    for(int i = originalPath.size()-1; i >= 0; i--){
        reversePath.push_back(originalPath.get(i));
    }
    return reversePath;
}

Vector<Vertex*> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    Vector<Vertex*> path;
    if(end == start){
        path.add(end);
        return path;
    }

    graph.resetData();
    bool atEnd = false;

    depthSearch(start,end,path,atEnd);

    return reversePath(path);
}

bool depthSearch(Vertex* location, Vertex* end, Vector<Vertex*>& path, bool& atEnd){
    location->setColor(GREEN);
    location->visited = true;

    if(location == end){
        path.add(location);
        cout << "at end" << endl;
        atEnd = true;
        return true;
    }else{
        Set<Edge*> Neighbors = location->edges;
        Set<Edge*>::iterator itr;
        for(itr = Neighbors.begin(); itr != Neighbors.end(); ++itr){
            Edge* edge = *itr;
            Vertex* nextLoc = edge->finish;
            nextLoc->previous = location;
            if(nextLoc->visited != true && atEnd == false){
                depthSearch(nextLoc,end,path,atEnd);
            }
        }
        location->setColor(GRAY);
        if(atEnd)
            path.add(location);
    }
    return false;
}

//still have to add locations to path
Vector<Vertex*> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    Vector<Vertex*> path;
    Queue<Vertex*> unvisited;
    unvisited.enqueue(start);

    while(!unvisited.isEmpty()){
        Vertex* currLoc = unvisited.dequeue();
        currLoc->setColor(GREEN);
        currLoc->visited = true;

        if(currLoc == end){
            getPath(path,currLoc,start);
            break;
        }

        Set<Edge*> neighbors = currLoc->edges;
        Set<Edge*>::iterator itr;
        for(itr = neighbors.begin(); itr != neighbors.end(); ++itr){
            Edge* edge = *itr;
            Vertex* child = edge->finish;
            if(child->visited == false)
                child->previous = currLoc;
                unvisited.enqueue(child);
        }
    }
    return reversePath(path);
}

Vector<Vertex*> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    return runSearch(start,end,false);
}
void getPath(Vector<Vertex*>& path, Vertex* location, Vertex* start){
    path.add(location);
    Vertex* last = location->previous;
    while(last != start){
        //cout << "last is: " << last->toString() << endl;
        path.add(last);
        last = last->previous;
    }
    path.add(last);
}

Vector<Vertex*> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    return runSearch(start,end,true);
}

Vector<Vertex*> runSearch(Vertex* start, Vertex* end, bool useAStar){
    Vector<Vertex*> path;
    PriorityQueue<Vertex*> toProcess;
    start->setColor(GREEN);
    toProcess.enqueue(start,0);

    while(!toProcess.isEmpty()){
        Vertex* currLoc = toProcess.dequeue();
        currLoc->setColor(GREEN);

        if(currLoc == end){
            getPath(path,currLoc,start);
            break;
        }

        Set<Edge*> neighbors = currLoc->edges;
        Set<Edge*>::iterator itr;
        for(itr = neighbors.begin(); itr != neighbors.end(); ++itr){
            Edge* edge = *itr;
            Vertex* child = edge->finish;
            double candidateDistance;
            if(useAStar){
                candidateDistance = heuristicFunction(child,end);
            }else{
                candidateDistance = edge->cost + currLoc->cost;
            }

            if(child->getColor() == UNCOLORED){
                child->setColor(YELLOW);
                child->previous = currLoc;
                child->cost = candidateDistance;
                toProcess.enqueue(child,candidateDistance);
            }else if(child->getColor() == YELLOW && (child->cost > candidateDistance) ){
                child->cost = candidateDistance;
                child->previous = currLoc;
                toProcess.changePriority(child,candidateDistance);
            }
        }
    }
    return reversePath(path);
}

//kruskal taken from http://www.codingfriends.com/index.php/2010/07/07/graphs-part-2/
//and from erickwill
Set<Edge*> kruskal(BasicGraph& graph) {
    Set<Edge*> mst;

    Set<Edge*> edges = graph.getEdgeSet();
    PriorityQueue<Edge*> edgesPQ;
    Vector< Set<Vertex*> > nodes;
    Set<Edge*> joiningEdges;

    foreach(Edge* edge in edges){
        edgesPQ.add(edge,edge->weight);
        bool inAlready = false;
        for(int i = 0; i < nodes.size(); i++){
            if(nodes[i].contains(edge->start)){
                inAlready = true;
                break;
            }
        }
        if(!inAlready){
            Set<Vertex*> newNode;
            newNode.add(edge->start);
            nodes.add(newNode);
        }
    }

    while(!edgesPQ.isEmpty()){
        Edge* edge = edgesPQ.dequeue();
        int startN, endN;
        startN = endN = -1;
        for(int i = 0; i < nodes.size(); i++){
            if(nodes[i].contains(edge->start))
                startN = i;
            if(nodes[i].contains(edge->finish))
                endN = i;
        }
        if(startN != endN){
            nodes[startN].addAll(nodes[endN]);
            nodes.remove(endN);
            joiningEdges.add(edge);
            mst.add(edge);
        }
    }
    return mst;
}
