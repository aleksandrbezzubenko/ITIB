#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

class Neuron
{
private:
	std::vector<double> synaptic_weights;
	std::vector<bool> target_function;
	std::vector<std::vector<bool>> center_coordinates;
	std::vector<double> rbf_outputs;
	double learning_rate;
	std::vector<int> set_indices;
	bool is_trained;
	std::vector<bool> inputs;
public:

	Neuron(std::vector<bool> _target_function, double _learning_rate)
		: target_function(_target_function), learning_rate(_learning_rate), is_trained(false)
	{
	}

	double Result(bool sigmoid_f)
	{
		double Result = 0;
		if (sigmoid_f) {
			Result = round(Sigmoid_function(f_net()));
		}
		else {
			Result = Step_function(f_net());
		}
		return Result;
	}

	void Get_synaptic_weights()
	{
		std::vector<int> indexes_1 = {};
		int count_1 = 0;
		std::vector<int> indexes_0 = {};
		int count_0 = 0;
		for (int i = 0; i < target_function.size(); ++i) {
			if (target_function[i] == 0) {
				++count_0;
				indexes_0.push_back(i);
			}
			else {
				++count_1;
				indexes_1.push_back(i);
			}
		}
		if (count_1 < count_0) {
			synaptic_weights.resize(count_1 + 1);
			set_indices = indexes_1;
		}
		else {
			synaptic_weights.resize(count_0 + 1);
			set_indices = indexes_0;
		}
	}

	void Get_center_coordinates()
	{
		for (int i = 0; i < set_indices.size(); ++i) {
			std::vector<bool> vec(4);
			vec[0] = (set_indices[i] / 8) % 2;
			vec[1] = (set_indices[i] / 4) % 2;
			vec[2] = (set_indices[i] / 2) % 2;
			vec[3] = set_indices[i] % 2;
			center_coordinates.push_back(vec);
		}
	}

	void Get_rbf_outputs()
	{
		rbf_outputs.resize(4);
		for (int i = 0; i < center_coordinates.size(); ++i) {
			double sum = 0;
			for (int j = 0; j < inputs.size(); ++j) {
				sum += pow((inputs[j] - center_coordinates[i][j]), 2);
			}
			rbf_outputs[i + 1] = exp((-1) * sum);
		}
		rbf_outputs[0] = 1;
	}

	double f_net()
	{
		double result = 0;
		for (int i = 0; i < rbf_outputs.size(); ++i) {
			result += synaptic_weights[i] * rbf_outputs[i];
		}
		return result;
	}

	bool Step_function(double net)
	{
		if (net >= 0) {
			return 1;
		}
		else {
			return 0;
		}
	}

	void Print_Result(std::vector<bool> Y, int E, int i)
	{
		std::cout << "Epoch " << i << ": Y = {";
		for (auto y : Y) {
			std::cout << y << " ";
		}
		std::cout << "}; W = {";
		for (auto w : synaptic_weights) {
			std::cout << w << ", ";
		}
		std::cout << "}; C = {";
		for (auto centers : center_coordinates) {
			std::cout << "{";
			for (auto c : centers) {
				std::cout << c << ", ";
			}
			std::cout << "}";
		}
		std::cout << "}; E = " << E << std::endl;
	}

	double Sigmoid_function(double net)
	{
		return 0.5 * (tanh(net) + 1);
	}

	double Derivative_sigmoid_function(double net)
	{
		return 0.5 * (1 - pow(tanh(net), 2));
	}

	void Reset()
	{
		synaptic_weights = { 0,0,0,0,0 };
		set_indices.clear();
		center_coordinates.clear();
		inputs.clear();
		is_trained = false;
	}

	void Training(bool is_sigmoid_tranning, std::string characteristicVector, bool print)
	{
		Get_synaptic_weights();
		Get_center_coordinates();
		if (print) {
			if (is_sigmoid_tranning) {
				std::cout << "Sigmoid function training" << std::endl;
			}
			else {
				std::cout << "Step function training" << std::endl;
			}
		}

		int epochs = 100;
		inputs.resize(4);
		for (int i = 0; i < epochs; ++i) {
			int E = 0;
			std::vector<bool> output = {};

			for (int j = 0; j < 16; ++j) {

				inputs[0] = (j / 8) % 2;
				inputs[1] = (j / 4) % 2;
				inputs[2] = (j / 2) % 2;
				inputs[3] = j % 2;
				Get_rbf_outputs();
				double Y = Result(is_sigmoid_tranning);
				output.push_back(Y);

				if (Y == target_function[j]) {
					continue;
				}
				else {
					++E;

					if (characteristicVector[j] != '0') {

						double delta = target_function[j] - Y;
						double net = f_net();
						for (int k = 0; k < inputs.size(); ++k) {
							if (is_sigmoid_tranning) {
								synaptic_weights[k] += learning_rate * delta *
									Derivative_sigmoid_function(net) * rbf_outputs[k];
							}
							else {
								synaptic_weights[k] += learning_rate * delta * rbf_outputs[k];
							}
						}

					}
				}
			}

			if (print) {
				Print_Result(output, E, i);
			}

			if (E == 0) {
				is_trained = true;
				break;
			}
		}
	}
	bool Next_set(std::vector<int>& a, int n, int m)
	{
		int k = m;
		for (int i = k - 1; i >= 0; --i)
			if (a[i] < n - k + i + 1)
			{
				++a[i];
				for (int j = i + 1; j < k; ++j)
					a[j] = a[j - 1] + 1;
				return true;
			}
		return false;
	}
	void Tranning_partly(bool activation_function_is_sigmoid)
	{
		std::cout << "Subset training" << std::endl;
		std::string characteristic_vector = "0000000000000000";
		int count_all_sets = 16;
		std::vector<int> all_sets(count_all_sets);

		for (int i = 0; i < count_all_sets; i++)
			all_sets[i] = i;

		for (int i = 2; i < count_all_sets; ++i) {
			bool flag = false;

			for (int i = 0; i < count_all_sets; i++)
				all_sets[i] = i;

			while (Next_set(all_sets, count_all_sets, i)) {
				characteristic_vector = "0000000000000000";

				for (int j = 0; j < i; ++j) {
					characteristic_vector[all_sets[j]] = '1';
				}

				bool print_result = false;
				Reset();
				Training(activation_function_is_sigmoid, characteristic_vector, print_result);

				if (is_trained) {
					std::cout << "Minimal training vector size: " << std::count(characteristic_vector.begin(), characteristic_vector.end(), '1') << std::endl;

					std::cout << "The subset:" << std::endl;
					for (int i = 0; i < characteristic_vector.size(); ++i)
					{
						if (characteristic_vector[i] == '1') {
							std::cout << "{ " << (i / 8) % 2 << ", " << (i / 4) % 2 << ", " << (i / 2) % 2 << ", " << i % 2 << " } ";
						}
					}
					std::cout << std::endl;
					Reset();
					print_result = true;
					Training(activation_function_is_sigmoid, characteristic_vector, print_result);
					flag = true;
					break;
				}

			}
			if (flag) {
				break;
			}
		}
		if (!is_trained) {
			std::cout << "Can't be trained" << std::endl;
		}
	}
};
