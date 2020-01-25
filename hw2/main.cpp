/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #2
 |    Propose:  word puzzle
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/4/7 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++
 |    Updated:  2019/3/27 09:40
 *---------------------------------------------------*/
#include <iostream>
using namespace std;


struct pos {
    int x;
    int y;
};
struct dir {
    int x=0;
    int y=0;
};
struct cell {
    char character;
    struct pos cell_pos;
};
int VowelOrConsonant (char x) {
    if ((x == 'a') || (x == 'e') || (x == 'i') || (x == 'o') || (x == 'u')) {
        return 0; // vowel
    }
    return 1; // consonant
}
bool In_Scope(pos tp, int range) {
    if ((tp.x >= 0) && (tp.x < range) && (tp.y >= 0) && (tp.y < range)) {
        return true; // target cell is in puzzle
    }
    return false; // target cell is out of puzzle
}
dir dir_translate(int x) {
    dir dir_translated;
    switch(x) {
        case 0: // down
            dir_translated.x = 1;
            dir_translated.y = 0;
            break;
        case 1: // right
            dir_translated.x = 0;
            dir_translated.y = 1;
            break;
        case 2: // up
            dir_translated.x = -1;
            dir_translated.y = 0;
            break;
        case 3: // left
            dir_translated.x = 0;
            dir_translated.y = -1;
            break;
    }
    return dir_translated;
}

class CellStack{
public:
    CellStack();
    ~CellStack();
    bool StateToPush(char) const;
    bool StateToPrint(CellStack&) const;
    int GetNumOfConsonant() const {return consonant_count;}
    int GetLength() const {return length;}
    char ShowCharInStack(int x) const {return array[x].character;}
    void PushToStack(cell&);
    void PopFromStack();
    pos GetStartingCellPos(int, int) const;
    cell GetCurrentCell() const;
private:
    cell *array;
    int length;
    int top;
    int consonant_count;
};
CellStack::CellStack()
{
    array = new cell [100];
    length = 0;
    top = -1;
    consonant_count = 0;
}
CellStack::~CellStack() {delete [] array;}
inline bool CellStack::StateToPush(char x) const{
    // to check if target character is valid to be pushed in stack
    char last_char = array[top].character;
    if (VowelOrConsonant(last_char) == 0) {
        // last character is a vowel, anything is valid to be pushed
        return true;
    } else if (VowelOrConsonant(last_char) == 1 && VowelOrConsonant(x) == 0) {
        // last character is a consonant, a vowel is valid to be pushed
        return true;
    }
    return false;
}
inline bool CellStack::StateToPrint(CellStack& x) const{
    if (x.GetNumOfConsonant()>= 3 && VowelOrConsonant(x.GetCurrentCell().character) == 1) {
        // valid to print: more than three consonants and ends with a consonant
        return true;
    }
    return false;
}
inline void CellStack::PushToStack(cell& x) {
    if (VowelOrConsonant(x.character)==1) {consonant_count++;}
    array[++top] = x;
    length ++;
}
inline void CellStack::PopFromStack() {
    if (VowelOrConsonant(array[top].character)==1) {consonant_count--;}
    top --;
    length --;
}
pos CellStack::GetStartingCellPos(int x, int range) const {
    // get the position of starting cell
    pos first_cell_pos;
    first_cell_pos.x = x / range;
    first_cell_pos.y = x % range;
    return first_cell_pos;
}
inline cell CellStack::GetCurrentCell() const {
    // get the top-most cell
    cell x;
    x.character = array[top].character;
    x.cell_pos = array[top].cell_pos;
    return x;
}

