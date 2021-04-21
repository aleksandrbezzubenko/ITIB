#include "NeuralNet.hpp"

int main()
{
    NeuralNet ner({ 1, -1 }, {0.2, -0.1}, 2, 1, 2, 1);
    ner.Tranning();

}