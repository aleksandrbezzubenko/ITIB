#include "Neuron.hpp"

int main()
{
	std::vector<bool> target_function = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1 };
	std::string characteristic_vector = "1111111111111111";
	Neuron neuron(target_function, 0.3);
	bool activation_function_is_sigmoid = false;
	bool print_result = true;
	neuron.Training(activation_function_is_sigmoid, characteristic_vector, print_result);
	neuron.Reset();
	activation_function_is_sigmoid = true;
	neuron.Training(activation_function_is_sigmoid, characteristic_vector, print_result);
	neuron.Reset();
	neuron.Tranning_partly(activation_function_is_sigmoid);
	activation_function_is_sigmoid = false;
	neuron.Reset();
	neuron.Tranning_partly(activation_function_is_sigmoid);
}