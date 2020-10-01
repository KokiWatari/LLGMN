#include <stdio.h>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#ifndef LAYER_H
#define LAYER_H
#include<vector>
using namespace std;
class Layer {
private:
    //D:入力次元、H:非線形変換後の次元、M:コンポーネント数、K:クラスの数
    int D, H, M, K;
    // input layer
    vector<double> input1;
    vector<vector<double>> input2;
    vector<double> input3;
    // output layer
    vector<double> X;
    vector<double> output1;
    vector<vector<double>> output2;
    vector<double> y;
    // error value
    vector<double> error;
    // weight
    vector<vector<vector<double>>> weight;
    //第2層途中式用
    double sum;
    //評価関数
    vector<double> J;
    //微分
    vector<vector<vector<double>>> dj_dw;
    vector<vector<double>> dj_dw_sum;
    //学習率
    double eta;


public:
    vector<double> forward(vector<double> input_data) {//input_data:入力ベクトル?
        //非線形変換




        //calculation of first layer
        for (int h = 0; h <= H; h++) {
            input1[h] = X[h];
            output1[h] = input1[h];
        }
        //caluculation of second layer
        for (int k = 0; k <= K; k++) {
            for (int m = 0; m <= M; m++) {
                if ((k == K) && (m == M)) {
                    input2[k][m] = 0;
                }
                else {
                    for (int h = 0; h <= H; h++) {
                        input2[k][m] += output1[h] * weight[h][k][m];
                    }
                }
                sum += exp(input2[k][m]);
            }
        }
        for (int k = 0; k <= K; k++) {
            for (int m = 0; m <= M; m++) {
                output2[k][m] = exp(input2[k][m]) / sum;
            }
        }
        //caluculation of third layer
        for (int k = 0; k <= K; k++) {
            for (int m = 0; m <= M; m++) {
                input3[k] += output2[k][m];
            }
            y[k] = input3[k];
        }
        return y;
    }

    void learn_online(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
        vector<double> Y = forward(input_data);
        for (int times = 0; times < 10001; ++times) {
   
            for (int k = 0; k <= K; k++) {
                for (int m = 0; m <= M; m++) {
                    for (int h = 0; h <= H; h++) {
                        error[k] = Y[k] - input_label[k];
                        dj_dw[h][k][m] = error[k] * (output2[k][m] / Y[k]) * X[h];
                        weight[h][k][m] -= eta * dj_dw[h][k][m];
                    }
                }
            }

        }


    }

    void learn_patch(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
        vector<double> Y = forward(input_data);
        for (int times = 0; times < 10001; ++times) {

            for (int k = 0; k <= K; k++) {
                for (int m = 0; m <= M; m++) {
                    for (int h = 0; h <= H; h++) {
                        error[k] = Y[k] - input_label[k];
                        dj_dw[h][k][m] = error[k] * (output2[k][m] / Y[k]) * X[h];
                        dj_dw_sum[k][m] += dj_dw[h][k][m];
                    }
                    for (int h = 0; h <= H; h++) {
                        weight[h][k][m] -= eta * dj_dw_sum[k][m] / H;
                    }
                }
            }

        }


    }



};
#endif LAYER_H