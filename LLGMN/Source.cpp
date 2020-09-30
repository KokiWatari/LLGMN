#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include"Layer.h"
using namespace std;
vector<vector<double>> get_vector_from_file(string filename);

int main() {

	// ƒf[ƒ^“Ç‚İæ‚è•”
	/*
	vector<vector<double>> input_data = get_vector_from_file("lea_sig");
	vector<vector<double>> output_data = get_vector_from_file("lea_T_sig");
	vector<vector<double>> test_input_data = get_vector_from_file("dis_sig.csv");
	vector<vector<double>> test_output_data = get_vector_from_file("dis_T_sig.csv");
	*/
	








	return 0;
}

vector<vector<double>> get_vector_from_file(string filename) {
	ifstream ifs(filename);
	if (ifs.fail()) {
		std::cerr << "¸”s" << std::endl;
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