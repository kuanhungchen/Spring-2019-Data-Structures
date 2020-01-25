/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #3
 |    Propose:  Bus Route Editor (bonus)
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/4/21 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++
 |    Updated:  2019/4/14 20:30
 *---------------------------------------------------*/
#include <iostream>
using namespace std;

struct path{
    string src_stop;
    string dst_stop;
    path* next_path_addr;
    void swap() {string temp=src_stop; src_stop=dst_stop; dst_stop=temp;}
};

class Pathes{
public:
    Pathes();
    void InsertStop(string src, string dst, string name, int mtd);
    void DeleteStop(string name);
    void BuildPath(string A, string B, string C);
    void PrintRoute();
    void Debug();
    void Display(path* x) {cout<<x->src_stop<<"->"<<x->dst_stop<<" (now: "<<x<<" next: "<<x->next_path_addr<<")"<<endl;};
    void Reverse();
    path* GetFirst() {return first;};
    path* FindPath(string src, string dst);
    path* FromWhichPath(path* x);
    path* ToWhichPath(path* x);
private:
    path* first;
    int mode;
};
Pathes::Pathes(){
    path* _A = new path();
    path* _B = new path();
    _A->src_stop = "NTHU";
    _A->dst_stop = "TSMC";
    _B->src_stop = "TSMC";
    _B->dst_stop = "NTHU";
    _A->next_path_addr = _B;
    _B->next_path_addr = _A;
    
    first = _A;
    mode = 0;  // 0: at least two stops remain; 1: only NTHU remains
}

inline void Pathes::InsertStop(string src, string dst, string name, int mtd) {
    if (FindPath(dst, src) != NULL) {
        if (mtd==1) {
            BuildPath(src, dst, name);
        } else if (mtd==2) {
            BuildPath(src, dst, name);
            BuildPath(dst, src, name);
        }
    } else {
        BuildPath(src, dst, name);
    }
}
inline void Pathes::DeleteStop(string name) {
    if (first->next_path_addr->next_path_addr == first) {
        mode = 1;
    } else {
        path* _target = first;
        while (_target->next_path_addr != first) {
            if (_target->dst_stop == name) {
                path* _next = ToWhichPath(_target);
                path* _last = FromWhichPath(_target);
                path* _next_next = ToWhichPath(_next);
                if (_target->src_stop != _next->dst_stop) {
                    path* _new = new path();
                    _new->src_stop = _target->src_stop;
                    _new->dst_stop = _next->dst_stop;
                    _last->next_path_addr = _new;
                    _new->next_path_addr = _next_next;
                    if (_new->src_stop=="NTHU") { first = _new; }
                    else if (_new->dst_stop=="NTHU") { first = _new->next_path_addr; }
                    _target = first;
                } else {
                    _last->next_path_addr = _next_next;
                    break;
                }
            } else { _target = _target->next_path_addr; }
        }
    }
}
inline void Pathes::Reverse() {
    if (mode == 0) {
        if ( first->next_path_addr->next_path_addr != first) {
            path* _current = first;
            do {
                (*_current).swap();
                _current = _current->next_path_addr;
            } while(_current!=first);
            path* collections[6000];
            int col_idx = 0;
            while (_current->next_path_addr!=first) {
                collections[col_idx] = _current;
                col_idx += 1;
                _current = _current->next_path_addr;
            }
            collections[col_idx] = _current;
            for (int i=col_idx; i>0; i--) {
                collections[i]->next_path_addr = collections[i-1];
            }
            collections[0]->next_path_addr = collections[col_idx];
            first = collections[col_idx];
            
            /* more efficient way but Time Limit Error
            _current = FromWhichPath(first);
            path* tmp = FromWhichPath(first);
            while (_current != first) {
                _current->next_path_addr = FromWhichPath(_current);
                _current = FromWhichPath(_current);
            }
            _current->next_path_addr = tmp;
            first = tmp;
             */
        }
    }
}
inline path* Pathes::FindPath(string src, string dst) {
    path* _target = first;
    while (_target->src_stop!=src || _target->dst_stop!=dst) {
        _target = _target->next_path_addr;
        if (_target == first) {return NULL;}
    }
    return _target;
}
inline path* Pathes::FromWhichPath(path* x) {
    path* _target = first;
    while (_target->next_path_addr != x) {
        _target = _target->next_path_addr;
    }
    return _target;
}
inline path* Pathes::ToWhichPath(path* x) {
    return x->next_path_addr;
}
inline void Pathes::BuildPath(string A, string B, string C) {
    path* _A_to_B = FindPath(A, B);
    path* _to_A = FromWhichPath(_A_to_B);
    path* _B_to = ToWhichPath(_A_to_B);
    path* _A_to_C = new path();
    path* _C_to_B = new path();
    _A_to_C->src_stop = A;
    _A_to_C->dst_stop = C;
    _C_to_B->src_stop = C;
    _C_to_B->dst_stop = B;
    _to_A->next_path_addr = _A_to_C;
    _A_to_C->next_path_addr = _C_to_B;
    _C_to_B->next_path_addr = _B_to;
    if (_A_to_C->src_stop=="NTHU") {first = _A_to_C;}
    else if (_C_to_B->dst_stop=="NTHU") {first = _C_to_B->next_path_addr;}
}
inline void Pathes::PrintRoute() {
    if (mode == 1){
        cout << "NTHU->NTHU" << endl;
    } else {
        path* _current = first;
        do {
            cout << _current->src_stop << "->";
            _current = _current->next_path_addr;
        } while (_current != first);
        cout << "NTHU" << endl;
    }
}
inline void Pathes::Debug() {
    path* _current = first;
    do {
        cout<<_current->src_stop<<"->"<<_current->dst_stop<<" (now: "<<_current<<" next: "<<_current->next_path_addr<<")"<<endl;
        _current = _current->next_path_addr;
    } while (_current != first);
    cout<<"[first: "<<first->src_stop<<"->"<<first->dst_stop<<"]"<<endl;
}


int main() {
    int num_of_ops;
    cin >> num_of_ops;
    string op;
    string src;
    string dst;
    string name;
    int method;
    Pathes BusRoute;
    for (int i=0; i<num_of_ops; i++) {
        cin >> op;
        if (op=="INSERT") {
            cin >> src >> dst >> name >> method;
            BusRoute.InsertStop(src, dst, name, method);
        } else if (op=="DELETE") {
            cin >> name;
            BusRoute.DeleteStop(name);
        } else if (op=="REVERSE") {
            BusRoute.Reverse();
        }
    }
    BusRoute.PrintRoute();
    return 0;
}