void PrintOrigin(CellStack& x) {
    // print current word in origin order
    for (int i=0; i<x.GetLength(); i++) {
        cout << x.ShowCharInStack(i);
    }
}
void PrintRearranged(CellStack& x) {
    // print current word in rearranged order
    for (int i=0; i<x.GetLength(); i++) {
        if (VowelOrConsonant(x.ShowCharInStack(i))==0) cout << x.ShowCharInStack(i);
    }
    for (int i=0; i<x.GetLength(); i++) {
        if (VowelOrConsonant(x.ShowCharInStack(i))==1) cout << x.ShowCharInStack(i);
    }
}


int main(int argc, const char * argv[]) {
    int size_of_puzzle = 0;
    cin >> size_of_puzzle;
    char puzzle[size_of_puzzle][size_of_puzzle];
    for (int i = 0; i < size_of_puzzle; i ++) {
        for (int j = 0; j < size_of_puzzle; j++) {
            cin >> puzzle[i][j];
        }
    }
    int dir_map[size_of_puzzle][size_of_puzzle]; // [0: down, 1: right, 2: up, 3: left]
    int inc_map[size_of_puzzle][size_of_puzzle]; // [0: not in path, 1: in path]
    for (int i=0; i<size_of_puzzle; i++) {
        for (int j=0; j<size_of_puzzle; j++) {
            dir_map[i][j] = -1;
            inc_map[i][j] = 0;
        }
    }
    int starting_cell_pointer = 0; // pointer of starting cell
    CellStack MyWordStack;
    while (starting_cell_pointer < size_of_puzzle*size_of_puzzle) {
        cell starting_cell;
        starting_cell.cell_pos = MyWordStack.GetStartingCellPos(starting_cell_pointer, size_of_puzzle);
        starting_cell.character = puzzle[starting_cell.cell_pos.x][starting_cell.cell_pos.y];
        if (VowelOrConsonant(starting_cell.character)==0) {
            starting_cell_pointer ++;
            continue;
        }
        MyWordStack.PushToStack(starting_cell);
        cell current_cell = MyWordStack.GetCurrentCell();
        inc_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = 1;
        dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
        cell target_cell;
        while(dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] != 4 && MyWordStack.GetLength()!=0) {
            current_cell = MyWordStack.GetCurrentCell();
            target_cell.cell_pos.x = current_cell.cell_pos.x + dir_translate(dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y]).x;
            target_cell.cell_pos.y = current_cell.cell_pos.y + dir_translate(dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y]).y;
            if (In_Scope(target_cell.cell_pos, size_of_puzzle)) {
                // target cell is in puzzle
                target_cell.character = puzzle[target_cell.cell_pos.x][target_cell.cell_pos.y];
                if (inc_map[target_cell.cell_pos.x][target_cell.cell_pos.y] == 1) {
                    // target cell included in path
                    dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
                } else {
                    // target cell not included in path
                    if (MyWordStack.StateToPush(target_cell.character)) { // valid to push
                        MyWordStack.PushToStack(target_cell);
                        current_cell = MyWordStack.GetCurrentCell();
                        inc_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = 1;
                        if (MyWordStack.StateToPrint(MyWordStack)) { // valid to print
                            PrintOrigin(MyWordStack);
                            cout << " ";
                            PrintRearranged(MyWordStack);
                            cout << "\n";
                        }
                        dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
                    } else { // invalid to push
                        dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
                    }
                }
            } else { // target cell is not in puzzle
                dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
            }
            while (dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] == 4 && MyWordStack.GetLength()!= 0) {
                if (MyWordStack.GetLength() == 1) { // only the starting cell in stack
                    dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = -1;
                    inc_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = 0;
                    MyWordStack.PopFromStack();
                    starting_cell_pointer ++;
                } else { // other attached cell in stack
                    dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = -1;
                    inc_map[current_cell.cell_pos.x][current_cell.cell_pos.y] = 0;
                    MyWordStack.PopFromStack();
                    current_cell = MyWordStack.GetCurrentCell();
                    dir_map[current_cell.cell_pos.x][current_cell.cell_pos.y] ++;
                }
            }
        }
    }
    return 0;
}
