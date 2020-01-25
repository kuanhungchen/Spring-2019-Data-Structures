/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #6
 |    Propose:  Huffman Compression
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/6/9 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++
 |    Updated:  2019/5/26 18:13
 *---------------------------------------------------*/
#include <iostream>
using namespace std;

struct Node {
    char _char;
    int _freq;
    Node* _next;
    Node* _left;
    Node* _right;
};
Node* CreateNode(char c, int f, Node* n, Node* l, Node* r) {
    Node* _node = new Node();
    _node->_char = c;
    _node->_freq = f;
    _node->_next = n;
    _node->_left = l;
    _node->_right = r;
    return _node;
}

class Tree {
private:
    Node* _root;
    int _length;
public:
    Tree();
    Node* Search(char);
    Node* GetRoot() {return _root;}
    void Sorting();
    void AddLength() {_length += 1;}
    void Replace(int, int);
    void Merge();
    void Traverse(char, string, Node*);
};
Tree::Tree() {
    _root = CreateNode('_', 100, NULL, NULL, NULL);  // put a dummy node in front
    _length = 0;
}
Node* Tree::Search(char c) {  // return target node or tail node
    Node* _current = _root;
    Node* _tail = _current;
    while(_current) {
        _tail = _current;
        if (_current->_char == c)
            return _current;
        _current = _current->_next;
    }
    return _tail;
}
void Tree::Replace(int x, int y) {  // use node A to replace node B (only char, freq)
    Node* _A = _root;
    Node* _B = _root;
    for (int i=0; i<y; i++)
        _A = _A->_next;
    for (int i=0; i<x; i++)
        _B = _B->_next;
    _B->_char = _A->_char;
    _B->_freq = _A->_freq;
}
void Tree::Sorting() {  // insertion sort of "linked-list" version
    Node* _node = _root;
    for (int i=0; i<_length; i++)
        _node = _node->_next;
    _node->_next = CreateNode('_', -1, NULL, NULL, NULL);  // add a dummy node at tail
    
    for (int i=_length-1; i>0; i--) {
        _node = _root;
        for (int j=0; j<i; j++)
            _node = _node->_next;
        Replace(_length+1, i);
        
        Node* _cur = _node->_next;
        int _target = _node->_freq;
        for (int k=1; k<=_length - i + 1; k++) {
            if (_cur->_freq > _target) {
                Replace(i+k-1, i+k);
                _cur = _cur->_next;
            } else {
                Replace(i+k-1, _length+1);
                break;
            }
//            if (k==_length-i)
//                Replace(_length, _length+1);
        }
    }
    _node = _root;
    for (int i=0; i<_length; i++)
        _node = _node->_next;
    _node->_next = NULL;
}
void Tree::Merge() {
    Node* _m_from;
    Node* _n_from;
    Node* _node;
    Node* _to;
    Node* _m;
    Node* _n;
    while (_length > 1) {
        _m_from = _root;
        _node = _m_from->_next;
        _to = _node->_next;
        int _offset = 1;
        while (_to) {
            if (_to->_freq < _node->_freq) {
                for (int i=0; i<_offset; i++) {
                    _node = _node->_next;
                    _m_from = _m_from->_next;
                }
                _offset = 1;
            } else {
                _offset += 1;
            }
            _to = _to->_next;
        }
        _m = _node;
        
        _n_from = _root;
        _node = _n_from->_next;
        _to = _node->_next;
        _offset = 1;
        while (_to) {
            if (_to != _m && _to->_freq < _node->_freq) {
                for (int i=0; i<_offset; i++) {
                    _node = _node->_next;
                    _n_from = _n_from->_next;
                }
                _offset = 1;
            } else {
                _offset += 1;
            }
            _to = _to->_next;
        }
        
        if (_node == _m)
            _n = _node->_next;
        else
            _n = _node;
        Node* _new = CreateNode('_', _m->_freq + _n->_freq, NULL, _m, _n);
        if (_m->_next == _n) {
            _m_from->_next = _n->_next;
        } else {
            _m_from->_next = _m->_next;
            _n_from->_next = _n->_next;
        }
        _m->_next = NULL;
        _n->_next = NULL;
        
        _node = _root;
        while (_node->_next != NULL && _node->_next->_freq >= _new->_freq) {
            _node = _node->_next;
        }
        _new->_next = _node->_next;
        _node->_next = _new;
        _length -= 1;
    }
}
void Tree::Traverse(char _t, string _ans, Node* _r) {
    if (_r == NULL)
        return;
    if (_r->_left != NULL && _r->_left->_char == _t)
        cout << _ans << "0";
    else if (_r->_right != NULL && _r->_right->_char == _t)
        cout << _ans << "1";
    else {
        Traverse(_t, _ans+"0", _r->_left);
        Traverse(_t, _ans+"1", _r->_right);
    }
}

int main(int argc, const char * argv[]) {
    int num_of_text;
    cin >> num_of_text;
    string _target;
    cin.get();
    cin >> _target;
    Tree MyHuffmanTree;
    char text;
    cin.get();
    for (int i=0; i<num_of_text; i++) {
        cin.get(text);
        while (text != '\n') {
            if (text != ' ') {
                Node* tmp = MyHuffmanTree.Search(text);
                if (tmp->_char == text)  // already exists, freq ++
                    tmp->_freq += 1;
                else  {  // doesn't exist, add new node at tail
                    tmp->_next = CreateNode(text, 1, NULL, NULL, NULL);
                    MyHuffmanTree.AddLength();
                }
            }
            cin.get(text);
        }
    }
    MyHuffmanTree.Sorting();
    MyHuffmanTree.Merge();
    int idx = 0;
    while (_target[idx] != '\0') {
        MyHuffmanTree.Traverse(_target[idx], "", MyHuffmanTree.GetRoot()->_next);
        idx += 1;
    }
    cout << endl;
    return 0;
}
