// backPropagation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <functional>
#include <typeinfo>
#include <iostream>
#include <utility>

class Variable {
public:
    double value;
    double gradient;
    std::vector<std::pair<Variable*, std::function<Variable* (const Variable*)>>> parents;
    char op;

    Variable(double _value, std::vector<std::pair<Variable*, std::function<Variable* (const Variable*)>>> _parents = {}, char _op = 0) {
        this->value = _value;
        this->gradient = 0.0;
        this->parents = _parents;
        this->op = _op;
    }

 
    void backward(double grad = 1.0) {
        this->gradient += grad;

        for (std::pair<Variable*, std::function<Variable* (const Variable*)>> parent : this->parents) {
            Variable* lambdaFunction = parent.second(this);
            parent.first->backward((grad * lambdaFunction->value));
            delete lambdaFunction;
        }
    }

    template <typename T>
    const Variable operator+(const T& other) const {
        const Variable* _other;
        if constexpr (std::is_same_v<T, Variable>) {
            _other = (Variable*)&other;
        }
        else {
            _other = new Variable(other);
        }
        return Variable(this->value + _other->value, std::vector<std::pair<Variable*, std::function<Variable* (const Variable*)>>>{ std::make_pair(const_cast<Variable*>(this), [](const Variable*) -> Variable* { return new Variable(1); }), std::make_pair(const_cast<Variable*>(_other), [](const Variable*) -> Variable* { return new Variable(1); }) }, '+');
    }


    template <typename T>
    const Variable operator*(const T& other) const {
        const Variable* _other;
        if constexpr (std::is_same_v<T, Variable>) {
            _other = (Variable*)&other;
        }
        else {
            _other = new Variable(other);
        }
        return Variable(this->value * _other->value,
            std::vector<std::pair<Variable*, std::function<Variable* (const Variable*)>>>{ std::make_pair(const_cast<Variable*>(this), [=](const Variable*) -> Variable* { return new Variable(_other->value); }), std::make_pair(const_cast<Variable*>(_other), [=](const Variable*) -> Variable* { return new Variable(this->value); }) }, '*');
    }


    template <typename T>
    const Variable operator-(const T& other) const {
        const Variable* _other;
        if constexpr (std::is_same_v<T, Variable>) {
            _other = (Variable*)&other;
        }
        else {
            _other = new Variable(other);
        }
        return Variable(this->value - _other->value,
            std::vector<std::pair<Variable*, std::function<Variable* (const Variable*)>>>{ std::make_pair(const_cast<Variable*>(this), [](const Variable*)-> Variable* {return new Variable(1); }), std::make_pair(const_cast<Variable*>(_other), [](const Variable*)-> Variable* {return new Variable(-1); }) }, '-');
    }
};


double mean(std::vector<double> array) {
    double sum = 0.0;
    for (double element : array) {
        sum += element;
    }
    return sum / array.size();
}

void SingleNeuron() {

    //Random values for w and b (weight and bias)  
    Variable w = Variable(1.5);
    Variable b = Variable(0.4);
    
    //Epoch
    int epochs = 1400;
    
    //Learning rate
    double learning_rate = 0.001;
    
    //Data {Input, Target}
    std::vector<std::vector<double>> data = { {6.0,48.2},
                                            {15.0,59.0},
                                            {27.0,80.6},
                                            {20.0,68.0},
                                            {2.0,35.6} };
    std::vector<double> dL_dW(data.size());
    std::vector<double> dL_db(data.size());

    std::vector<double> MSEArray(epochs);
    std::vector<double> MSESubArray(data.size());

    for (unsigned long long epoch = 0ULL; epoch < epochs; ++epoch) {
    	w.gradient = 0;
    	b.gradient = 0;
        for (int i = 0; i < data.size(); ++i) {
            double p = data[i][0];
            double t = data[i][1];
            
            // p = input, w = weight, b = bias, t = target
            //(((w*p)+b)-t)^2
            
            Variable MSE = (((w*p)+b)-t) * (((w*p)+b)-t);
            
            //Variable z0 = (w * p);
            //Variable z1 = (z0 + b);
            //Variable z2 = (z1 - t);
            //Variable MSE = (z2 * z2);
            
            MSE.backward();
            MSESubArray[i] = MSE.value;
            dL_dW[i] = w.gradient;
            dL_db[i] = b.gradient;
        }
        MSEArray[epoch] = mean(MSESubArray);
        double mean_dL_dW = mean(dL_dW);
        double mean_dL_db = mean(dL_db);

        double w0 = learning_rate * mean_dL_dW;
        double b0 = learning_rate * mean_dL_db;

        w.value -= w0;
        b.value -= b0;
        std::cout << "Epoch: " << epoch + 1 << ", w = " << w.value << ", b = " << b.value << std::endl;
    }

}

int main() {

    //    int p = 5;
    //    int t = 10;
        //((w*p)+b-t)^2
    //    Variable z0 = (w*p);
      //  Variable z1 = (z0+b);
        //Variable z2 = (z1-t);
    //    Variable z = z2*z2;
    //	Variable z = w*2;
    //    z.backward();

    //    std::cout << "Valor de z: " << z.value << "\nGradiente de w: "<< w.gradient << "\nGradiente de b: "<< b.gradient <<std::endl;
    SingleNeuron();
    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
