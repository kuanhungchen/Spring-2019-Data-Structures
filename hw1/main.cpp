/*----------------------------------------------------
 |     Course:  Data structure
 | Assignment:  homework #1
 |    Propose:  check and/or fix magic squares
 | Instructor:  Liu Ren-Shuo
 |        Due:  2019/2/27 23:59
 |
 |     Author:  Chen Kuan-Hung (s77343@icloud.com)
 | Student ID:  105061171
 |   Language:  C++ (mainly)
 |    Updated:  2019/2/26 21:17
 *---------------------------------------------------*/
#include <iostream>
#include <map>
#include <iomanip>
#include <cmath>
using namespace std;

int get_length(int target) {
    int length = 0;
    while(target) {
        target /= 10;
        length ++;
    }
    return length;
}

int main() {
    int num_of_squares = 0;
    cin >> num_of_squares;
    int square_sizes[num_of_squares];
    int squares[num_of_squares][25][25];
    
    /*----------------input data----------------*/
    for (int current_square=0; current_square<num_of_squares; current_square++) {
        cin >> square_sizes[current_square];
        for (int index_in_square=0; index_in_square<square_sizes[current_square]; index_in_square++) {
            for (int index_in_square_inner=0; index_in_square_inner<square_sizes[current_square]; index_in_square_inner++){
                cin >> squares[current_square][index_in_square][index_in_square_inner];
            }
        }
    }
    
    /*----------------show input data----------------*/
//    for (int index=0; index<num_of_squares; index++) {
//        for (int i=0; i < square_sizes[index]; i++){
//            for (int j=0; j < square_sizes[index]; j++){
//                cout << squares[0][i][j] << " ";
//            }
//            cout << endl;
//        }
//    }
    
    /*----------------compute summations of cols and rows----------------*/
    for (int check_square_index=0; check_square_index<num_of_squares; check_square_index++) {
        int col_sums[square_sizes[check_square_index]];
        int row_sums[square_sizes[check_square_index]];
        
        /*----------col summation----------*/
        for (int col_index=0; col_index<square_sizes[check_square_index]; col_index++) {
            for (int col_index_inner=0; col_index_inner<square_sizes[check_square_index]; col_index_inner++) {
                if (col_index==0) col_sums[col_index_inner] = squares[check_square_index][col_index][col_index_inner];
                else col_sums[col_index_inner] += squares[check_square_index][col_index][col_index_inner];
            }
        }
        /*----------row summation----------*/
        for (int row_index=0; row_index<square_sizes[check_square_index]; row_index++) {
            for (int row_index_inner=0; row_index_inner<square_sizes[check_square_index]; row_index_inner++) {
                if (row_index_inner==0) row_sums[row_index] = squares[check_square_index][row_index][row_index_inner];
                else row_sums[row_index] += squares[check_square_index][row_index][row_index_inner];
            }
        }
        
        /*----------------show summation results----------------*/
//        cout<<"Cols\tRows\n";
//        for (int i=0; i<square_sizes[check_square_index]; i++) {
//            cout<<col_sums[i]<<"\t"<<row_sums[i]<<"\n";
//        }
        
        /*----------------insert into map----------------*/
        map<int, int>sum_to_position_col;
        map<int, int>sum_to_position_row;
        typedef pair<int, int> sum_to_position_pair;
        for (int i=0; i<square_sizes[check_square_index]; i++){
            sum_to_position_col.insert(sum_to_position_pair(i, col_sums[i]));
            sum_to_position_row.insert(sum_to_position_pair(i, row_sums[i]));
        }
        
        /*----------------start check good/bad/fixable----------------*/
        /*----------For column check----------*/
        int answer_of_col = 2; // [-1: bad, 0: fixable, 1: good]
        int supreme_value = -1;
        int suspect_col[2] = {-1, -1};
        map<int, int>::iterator iter_col = sum_to_position_col.begin();
        int count_col = 0;
        int once_count_col = 0;
        for (int check_each_sum=0; check_each_sum<square_sizes[check_square_index]; check_each_sum++) {
            iter_col = sum_to_position_col.begin();
            while (iter_col!=sum_to_position_col.end()) {
                if (iter_col->second == col_sums[check_each_sum]) {
                    count_col++;
                }
                iter_col++;
            }
            if (count_col == square_sizes[check_square_index]) {
                // GOOD AT COLUMN
                answer_of_col = 1;
                supreme_value = col_sums[check_each_sum];
            }
            else if (count_col == 1) {
                if (once_count_col == 0) {
                    iter_col = sum_to_position_col.begin();
                    while (iter_col->second != col_sums[check_each_sum]){
                        iter_col++;
                    }
                    suspect_col[0] = iter_col->first;
                }
                else{
                    iter_col = sum_to_position_col.begin();
                    while (iter_col->second != col_sums[check_each_sum]){
                        iter_col++;
                    }
                    suspect_col[1] = iter_col->first;
                }
                once_count_col++;
            }
            else if (count_col == square_sizes[check_square_index]-2) {supreme_value = col_sums[check_each_sum];}
            else if (count_col != square_sizes[check_square_index]-2) {
                // BAD AT COLUMN
                answer_of_col = -1;
            }
            count_col = 0;
        }
        /*----------For row check----------*/
        int answer_of_row = 2; // [-1: bad, 0: fixable, 1: good]
        int supreme_value_row = -1;
        int suspect_row[2] = {-1, -1};
        map<int, int>::iterator iter_row = sum_to_position_row.begin();
        int count_row = 0;
        int once_count_row = 0;
        for (int check_each_sum=0; check_each_sum<square_sizes[check_square_index]; check_each_sum++) {
            iter_row = sum_to_position_row.begin();
            while (iter_row!=sum_to_position_row.end()) {
                if (iter_row->second == row_sums[check_each_sum]) {
                    count_row++;
                }
                iter_row++;
            }
            if (count_row == square_sizes[check_square_index]) {
                // GOOD AT ROW
                answer_of_row = 1;
                supreme_value_row = row_sums[check_each_sum];
            }
            else if (count_row == 1) {
                if (once_count_row == 0) {
                    iter_row = sum_to_position_row.begin();
                    while (iter_row->second != row_sums[check_each_sum]){
                        iter_row++;
                    }
                    suspect_row[0] = iter_row->first;
                }
                else{
                    iter_row = sum_to_position_row.begin();
                    while (iter_row->second != row_sums[check_each_sum]){
                        iter_row++;
                    }
                    suspect_row[1] = iter_row->first;
                }
                once_count_row++;
            }
            else if (count_row == square_sizes[check_square_index]-2) {supreme_value_row = row_sums[check_each_sum];}
            else if (count_row != square_sizes[check_square_index]-2) {
                // BAD AT ROW
                answer_of_row= -1;
            }
            count_row = 0;
        }
        if (answer_of_row==2 || answer_of_col==2) {
            if (supreme_value_row == -1 || supreme_value == -1) {
                // BAD AT ROW
                answer_of_col = -2;
                answer_of_row = -2;
            }
            else {
                
                if (once_count_col==2 && (col_sums[suspect_col[0]]+col_sums[suspect_col[1]]-2*supreme_value)==0 && once_count_row==2 && (row_sums[suspect_row[0]]+row_sums[suspect_row[1]]-2*supreme_value_row)==0) {
                    // can fix
                    answer_of_row = 0;
                    answer_of_col = 0;
                }
                else if (once_count_col==0 && once_count_row==2 && (row_sums[suspect_row[0]]+row_sums[suspect_row[1]]-2*supreme_value_row)==0) {
                    // can fix
                    answer_of_col = 1;
                    answer_of_row = 0;
                }
                else if (once_count_col==2 && (col_sums[suspect_col[0]]+col_sums[suspect_col[1]]-2*supreme_value)==0 && once_count_row==0) {
                    // can fix
                    answer_of_col = 0;
                    answer_of_row = 1;
                }
                else {
                    // can not fix
                    answer_of_col = -3;
                    answer_of_row = -3;
                }
            }
        }
        
        /*----------------final print----------------*/
        if (answer_of_col == 1 && answer_of_row == 1) {
            /*----------good at both----------*/
            int diagonal_1 = 0;
            int diagonal_2 = 0;
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index]; diagonal_check_index++){
                diagonal_1 += squares[check_square_index][diagonal_check_index][diagonal_check_index];
            }
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index];diagonal_check_index++){
                diagonal_2 += squares[check_square_index][diagonal_check_index][square_sizes[check_square_index]-diagonal_check_index-1];
            }
            if (diagonal_1 != supreme_value || diagonal_2 != supreme_value) {cout<<"bad";}
            else {cout<<"good "<<supreme_value;}
        }
        else if (answer_of_col<0 || answer_of_row<0) {
            /*----------bad at either one----------*/
            cout<<"bad";
        }
        /*----------------for fixable cases----------------*/
        else if (once_count_col == 0 && once_count_row == 2) {
            /*----------vertical swap----------*/
            int target_index = -1;
            for (int find_swap_index=0; find_swap_index<square_sizes[check_square_index]; find_swap_index++) {
                if ((abs(supreme_value - row_sums[suspect_row[0]])+abs(supreme_value - row_sums[suspect_row[1]])-2*abs(squares[check_square_index][suspect_row[0]][find_swap_index]-squares[check_square_index][suspect_row[1]][find_swap_index]))==0) {
                    // swap
                    target_index = find_swap_index;
                    int temp = squares[check_square_index][target_index][suspect_row[0]];
                    squares[check_square_index][target_index][suspect_row[0]] = squares[check_square_index][target_index][suspect_row[1]];
                    squares[check_square_index][target_index][suspect_row[1]] = temp;
                }
            }
            // check diagonal
            int diagonal_1 = 0;
            int diagonal_2 = 0;
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index]; diagonal_check_index++){
                diagonal_1 += squares[check_square_index][diagonal_check_index][diagonal_check_index];
            }
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index];diagonal_check_index++){
                diagonal_2 += squares[check_square_index][diagonal_check_index][square_sizes[check_square_index]-diagonal_check_index-1];
            }
            if (diagonal_1 != supreme_value || diagonal_2 != supreme_value) {cout<<"bad";}
            else {
                // print new square
                cout<<"fixable "<<supreme_value<<endl;
                for (int print_index=0; print_index<square_sizes[check_square_index]; print_index++) {
                    for (int print_index_inner=0; print_index_inner<square_sizes[check_square_index]; print_index_inner++) {
                        if (print_index_inner==target_index && (print_index==suspect_row[0] || print_index==suspect_row[1])){
                            int length_of_number = get_length(squares[check_square_index][print_index][target_index]);
                            for (int i=0; i<(6-length_of_number-2); i++) {cout<<" ";}
                            cout<<"(";
                            cout<<squares[check_square_index][print_index][target_index];
                            cout<<")";
                            
                        }
                        else {cout << setw(6) << squares[check_square_index][print_index][print_index_inner];}
                    }
                    if (print_index != square_sizes[check_square_index]-1) {cout << endl;}
                }
            }
        }
        else if (once_count_col == 2 && once_count_row == 0) {
            /*----------horizontal swap----------*/
            int target_index = -1;
            for (int find_swap_index=0; find_swap_index<square_sizes[check_square_index]; find_swap_index++) {
                if ((abs(supreme_value - col_sums[suspect_col[0]])+abs(supreme_value - col_sums[suspect_col[1]])-2*abs(squares[check_square_index][find_swap_index][suspect_col[0]]-squares[check_square_index][find_swap_index][suspect_col[1]]))==0) {
                    // swap
                    target_index = find_swap_index;
                    int temp = squares[check_square_index][target_index][suspect_col[0]];
                    squares[check_square_index][target_index][suspect_col[0]] = squares[check_square_index][target_index][suspect_col[1]];
                    squares[check_square_index][target_index][suspect_col[1]] = temp;
                    break;
                }
            }
            // check diagonal
            int diagonal_1 = 0;
            int diagonal_2 = 0;
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index]; diagonal_check_index++){
                diagonal_1 += squares[check_square_index][diagonal_check_index][diagonal_check_index];
            }
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index];diagonal_check_index++){
                diagonal_2 += squares[check_square_index][diagonal_check_index][square_sizes[check_square_index]-diagonal_check_index-1];
            }
            if (diagonal_1 != supreme_value || diagonal_2 != supreme_value) {cout<<"bad";}
            else {
                // print new square
                cout<<"fixable "<<supreme_value<<endl;
                for (int print_index=0; print_index<square_sizes[check_square_index]; print_index++) {
                    for (int print_index_inner=0; print_index_inner<square_sizes[check_square_index]; print_index_inner++) {
                        if (print_index==target_index && (print_index_inner==suspect_col[0]||print_index_inner==suspect_col[1])){
                            int length_of_number = get_length(squares[check_square_index][target_index][print_index_inner]);
                            for (int i=0; i<(6-length_of_number-2); i++) {cout<<" ";}
                            cout<<"(";
                            cout<<squares[check_square_index][target_index][print_index_inner];
                            cout<<")";
                            
                        }
                        else {cout << setw(6) << squares[check_square_index][print_index][print_index_inner];}
                    }
                    if (print_index != square_sizes[check_square_index]-1) {cout << endl;}
                }
            }
        }
        else if (once_count_col == 2 && once_count_row == 2){
            /*----------both vertical and horizontal----------*/
            int swap_case = -1; // [0: (r0, c0) <-> (r1, c1) ; 1: (r0, c1) <-> (r1, c0)]
            if ((abs(supreme_value - col_sums[suspect_col[0]])+abs(supreme_value - col_sums[suspect_col[1]])-2*abs(squares[check_square_index][suspect_row[0]][suspect_col[0]]-squares[check_square_index][suspect_row[1]][suspect_col[1]]))==0) {
                // swap
                int temp = squares[check_square_index][suspect_row[0]][suspect_col[0]];
                squares[check_square_index][suspect_row[0]][suspect_col[0]] = squares[check_square_index][suspect_row[1]][suspect_col[1]];
                squares[check_square_index][suspect_row[1]][suspect_col[1]] = temp;
                swap_case = 0;
            }
            else {
                // swap
                int temp = squares[check_square_index][suspect_row[0]][suspect_col[1]];
                squares[check_square_index][suspect_row[0]][suspect_col[1]] = squares[check_square_index][suspect_row[1]][suspect_col[0]];
                squares[check_square_index][suspect_row[1]][suspect_col[0]] = temp;
                swap_case = 1;
            }
            // check diagonal
            int diagonal_1 = 0;
            int diagonal_2 = 0;
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index]; diagonal_check_index++){
                diagonal_1 += squares[check_square_index][diagonal_check_index][diagonal_check_index];
            }
            for (int diagonal_check_index=0; diagonal_check_index<square_sizes[check_square_index];diagonal_check_index++){
                diagonal_2 += squares[check_square_index][diagonal_check_index][square_sizes[check_square_index]-diagonal_check_index-1];
            }
            if (diagonal_1 != supreme_value || diagonal_2 != supreme_value) {cout<<"bad";}
            else {
                // print new square
                cout<<"fixable "<<supreme_value<<endl;
                for (int print_index=0; print_index<square_sizes[check_square_index]; print_index++) {
                    for (int print_index_inner=0; print_index_inner<square_sizes[check_square_index]; print_index_inner++) {
                        if (swap_case==0 && ((print_index==suspect_row[0]&&print_index_inner==suspect_col[0])||(print_index==suspect_row[1]&&print_index_inner==suspect_col[1]))){
                            int length_of_number = get_length(squares[check_square_index][print_index][print_index_inner]);
                            for (int i=0; i<(6-length_of_number-2); i++) {cout<<" ";}
                            cout<<"(";
                            cout<<squares[check_square_index][print_index][print_index_inner];
                            cout<<")";
                            
                        }
                        else if (swap_case==1 && ((print_index==suspect_row[0]&&print_index_inner==suspect_col[1])||(print_index==suspect_row[1]&&print_index_inner==suspect_col[0]))){
                            int length_of_number = get_length(squares[check_square_index][print_index][print_index_inner]);
                            for (int i=0; i<(6-length_of_number-2); i++) {cout<<" ";}
                            cout<<"(";
                            cout<<squares[check_square_index][print_index][print_index_inner];
                            cout<<")";
                            
                        }
                        else {cout << setw(6) << squares[check_square_index][print_index][print_index_inner];}
                    }
                    if (print_index != square_sizes[check_square_index]-1) {cout << endl;}
                }
            }
        }
        cout<<endl;
    }
    return 0;
}
