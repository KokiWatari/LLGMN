#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include"Layer.h"
#define huge 100
using namespace std;
vector<vector<double>> get_vector_from_file(string filename);

Layer::Layer(int D, int K, int M, double eta)
	:D(D)
	,K(K)
	,M(M)
	,eta(eta)
{
	x = vector<double>(1,1);
	input1 = vector<double>(huge);
	input2 = vector<vector<double>>(K, vector<double>(M, 0));
	input3 = vector<double>(K);
	output1 = vector<double>(huge);
	output2 = vector<vector<double>>(K, vector<double>(M, 0));
	y = vector<double>(K);
	weight = vector<vector<vector<double>>>(huge, vector<vector<double>>(K, vector<double>(M, 0)));
	dj_dw = vector<vector<vector<double>>>(huge, vector<vector<double>>(K, vector<double>(M, 0)));
	init_weight();
}


int main() {
	int d, k, m,is_online;
	cout << "入力次元(今回は800):";
	cin >> d;
	cout << "クラス数(今回は4):";
	cin >> k;
	cout << "コンポーネント数:";
	cin >> m;
	cout << "一括学習:0, 逐次学習:1" << endl << ":";
	cin >> is_online;
	// データ読み取り部
	vector<vector<double>> input_data = get_vector_from_file("lea_sig.csv");
	vector<vector<double>> input_label = get_vector_from_file("lea_T_sig.csv");
	vector<vector<double>> test_input_data = get_vector_from_file("dis_sig.csv");
	vector<vector<double>> test_input_label = get_vector_from_file("dis_T_sig.csv");
	//Layerクラス　第4引数が学習率
	Layer layer(d, k, m, 0.05);

	if (is_online) {
		layer.learn_online(input_data, input_label);
	}
	else {
		layer.learn_patch(input_data, input_label);
	}
	
	vector<vector<double>> output(input_label.size(), vector<double>(input_label[0].size()));
	printf("学習データの出力結果\n");
	for (int i = 0; i < input_data.size(); i++) {
		output[i] = layer.forward(input_data[i]);
		for (int j = 0; j < input_label[0].size(); j++) {
			printf("%lf ,", output[i][j]);
		}
		printf("\n");
	}

	vector<vector<double>> test_output(test_input_label.size(), vector<double>(test_input_label[0].size()));
	printf("テストデータの出力結果\n");
	for (int i = 0; i < test_input_data.size(); i++) {
		test_output[i] = layer.forward(test_input_data[i]);
		for (int j = 0; j < test_input_label[0].size(); j++) {
			printf("%lf ,", test_output[i][j]);
		}
		printf("\n");
	}


	return 0;
}

vector<vector<double>> get_vector_from_file(string filename) {
	ifstream ifs(filename);
	if (ifs.fail()) {
		std::cerr << "失敗" << std::endl;
	}
	string str, str1;
	vector<vector<double>> data;
	while (getline(ifs, str)) {
		stringstream ss{ str };
		vector<double> tmp;
		while (getline(ss, str1, ',')) {
			tmp.push_back(stod(str1));
		}
		data.push_back(tmp);
	}
	return data;
}