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
    //D:���͎����AM:�R���|�[�l���g���AK:�N���X��
    const int D, M, K;
    //H:����`�ϊ���̎���
    int H;
    // input layer
    vector<double> input1;
    vector<vector<double>> input2;
    vector<double> input3;
    // output layer
    vector<double> x;
    vector<double> output1;
    vector<vector<double>> output2;
    vector<double> y;
    // weight
    vector<vector<vector<double>>> weight;
    //����
    vector<vector<vector<double>>> dj_dw;
    //�w�K��
    const double eta;


public:
    Layer(int D,int K,int M,double eta);
    virtual ~Layer() {};

    vector<double> forward(vector<double> input_data) {
        //����`�ϊ�




        //��P�w
        for (int h = 0; h < H; h++) {
            input1[h] = x[h];
            output1[h] = input1[h];
        }
        //��Q�w
        double sum = 0;
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
        //��R�w
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
        double evaluation = -100;
        for (int times = 0; times < 10001 && evaluation / input_data.size() < -0.01; ++times) {
            evaluation = 0;
            for (int d = 0; d < input_data.size(); d++) {
                int a = rand() % input_data.size();
                vector<double> Y = forward(input_data[a]);
                for (int k = 0; k < K; k++) {
                    for (int m = 0; m < M; m++) {
                        for (int h = 0; h < H; h++) {
                            dj_dw[h][k][m] = (Y[k] - input_label[a][k]) * (output2[k][m] / Y[k]) * x[h];
                            //�d�݂̍X�V(����)
                            weight[h][k][m] -= eta * dj_dw[h][k][m];
                        }
                    }
                }
                //�]���֐�(�����G���g���s�[�덷)�̌v�Z
                double j = 0;
                for (int k = 0; k < K; k++) {
                    j += input_label[a][k] * log(Y[k]);
                }
                evaluation -= j;
            }
            if (times % 10 == 0) {
                cout << "times:" << times << "evaluation, :" << evaluation / input_data.size() << endl;
            }
        }
    }


    void learn_patch(vector<vector<double>>& input_data, vector<vector<double>>& input_label) {
        double evaluation = -100;
        double dj_dw_sum1 = 100;
        double dj_dw_sum2 = 100;
        for (int times = 0; times < 10001 && evaluation / input_data.size() < -0.01; ++times) {
            evaluation = 0;
            dj_dw_sum2 = 0;
            for (int d = 0; d < input_data.size(); d++) {
                vector<double> Y = forward(input_data[d]);
                dj_dw_sum1 = 0;
                for (int k = 0; k < K; k++) {
                    for (int m = 0; m < M; m++) {
                        for (int h = 0; h < H; h++) {
                            dj_dw[h][k][m] = (Y[k] - input_label[d][k]) * (output2[k][m] / Y[k]) * x[h];
                            dj_dw_sum1 += dj_dw[h][k][m];
                        }
                    }
                }
                dj_dw_sum2 += dj_dw_sum1 / (H * K * M);
                //�]���֐�(�����G���g���s�[�덷)�̌v�Z
                double j = 0;
                for (int k = 0; k < K; k++) {
                    j += input_label[d][k] * log(Y[k]);
                }
                evaluation -= j;
            }
            //�d�݂̍X�V(�ꊇ)
            for (int k = 0; k < K; k++) {
                for (int m = 0; m < M; m++) {
                    for (int h = 0; h < H; h++) {
                        weight[h][k][m] -= eta * dj_dw_sum2 / input_data.size();
                    }
                }
            }
            if (times % 10 == 0) {
                cout << "times:" << times << "evaluation, :" << evaluation / input_data.size() << endl;
            }
        }    
    }


    void init_weight() {
        srand((unsigned int)time(NULL));
        for (int k = 0; k < K; k++) {
            for (int m = 0; m < M; m++) {
                for (int h = 0; h < H; h++) {
                    weight[h][k][m] = rand() % 2;
                }
            }
        }
    }



};
#endif LAYER_H