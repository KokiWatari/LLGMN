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
    //D:入力次元、M:コンポーネント数、K:クラス数、H:非線形変換後の次元
    const int D, M, K, H;
    // input layer
    vector<double> x;
    vector<double> input1;
    vector<vector<double>> input2;
    vector<double> input3;
    // output layer
    vector<double> output1;
    vector<vector<double>> output2;
    vector<double> y;
    // weight
    vector<vector<vector<double>>> weight;
    //微分
    vector<vector<vector<double>>> dj_dw;
    //学習率
    const double eta;


public:
    Layer(int D,int K,int M,double eta);
    virtual ~Layer() {};


    vector<double> forward(vector<double> input_data) {
        //非線形変換
        x[0] = 1;
        for (int i = 0; i < D; i++) {
            x[i+1] = input_data[i];
        }
        int a = 1;
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < D; j++) {
                if (i <= j) {
                    x[D + a] = input_data[i] * input_data[j];
                    a += 1;
                }
            }
        }
        //第１層
        for (int h = 0; h < H; h++) {
            input1[h] = x[h];
            output1[h] = input1[h];
        }
        //第２層
        double sum = 0;
        vector<vector<double>> z2(K, vector<double>(M, 0));
        input2 = z2;
        for (int k = 0; k < K; k++) {
            for (int m = 0; m < M; m++) {
                if ((k == K) && (m == M)) {
                    for (int h = 0; h < H; h++) {
                        weight[h][K][M] = 0;
                    }
                    input2[k][m] = 0;
                }
                else {
                    for (int h = 0; h < H; h++) {
                        input2[k][m] += output1[h] * weight[h][k][m];
                    }
                }
                sum += exp(input2[k][m]);
            }
        }
        for (int k = 0; k < K; k++) {
            for (int m = 0; m < M; m++) {
                output2[k][m] = exp(input2[k][m]) / sum;
            }
        }
        //第３層
        vector<double> z3(K, 0);
        input3 = z3;
        for (int k = 0; k < K; k++) {
            for (int m = 0; m < M; m++) {
                input3[k] += output2[k][m];
            }
            y[k] = input3[k];
        }
        return y;
    }


    void learn_online(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
        srand((unsigned int)time(NULL));
        double error = 100;
        for (int times = 0; times < 10001 && error / input_data.size() > 0.01; ++times) {
            error = 0;
            for (int n = 0; n < input_data.size(); n++) {
                int a = rand() % input_data.size();
                vector<double> Y = forward(input_data[a]);
                for (int k = 0; k < K; k++) {
                    for (int m = 0; m < M; m++) {
                        for (int h = 0; h < H; h++) {
                            dj_dw[h][k][m] = (Y[k] - input_label[a][k]) * (output2[k][m] / Y[k]) * x[h];
                            //重みの更新(逐次)
                            weight[h][k][m] -= eta * dj_dw[h][k][m];
                        }
                    }
                }
                //評価関数(交差エントロピー誤差)の計算
                double j = 0;
                for (int k = 0; k < K; k++) {
                    j += input_label[a][k] * log(Y[k]);
                }
                error -= j;
            }
            if (times % 10 == 0) {
                cout << "times:" << times << "  error:" << error / input_data.size() << endl;
            }
        }
    }


    void learn_patch(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
        double error = 100;
        for (int times = 0; times < 10001 && error / input_data.size() > 0.01; ++times) {
            error = 0;
            vector<vector<vector<double>>> dj_dw_sum(H, vector<vector<double>>(K, vector<double>(M, 0)));
            for (int n = 0; n < input_data.size(); n++) {
                vector<double> Y = forward(input_data[n]);
                for (int k = 0; k < K; k++) {
                    for (int m = 0; m < M; m++) {
                        for (int h = 0; h < H; h++) {
                            dj_dw[h][k][m] = (Y[k] - input_label[n][k]) * (output2[k][m] / Y[k]) * x[h];
                            dj_dw_sum[h][k][m] += dj_dw[h][k][m];
                        }
                    }
                }
                //評価関数(交差エントロピー誤差)の計算
                double j = 0;
                for (int k = 0; k < K; k++) {
                    j += input_label[n][k] * log(Y[k]);
                }
                error -= j;
            }
            //重みの更新(一括)
            for (int k = 0; k < K; k++) {
                for (int m = 0; m < M; m++) {
                    for (int h = 0; h < H; h++) {
                        weight[h][k][m] -= eta * dj_dw_sum[h][k][m] / input_data.size();
                    }
                }
            } 
            if (times % 10 == 0) {
                cout << "times:" << times << "  error:" << error / input_data.size() << endl;
            }
        }    
    }


    void init_weight() {
        srand((unsigned int)time(NULL));
        for (int k = 0; k < K; k++) {
            for (int m = 0; m < M; m++) {
                for (int h = 0; h < H; h++) {
                    weight[h][k][m] = (double)rand() / RAND_MAX;
                }
            }
        }
    }



};
#endif LAYER_H