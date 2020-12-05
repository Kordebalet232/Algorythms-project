#include <iostream>
#include <limits>
#include <time.h>


using namespace std;

const int _inf = numeric_limits<int>::max();
const int _inf2 = numeric_limits<int>::max() - 1;


void Print_matrix(int n, int** X) // print matrix
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if ((i == 0) && (j == 0))
                cout << " " << '\t';
            else if (X[i][j] == _inf)
                cout << "inf" << '\t';
            else if (X[i][j] == _inf2)
                cout << "inf2" << '\t';
            else
                cout << X[i][j] << '\t';
        }
        cout << endl << endl;
    }
    cout << endl;
};

int DecreaseRows(int n, int** X) // returns sum of min elements
{
    int min, sum = 0;
    for (int i = 1; i < n; ++i)
    {
        min = _inf;
        for (int j = 1; j < n; ++j)
        {
            if (X[i][j] < min)
                min = X[i][j];
        }
        if (min != 0)
        {
            for (int j = 1; j < n; ++j)
            {
                if (X[i][j] < _inf2)
                    X[i][j] -= min;
            }
            sum += min;
        }
    }
    return sum;
};

int DecreaseColumns(int n, int** X) // returns sum of min elements
{
    int min, sum = 0;
    for (int i = 1; i < n; ++i)
    {
        min = _inf;
        for (int j = 1; j < n; ++j)
        {
            if (X[j][i] < min)
                min = X[j][i];
        }
        if (min != 0)
        {
            for (int j = 1; j < n; ++j)
            {
                if (X[j][i] < _inf2)
                    X[j][i] -= min;
            }
            sum += min;
        }
    }
    return sum;
};

void CheckZeros(int n, int** X, int& i_max, int& j_max) // looking for zero with max estimate
{
    int minRow, minCol;
    int k = -1;
    for (int i = 1; i < n; ++i)
    {
        for (int j = 1; j < n; ++j)
        {
            if (X[i][j] == 0)
            {
                minRow = _inf2;
                minCol = _inf2;
                for (int h = 1; h < n; ++h)
                {
                    if ((X[i][h] <= minRow) && (h != j)) // find min in row
                        minRow = X[i][h];

                    if ((X[h][j] <= minCol) && (h != i)) // find min in col
                        minCol = X[h][j];
                }
                if (minRow == _inf2)
                    minCol = 0;
                else if (minCol == _inf2)
                    minRow = 0;

                if (minRow + minCol > k) // check if it's max
                {
                    i_max = i;
                    j_max = j;
                    k = minRow + minCol;
                }
            }
        }
    }
};

void Edge_matrix(int n, int** X, int row, int col) // edge matrix
{
    int i_inf = -1;
    int j_inf = -1;

    for (int i = 1; i < n; ++i)
    {
        if (X[row][i] == _inf)
            i_inf = i;
        if (X[i][col] == _inf)
            j_inf = i;
    }
    if (i_inf == -1 || j_inf == -1)
    {
        cout << "Infinum not found" << endl;
        cout << "row = " << row << ", col = " << col << endl;
        cout << "i_inf = " << i_inf << ", j_inf = " << j_inf << endl;
        Print_matrix(n, X);
        exit(1);
    }
    X[j_inf][i_inf] = _inf;

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - 1; ++j)
        {
            if (i < row)
            {
                if (j >= col)
                    X[i][j] = X[i][j + 1];
            }
            else {
                if (j < col)
                    X[i][j] = X[i + 1][j];
                else
                    X[i][j] = X[i + 1][j + 1];
            }
        }
    }
};

int** Generator(int n) // generator for random inputs
{
    int** X = new int* [n + 1];
    for (int i = 0; i < n + 1; ++i)
        X[i] = new int[n + 1];
    for (int i = 0; i < n + 1; ++i)
    {
        X[0][i] = i - 1;
        X[i][0] = i - 1;
    }
    for (int i = 1; i < n + 1; ++i)
    {
        for (int j = 1; j < n + 1; ++j)
        {
            if (i == j)
                X[i][j] = _inf;
            else
                X[i][j] = rand() % 1000000 + 1;

        }
    }
    return X;
}

