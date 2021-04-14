#include <iostream>
#include <cmath>
#include <vector>

class Neuron
{
private:
    int N;
    std::vector<double> weights;
    std::vector<double> real_values;
    std::vector<double> values;


    double Net(int index, int p)
    {
        double net = weights[0];
        for (int i = 1; i < p; ++i) {
            net += weights[i] * real_values[index - p + i - 1];
        }
        return net;
    }
    void Print_result(int epoch, double error)
    {
        std::cout << "Epoch: " << epoch << "; Error = " << error << std::endl;
        std::cout << "Weights: ";
        for (auto weight : weights) {
            std::cout << weight << ' ';
        }
        std::cout << std::endl;
    }

public:
    Neuron(int _N)
        : N(_N)
    {}
    void Function(double a, double b, int count_points)
    {
        double step = (b - a) / count_points;
        for (int i = 0; i < count_points; ++i) {
            double x = a + step * i;
            double func = sqrt(0.1 * x) + 1;
            real_values.push_back(func);
        }
    }
    void Train(int epochs, int p, double learning_rate)
    {
        weights.resize(p + 1);
        values.resize(N);
        double error = 0;
        for (int i = 0; i < epochs; ++i) {
            for (int j = p; j < N; ++j) {

                values[j] = Net(j, p);

                double delta = real_values[j] - values[j];
                for (int k = 1; k < p + 1; ++k) {
                    weights[k] += learning_rate * delta *
                        real_values[j - p + k];
                }
                weights[0] = learning_rate * delta;

                error = 0;
                for (int k = p; k < N; ++k) {
                    error += pow(delta, 2);
                }
                error = sqrt(error);

            }

        }
        Print_result(epochs, error);
    }

    void Forecasting(int p)
    {
        values.resize(N);
        for (int i = N + 1; i < 2 * N + 1; ++i) {
            values[i - N - 1] = Net(i, p);
        }
    }

    std::vector<double> Get_values() { return values; }
    std::vector<double> Get_real_values() { return real_values; }
    void Clear_func() { real_values.clear(); }
};

int main()
{
    int a = 1;
    int b = 5;
    int N = 20; // кол-во частей, на которые разбивается отрезок

    Neuron ner(N);

    std::cout << std::endl << "Change window size" << std::endl;
    ner.Function(1, 5, 20);
    int p = 2;  // длина окна
    while (p <= 10) {
        std::cout << std::endl << "learning rate = " << 0.3 << std::endl;
        std::cout << "Window size = " << p << std::endl;
        ner.Train(5000, p, 0.3);
        ++p;
    }
    ner.Clear_func();

    Neuron ner1(N);

    std::cout << "Change learning rate" << std::endl;
    ner1.Function(1, 5, 20);
    double learning_rate = 0.1; // норма обучения
    while (learning_rate <= 1) {
        std::cout << std::endl << "learning rate = " << learning_rate << std::endl;
        std::cout << "Window size = " << 5 << std::endl;
        ner1.Train(5000, 2, learning_rate);
        learning_rate += 0.1;
    }
    ner1.Clear_func();

    Neuron ner2(N);

    std::cout << std::endl << "Change count epochs" << std::endl;
    ner2.Function(1, 5, 20);
    int epochs = 500; // кол-во эпох
    while (epochs <= 10000) {
        std::cout << std::endl << "learning rate = " << 0.3 << std::endl;
        std::cout << "Window size = " << 5 << std::endl;
        ner2.Train(epochs, 5, 0.3);
        epochs += 500;
    }
    ner2.Clear_func();

    Neuron ner3(N);
    ner3.Function(1, 9, 40);
    ner3.Train(5000, 5, 0.3);
    ner3.Forecasting(5);
    std::vector<double> forecasting = ner3.Get_values();
    std::vector<double> real_v = ner3.Get_real_values();
    std::cout << "Real_values: ";
    for (int i = N; i < 2 * N; ++i) {
        std::cout << real_v[i] << ' ';
    }
    std::cout << std::endl << std::endl << "Forecasting: ";
    for (int i = 0; i < N; ++i) {
        std::cout << forecasting[i] << ' ';
    }
}