/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #5
 |    Propose:  Network Planning
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/5/26 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++
 |    Updated:  2019/5/14 23:36
 *---------------------------------------------------*/
#include <iostream>
using namespace std;

int FindMin(int x, int y) {if (x < y) return x; else return y;}
struct Edge {
    int _id = -1;
    int _latency = -1;
    int _bandwidth = -1;
    Edge* _next = NULL;
};
class Graph {
private:
    Edge** _network;
    int _num_of_vertex;
    int* _all_latency;
    int* _all_bandwidth;
    bool* _done;
public:
    Graph(int length);
    ~Graph();
    void CreateEdge(int, int, int, int);
    void FindMaxBandwidth(int);
    void FindMinLatency(int);
    void ResetDone() {for (int i=0; i<_num_of_vertex; i++) _done[i] = false;}
    void PrintResult();
};
Graph::Graph(int x) {
    _num_of_vertex = x;
    _network = new Edge* [x];
    _all_latency = new int[x];
    _all_bandwidth = new int[x];
    _done = new bool[x];
    for (int i=0; i<x; i++) {
        _network[i] = NULL;
    }
}
Graph::~Graph() {
    delete [] _network;
    delete [] _all_latency;
    delete [] _all_bandwidth;
    delete [] _done;
}
void Graph::CreateEdge(int x, int y, int l, int b) {
    if (_network[x] == NULL) {
        Edge* _edge = new Edge();
        _edge->_id = y;
        _edge->_latency = l;
        _edge->_bandwidth = b;
        _network[x] = _edge;
    } else {
        Edge* _cur = _network[x];
        while (_cur->_next != NULL) {
            _cur = _cur->_next;
        }
        Edge* _edge = new Edge();
        _edge->_id = y;
        _edge->_latency = l;
        _edge->_bandwidth = b;
        _cur->_next = _edge;
    }
}
void Graph::PrintResult() {
    for (int i=1; i<_num_of_vertex; i++) {
        if (_all_latency[i] == 100)
            cout << i << " " << "inf" << " " << _all_bandwidth[i] << endl;
        else
            cout << i << " " << _all_latency[i] << " " << _all_bandwidth[i] << endl;
    }
}
void Graph::FindMaxBandwidth(int t) {
    for (int i=0; i<_num_of_vertex; i++) {
        _all_bandwidth[i] = 0;
    }
    Edge* _cur = _network[t];
    while (_cur->_next != NULL) {
        _all_bandwidth[_cur->_id] = _cur->_bandwidth;
        _cur = _cur->_next;
    }
    _all_bandwidth[_cur->_id] = _cur->_bandwidth;
    
    _done[t] = true;
    
    for (int i=0; i<_num_of_vertex-1; i++) {
        int max_bw = -1;
        int max_idx = -1;
        for (int j=1; j<_num_of_vertex; j++) {
            if (_done[j] == false && _all_bandwidth[j] > max_bw) {
                max_bw = _all_bandwidth[j];
                max_idx = j;
            }
        }
        _done[max_idx] = true;
        if (_network[max_idx] != NULL) {
            _cur = _network[max_idx];
            while (_cur != NULL) {
                if (_done[_cur->_id] == false && FindMin(_all_bandwidth[max_idx], _cur->_bandwidth) > _all_bandwidth[_cur->_id])
                    _all_bandwidth[_cur->_id] = FindMin(_all_bandwidth[max_idx], _cur->_bandwidth);
                _cur = _cur->_next;
            }
        }
    }
}
void Graph::FindMinLatency(int t) {
    for (int i=0; i<_num_of_vertex; i++) {
        _all_latency[i] = 100;
    }
    Edge* _cur = _network[t];
    while (_cur->_next != NULL) {
        _all_latency[_cur->_id] = _cur->_latency;
        _cur = _cur->_next;
    }
    _all_latency[_cur->_id] = _cur->_latency;
    
    _done[t] = true;
    
    for (int i=0; i<_num_of_vertex-1; i++) {
        int min_lat = 200;
        int min_idx = -1;
        for (int j=1; j<_num_of_vertex; j++) {
            if (_done[j] == false && _all_latency[j] < min_lat) {
                min_lat = _all_latency[j];
                min_idx = j;
            }
        }
        
        _done[min_idx] = true;
        
        if (_network[min_idx] != NULL) {
            _cur = _network[min_idx];
            while (_cur != NULL) {
                if (_done[_cur->_id] == false && (_all_latency[min_idx] + _cur->_latency) < _all_latency[_cur->_id])
                    _all_latency[_cur->_id] = _all_latency[min_idx] + _cur->_latency;
                _cur = _cur->_next;
            }
        }
    }
}
int main(int argc, const char * argv[]) {
    int num_of_building;
    cin >> num_of_building;
    int num_of_connection;
    cin >> num_of_connection;
    Graph MyGraph(num_of_building);
    int src = 0;
    int dst = 0;
    int lat = 0;
    int bw = 0;
    for (int i=0; i<num_of_connection; i++) {
        cin >> src >> dst >> lat >> bw;
        MyGraph.CreateEdge(src, dst, lat, bw);
    }
    MyGraph.ResetDone();
    MyGraph.FindMaxBandwidth(0);
    MyGraph.ResetDone();
    MyGraph.FindMinLatency(0);
    MyGraph.PrintResult();
    return 0;
}