void Little_alg_with_steps(int n, int** A, int border, int& record, int** PCSol, int** CSol, int num) // Little's algorythm with steps
{
    cout << "Текущая матрица: " << endl;
    Print_matrix(n, A);

    int sumMinRow;
    sumMinRow = DecreaseRows(n, A);

    int sumMinCol;
    sumMinCol = DecreaseColumns(n, A);

    cout << "После преобразований:" << endl;
    Print_matrix(n, A);

    int newBorder = border + sumMinRow + sumMinCol;

    if (newBorder < record)
    {
        int i_max = 0, j_max = 0;
        CheckZeros(n, A, i_max, j_max);

        PCSol[num][0] = A[i_max][0];
        PCSol[num][1] = A[0][j_max];
        cout << "Текущая часть решения:" << endl;
        for (int i = 0; i <= num; ++i)
        {
            cout << "(" << PCSol[i][0] << ";" << PCSol[i][1] << ")" << endl;
        }
        cout << endl;

        int** Amn = new int* [n];
        for (int i = 0; i < n; ++i)
            Amn[i] = new int[n];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                Amn[i][j] = A[i][j];
            }
        }
        Edge_matrix(n, Amn, i_max, j_max);

        if (n > 4) // begin recursion
        {
            cout << "Текущая граница: " << newBorder << endl << endl;
            Little_alg_with_steps(n - 1, Amn, newBorder, record, PCSol, CSol, num + 1);
        }
        else // add rest arcs
        {
            cout << "Текущая матрица: " << endl;
            Print_matrix(n - 1, Amn);
            int lastLimit = newBorder;
            for (int i = 1; i < n - 1; ++i)
            {
                for (int j = 1; j < n - 1; ++j)
                {
                    if ((Amn[i][j] != _inf) && (Amn[i][j] != _inf2))
                    {
                        ++num;
                        PCSol[num][0] = Amn[i][0];
                        PCSol[num][1] = Amn[0][j];
                        lastLimit += Amn[i][j];
                    }
                }
            }

            cout << "Кандидат решения:" << endl;
            for (int i = 0; i < num + 1; ++i)
            {
                CSol[i][0] = PCSol[i][0];
                CSol[i][1] = PCSol[i][1];
                cout << "(" << CSol[i][0] << ";" << CSol[i][1] << ")" << endl;
            }
            cout << endl;

            if (record == _inf)
                cout << "Текущая граница (" << lastLimit << ") < " << "Рекорд (inf) ==> " << endl;
            else
                cout << "Текущая граница (" << lastLimit << ") <= " << "Рекорд (" << record << ") ==> " << endl;

            record = lastLimit;
            cout << "Новый рекорд: " << record << endl << endl;
            num -= 2;
        }

        // recursion
        int** A_mn = new int* [n];
        for (int i = 0; i < n; ++i)
            A_mn[i] = new int[n];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A_mn[i][j] = A[i][j];
            }
        }
        A_mn[i_max][j_max] = _inf2;

        Little_alg_with_steps(n, A_mn, newBorder, record, PCSol, CSol, num);
    }

    else
    {
        cout << "Текущая граница (" << newBorder << ") >= Рекорд (" << record << ") ==> Остановка" << endl << endl;
    }
};

