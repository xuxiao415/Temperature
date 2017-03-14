#include <iostream>
#include <algorithm>
#include <cmath>
#include <pthread.h>

#define WIDTH 60 //矩阵大小

using namespace std;

typedef struct Args{
    int begin;
    int step;
}Args;              //cal_temperature函数的参数类型

double InitMatrix[WIDTH][WIDTH], ResultMatrix[WIDTH][WIDTH];
bool stable_flag = false;//温度矩阵稳定的参数

void *cal_temperature(void *_args);

int main (int argc, char *argv[])
{
    int np = atoi(argv[1]);
    if (np < 0 && np > (WIDTH - 2)){
        cout<<"请将线程数设置在0到之间，防止产生空线程"<<endl;
        return 0;
    }

    for (int i = 0; i < WIDTH; ++ i) {
        for (int j = 0; j < WIDTH; ++ j) {
            if (i == 0 || j == 0 || i == (WIDTH - 1) || j == (WIDTH - 1)) {
                InitMatrix[i][j] = ResultMatrix[i][j] = \
                static_cast<double>(max(i,j));
            }
        }
    }                   //温度矩阵初始化

    Args *arg;
    pthread_t *pid;
    pid = (pthread_t *)malloc(np * sizeof(pthread_t));

    while (!stable_flag) {
        stable_flag = true;
        for (int i = 1; i <= np; ++i) {
            arg = (Args *)malloc(sizeof(Args));
            arg->begin = i;
            arg->step = np;
            pthread_create(&pid[i - 1], NULL, cal_temperature, (void *)arg);
        }               //创建线程
        for (int i = 0; i < np; i++) {
            pthread_join(pid[i], NULL);//主线程等待子线程
        }
    }

    for (int i = 1; i < (WIDTH - 1); ++ i) {
        for (int j = 1; j < (WIDTH - 1); ++ j) {
            InitMatrix[i][j] = ResultMatrix[i][j];
            cout << InitMatrix[i][j] << " ";
        }
        cout << endl;                   //输出稳定的温度矩阵
    }

    int counter = 0;
    for (int i = 1; i < (WIDTH - 1); ++ i) {
        for (int j = 1; j < (WIDTH - 1); ++ j) {
            if ((InitMatrix[i][j] - 5.5) > pow(10.0, -3)) {
                counter ++;
            }
        }
    }                           //计算温度矩阵大于某温度阀值的点的个数

    cout << "\tResult :" << counter << endl;//输出个数

    free(pid);

    return 0;
}

void *cal_temperature(void *_args){
    Args *args = (Args *)_args;
    //矩阵按行划分，每个线程计算相同的行数
    for (int j = args->begin; j < (WIDTH - 1); j += args->step) {
        for (int i = 1; i < (WIDTH - 1); ++ i) {
            ResultMatrix[j][i] = \
                0.25 * (InitMatrix[j - 1][i] + \
                InitMatrix[j + 1][i] + \
                InitMatrix[j][i - 1] + \
                InitMatrix[j][i + 1]);
            if (fabs(ResultMatrix[j][i] - \
        InitMatrix[j][i]) > pow(10.0, -3)) {
                stable_flag = false;
            }
        }
    }
    for (int j = args->begin; j < (WIDTH - 1); j += args->step) {
        for (int i = 1; i < (WIDTH - 1); ++ i) {
            InitMatrix[j][i] = \
            ResultMatrix[j][i];
        }
    }
    return NULL;
}
