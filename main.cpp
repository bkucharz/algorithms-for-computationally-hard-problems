#include <iostream>
enum link {linked, blocked, unknown};
enum direction {stationary=-1, up, right, down, left};
direction oppositeDir(direction dir) {
    return direction((dir+2)%4);
}

struct Vector {
    explicit Vector(direction dir) {
        this->dir = dir;
        this->row = 0;
        this->column = 0;
        if (dir == up){
            row = -1;
        }
        else if (dir == right){
            column=1;
        }
        else if (dir == down){
            row=1;
        }
        else if (dir == left){
            column=-1;
        }
    }
    char toChar() const {
        if (dir == up){
            return 'U';
        }
        else if (dir == right){
            return 'R';
        }
        else if (dir == down){
            return 'D';
        }
        else if (dir == left){
            return 'L';
        }
        return 'X';
    }

    Vector opposite() const {
        return * new Vector(oppositeDir(this->dir));
    }


    int row; int column; direction dir;};

bool checkCorrectness(int n, int m, int **hints, link**** joints);
bool solve(bool start, int n, int m, int **hints,  link**** joints, int, int, int, int);



int main() {
    int n, m;
    std::cin >> m >> n;
    int **hints = new int * [n];
    link**** joints = new link *** [n+1];

    for (int row=0; row<n; row++){
        hints[row] = new int [m];
        for (int column=0; column<m; column++){
            char temp;
            std::cin >> temp;
            hints[row][column] = temp - '0';
        }
    }

    for (int row=0; row<n+1; row++){
        joints[row] = new link ** [m+1];
        for (int column=0; column<m+1; column++){
            joints[row][column] = new link * [4];
        }
    }



    link* blockedLink = new link(blocked);
    for (int col=0; col<m+1; col++) {
        joints[0][col][up] = blockedLink;
        joints[n][col][down] = blockedLink;
    }

    for (int row=0; row<n+1; row++) {
        joints[row][0][left] = blockedLink;
        joints[row][m][right] = blockedLink;
    }

    for (int row=0; row<n+1; row++) {
        for (int col=1; col<m+1; col++) {
            joints[row][col][left] = joints[row][col-1][right] = new link(unknown);
        }
    }

    for (int row=1; row<n+1; row++) {
        for (int col=0; col<m+1; col++) {
            joints[row][col][up] = joints[row-1][col][down] = new link(unknown);
        }
    }


    bool found = false;
    for (int start_row=0; start_row<n+1; start_row++){
        for (int start_col=0; start_col<m+1; start_col++){
            if (solve(true, n, m, hints, joints, start_row, start_col, start_row, start_col)) {
                found = true;
                break;
            }
        }
        if (found){
            break;
        }
    }
}


int countHint(int row, int column, link**** joints) {
    int count = 0;
    if (*joints[row][column][right] == linked) count++;
    if (*joints[row][column+1][down] == linked) count++;
    if (*joints[row+1][column+1][left] == linked) count++;
    if (*joints[row+1][column][up] == linked) count++;

    return count;
}

bool checkCorrectness(int n, int m, int **hints, link**** joints) {
    for (int row=0; row<n; row++){
        for (int column=0; column<m; column++){
            if (countHint(row, column, joints) != hints[row][column]) {
                return false;
            }
        }
    }
    return true;
}


bool solve(bool start, int n, int m, int** hints,  link**** joints, int row, int col, int start_row, int start_col) {
    if (!start and row == start_row and col == start_col){
        if (checkCorrectness(n, m, hints, joints)) {
            std::cout << start_col << " " << start_row << std::endl;
            return true;
        } else {
            return false;
        }
    }

    for (int dir = 0; dir < 4; dir ++) {
        if (*joints[row][col][dir] == unknown){
            Vector v = * new Vector(static_cast<direction>(dir));

            int count = 0;
            for (int dir2 = 0; dir2 < 4; dir2 ++) {
                if (*joints[row + v.row][col+v.column][dir2] == linked){
                    count++;
                }
            }
            if (count == 2){
                continue;
            }


            *joints[row][col][dir] = linked;

            bool foundExceededHint = false;
            if (row-1 >= 0 and col-1 >= 0) {
                if (countHint(row - 1, col - 1, joints) > hints[row - 1][col - 1]){
                    foundExceededHint = true;
                }
            }
            if (row-1 >= 0 and col < m) {
                if (countHint(row - 1, col, joints) > hints[row - 1][col]){
                    foundExceededHint = true;
                }
            }
            if (row < n and col < m) {
                if (countHint(row, col, joints) > hints[row][col]){
                    foundExceededHint = true;
                }
            }
            if (row < n and col-1 >= 0) {
                if (countHint(row, col - 1, joints) > hints[row][col - 1]){
                    foundExceededHint = true;
                }
            }

            if (foundExceededHint){
                *joints[row][col][v.dir] = unknown;
                continue;
            }


            if (solve(false, n, m, hints, joints, row + v.row, col + v.column, start_row, start_col))
            {
                std::cout << v.opposite().toChar();
                return true;
            }


            *joints[row][col][v.dir] = unknown;
        }
    }


    return false;

}