void Little_alg(int n, int** A, int limit, int& record, int** PCSol, int** CSol, int num) // Little's algorythm
{
    int sumMinRow;
    sumMinRow = DecreaseRows(n, A);

    int sumMinCol;
    sumMinCol = DecreaseColumns(n, A);

    int newLimit = limit + sumMinRow + sumMinCol;

    if (newLimit < record)
    {
        int i_max = 0, j_max = 0;
        CheckZeros(n, A, i_max, j_max);

        PCSol[num][0] = A[i_max][0];
        PCSol[num][1] = A[0][j_max];

        int** Amn = new int* [n];
        for (int i = 0; i < n; ++i)
            Amn[i] = new int[n];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                Amn[i][j] = A[i][j];
            }
        }
        Edge_matrix(n, Amn, i_max, j_max);

        if (n > 4) // begin recursion
            Little_alg(n - 1, Amn, newLimit, record, PCSol, CSol, num + 1);

        else // add rest arcs
        {
            int lastLimit = newLimit;
            for (int i = 1; i < n - 1; ++i)
            {
                for (int j = 1; j < n - 1; ++j)
                {
                    if ((Amn[i][j] != _inf) && (Amn[i][j] != _inf2))
                    {
                        ++num;
                        PCSol[num][0] = Amn[i][0];
                        PCSol[num][1] = Amn[0][j];
                        lastLimit += Amn[i][j];
                    }
                }
            }

            for (int i = 0; i < num + 1; ++i)
            {
                CSol[i][0] = PCSol[i][0];
                CSol[i][1] = PCSol[i][1];
            }

            record = lastLimit;
            num -= 2;
        }

        // recursion
        int** A_mn = new int* [n];
        for (int i = 0; i < n; ++i)
            A_mn[i] = new int[n];
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                A_mn[i][j] = A[i][j];
            }
        }
        A_mn[i_max][j_max] = _inf2;

        Little_alg(n, A_mn, newLimit, record, PCSol, CSol, num);
    }
};

int main() {
    srand(time(0));
    setlocale(LC_ALL, "Russian");
    int n;

    short a;
    cout << "Как использовать алгоритм?" << endl << "1. Показать шаги" << endl << "2. Среднее время для n вершин" << endl;
    cout << "Ваш выбор: ";
    cin >> a;
    if (a != 1 and a != 2) {
        cout << "Неверный ввод \n";
        return 0;
    }
    cout << "Введите n: ";
    cin >> n;
    cout << endl;
    if (n < 3)
    {
        cout << "Некорректные данные!" << endl;
        return 0;
    }

    int** A = new int* [n + 1]; 
    for (int i = 0; i < n + 1; ++i)
        A[i] = new int[n + 1];

    int** CandidateSolution = new int* [n + 1]; 
    for (int i = 0; i < n + 1; ++i)
        CandidateSolution[i] = new int[2];

    int** Solution = new int* [n + 1]; 
    for (int i = 0; i < n + 1; ++i)
        Solution[i] = new int[2];

    int limit, 
        record; 

    int number;

    switch (a) {
    case 1:
        A = Generator(n);
        number = 0;
        limit = 0;
        record = _inf;
        for (int i = 0; i < n + 1; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                CandidateSolution[i][j] = 0;
                Solution[i][j] = 0;
            }
        }
        Little_alg_with_steps(n + 1, A, limit, record, CandidateSolution, Solution, number);
        cout << "Вес цикла: " << record << endl;
        cout << "Решение: " << endl;
        for (int i = 0; i < n; ++i)
        {
            cout << "(" << Solution[i][0] << ";" << Solution[i][1] << ")" << endl;
        }
        break;
    case 2:
        double t = 0;
        clock_t start, end;

        int q = 100;
        for (int k = 0; k < q; ++k)
        {
            A = Generator(n);
            number = 0;
            limit = 0;
            record = _inf;
            for (int i = 0; i < n + 1; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    CandidateSolution[i][j] = 0;
                    Solution[i][j] = 0;
                }
            }
            start = clock();
            Little_alg(n + 1, A, limit, record, CandidateSolution, Solution, number);
            end = clock();
            t += (double)(end - start) / CLOCKS_PER_SEC;
        }
        t /= q;
        cout << "Среднее время: " << t << endl;
        break;
    }

    return 0;
}
