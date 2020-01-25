/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #4
 |    Propose:  Tic-Tac-Toe Tree
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/5/12 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++
 |    Updated:  2019/5/1 05:58
 *---------------------------------------------------*/
#include <iostream>
#include <queue>
using namespace std;

struct TreeNode{
    int _id;
    int _x;
    int _y;
    char _mark;
    TreeNode* _left;
    TreeNode* _right;
};

class Grid{
private:
    char content[3][3];
public:
    Grid();
    char GetMark(int x, int y) {return content[x][y];};
    void SetMark(int x, int y, char m) {content[x][y] = m;};
    void Reset();
};
Grid::Grid(){
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            content[i][j] = '_';
        }
    }
}
void Grid::Reset(){
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            content[i][j] = '_';
        }
    }
}

class Tree{
private:
    TreeNode* _root;
    bool win;
public:
    Tree();
    void SetRoot(int, int, char);
    TreeNode* Search(int);
    void SetLeftChild(TreeNode* root, TreeNode* left) {root->_left = left;};
    void SetRightChild(TreeNode* root, TreeNode* right) {root->_right = right;};
    void Display(TreeNode*);
    void Traverse(TreeNode*, Grid&, TreeNode [], int);
    bool Check(TreeNode [], Grid&, int);
    bool Result() {return win;};
    TreeNode* GetRoot() {return _root;};
};
Tree::Tree(){
    _root = new TreeNode();
    win = false;
}
void Tree::SetRoot(int x, int y, char m) {
    _root->_id = 0;
    _root->_x = x;
    _root->_y = y;
    _root->_mark = m;
}
TreeNode* Tree::Search(int x) {
    queue<TreeNode*> q;
    TreeNode* _current = _root;
    while (_current) {
        if (_current->_id == x)
            return _current;
        if (_current->_left)
            q.push(_current->_left);
        if (_current->_right)
            q.push(_current->_right);
        if (q.empty())
            return NULL;
        _current = q.front();
        q.pop();
    }
    return NULL;
}
void Tree::Display(TreeNode* _current){
    if (_current->_left) Display(_current->_left);
    if (_current->_right) Display(_current->_right);
    cout << _current->_x << " " << _current->_y << " " << _current->_mark << endl;
}
void Tree::Traverse(TreeNode* n, Grid& game, TreeNode path[], int path_length){
    if (n == NULL)
        return;
    
    // append current node to path
    path[path_length] = *n;
    path_length += 1;
    
    // leaf node case
    if (n->_left == NULL && n->_right == NULL) {
        game.Reset();
        if (Check(path, game, path_length)) {
            cout << "Win" << endl;
            for (int i=0; i<3; i++) {
                cout << game.GetMark(0, i);
                for (int j=1; j<3; j++) {
                    cout << " " << game.GetMark(j, i);
                }
                cout << endl;
            }
            win = true;
            return;
        }
    }
    else {  // recursion
        Traverse(n->_left, game, path, path_length);
        Traverse(n->_right, game, path, path_length);
    }
}
bool Tree::Check(TreeNode path[], Grid& game, int range) {
    if (range<3) return false;
    for (int i=0; i<range; i++) {
        game.SetMark(path[i]._x, path[i]._y, path[i]._mark);
    }
    if ((game.GetMark(0, 0) != '_' && game.GetMark(0, 0) == game.GetMark(1, 0) && game.GetMark(1, 0) == game.GetMark(2, 0)) ||
        (game.GetMark(0, 1) != '_' && game.GetMark(0, 1) == game.GetMark(1, 1) && game.GetMark(1, 1) == game.GetMark(2, 1)) ||
        (game.GetMark(0, 2) != '_' && game.GetMark(0, 2) == game.GetMark(1, 2) && game.GetMark(1, 2) == game.GetMark(2, 2)) ||
        (game.GetMark(0, 0) != '_' && game.GetMark(0, 0) == game.GetMark(0, 1) && game.GetMark(0, 1) == game.GetMark(0, 2)) ||
        (game.GetMark(1, 0) != '_' && game.GetMark(1, 0) == game.GetMark(1, 1) && game.GetMark(1, 1) == game.GetMark(1, 2)) ||
        (game.GetMark(2, 0) != '_' && game.GetMark(2, 0) == game.GetMark(2, 1) && game.GetMark(2, 1) == game.GetMark(2, 2)) ||
        (game.GetMark(0, 0) != '_' && game.GetMark(0, 0) == game.GetMark(1, 1) && game.GetMark(1, 1) == game.GetMark(2, 2)) ||
        (game.GetMark(2, 0) != '_' && game.GetMark(2, 0) == game.GetMark(1, 1) && game.GetMark(1, 1) == game.GetMark(0, 2))) {
        return true;
    }
    return false;
}
int main(int argc, const char * argv[]) {
    int num_of_nodes;
    cin >> num_of_nodes;
    int id;
    int parent_id;
    int x;
    int y;
    char mark;
    Grid MyGame;
    Tree MyTree;
    TreeNode* root = new TreeNode();
    cin >> id >> parent_id >> x >> y >> mark;
    root->_id = id;
    root->_x = x;
    root->_y = y;
    root->_mark = mark;
    MyTree.SetRoot(root->_x, root->_y, root->_mark);
    TreeNode* pre_node = new TreeNode();
    for (int i=1; i<num_of_nodes; i++) {
        TreeNode* node = new TreeNode();
        cin >> id >> parent_id >> x >> y >> mark;
        node->_id = id;
        node->_x = x;
        node->_y = y;
        node->_mark = mark;
        pre_node = MyTree.Search(parent_id);
        if (pre_node->_left != NULL) {
            MyTree.SetRightChild(pre_node, node);
        }
        else {
            MyTree.SetLeftChild(pre_node, node);
        }
    }
    TreeNode _path[512-1];
    MyTree.Traverse(MyTree.GetRoot(), MyGame, _path, 0);
    if (!MyTree.Result()) {
        cout << "Tie" << endl;
        MyTree.Display(MyTree.GetRoot());
    }
    return 0;
}
