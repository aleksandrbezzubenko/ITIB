#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>

class NeuralNet
{
private:
    std::vector<double> input;
    std::vector<double> target;
    int N;
    int J;
    int M;
    double learning_rate;
    std::vector<std::vector<double>> hidden_layer_weights;
    std::vector<std::vector<double>> output_layer_weights;
    double Net_hidden_layer(int index)
    {
        double net = hidden_layer_weights[index][0];
        for (int i = 0; i < N; ++i) {
            net += hidden_layer_weights[index][i + 1] * input[i];
        }
        return net;
    }
    double Net_output_layer(int index, std::vector<double> input_out_layer)
    {
        double net = output_layer_weights[index][0];
        for (int i = 0; i < J; ++i) {
            net += output_layer_weights[index][i + 1] * input_out_layer[i];
        }
        return net;
    }
    double Sigmoid_function(double net)
    {
        return (1 - exp((-1) * net)) / (1 + exp((-1) * net));
    }

    double Derivative_sigmoid_function(double net)
    {
        return 0.5 * (1 - pow(Sigmoid_function(net),2));
    }
    
    double Get_epsilon(std::vector<double> y)
    {
        double epsilon = 0;
        for (int i = 0; i < target.size(); ++i) {
            epsilon += pow((target[i] - y[i]), 2);
        }
        return sqrt(epsilon);
    }

    double Get_sum(int index, std::vector<double> vec_delta)
    {
        double result = 0;
        for (int i = 0; i < vec_delta.size(); ++i) {
            result += output_layer_weights[i][index] * vec_delta[i];
        }
        return result;
    }

    void Print(std::vector<double> result, double epsilon, int K)
    {
        std::cout << "F(y) = {";
        for (int i = 0; i < result.size(); ++i) {
            std::cout << result[i];
            if (i == 0) {
                std::cout << ", ";
            }
        }
        std::cout << "}    E(" << K << ") = " << epsilon << std::endl;
    }
public:
    NeuralNet(std::vector<double> _input, std::vector<double> _target,
        int _N, int _J, int _M, double _learning_rate) : input(_input),
        target(_target), N(_N), J(_J), M(_M), learning_rate(_learning_rate)
    {
        hidden_layer_weights.resize(J);
        for (int i = 0; i < hidden_layer_weights.size(); ++i) {
            hidden_layer_weights[i].resize(N + 1);
        }
        output_layer_weights.resize(M);
        for (int i = 0; i < M; ++i) {
            output_layer_weights[i].resize(J + 1);
        }
    }

    void Tranning()
    {
        double epsilon = 1;
        int K = 0;
        while (epsilon > 0.001) {
            std::vector<double> all_net_in_hidden_layer = {};
            std::vector<double> all_net_in_out_layer = {};
            std::vector<double> input_in_out_layer(J + 1);
            std::vector<double> out = {};
            for (int i = 0; i < J; ++i) {
                double net_in_hidden = Net_hidden_layer(i);
                all_net_in_hidden_layer.push_back(net_in_hidden);
                input_in_out_layer[i] = Sigmoid_function(net_in_hidden);
            }
            for (int i = 0; i < M; ++i) {
                double net_in_out = Net_output_layer(i, input_in_out_layer);
                all_net_in_out_layer.push_back(net_in_out);
                out.push_back(Sigmoid_function(net_in_out));
            }

            std::vector<double> all_hidden_error = {};
            std::vector<double> all_out_error = {};
            for (int i = 0; i < all_net_in_out_layer.size(); ++i) {
                double derivative = Derivative_sigmoid_function(all_net_in_out_layer[i]);
                double delta = derivative * (target[i] - out[i]);
                all_out_error.push_back(delta);
            }
            for (int i = 0; i < all_net_in_hidden_layer.size(); ++i) {
                double derivative = Derivative_sigmoid_function(all_net_in_hidden_layer[i]);
                double delta = derivative * (target[i] - out[i]);
                all_hidden_error.push_back(delta);
            }

            for (int i = 0; i < J; ++i) {
                hidden_layer_weights[i][0] += learning_rate * all_hidden_error[i];
                for (int j = 0; j < N; ++j) {
                    hidden_layer_weights[i][j + 1] += learning_rate * input[j] * all_hidden_error[i];
                }
            }
            for (int i = 0; i < M; ++i) {
                output_layer_weights[i][0] += learning_rate * all_out_error[i];
                for (int j = 0; j < J; ++j) {
                    output_layer_weights[i][j + 1] += learning_rate * input_in_out_layer[j] * all_out_error[i];
                }
            }

            ++K;
            epsilon = Get_epsilon(out);
            Print(out, epsilon, K);
            if (out == target) {
                break;
            }
        }
    }
};