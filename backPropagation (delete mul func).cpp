// backPropagation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

class Variable {
public:
    double value;
    double gradient;
    std::vector<std::pair<Variable*,Variable*(*)(Variable*)>> parents;
    char op;
    Variable( double _value, std::vector<std::pair<Variable*, Variable*(*)(Variable*)>> _parents = {}, char _op = 0 ) {
        this->value = _value;
        this->gradient = 0.0;
        this->parents = _parents;
        this->op = _op;
    }

    ~Variable() {
        delete this;
    }

    void backward( double grad = 1.0 ) {
        this->gradient += grad;
        for( std::pair<Variable*, Variable*(*) (Variable*)> parent : this->parents ) {
            parent.first->backward((grad * parent.second(this)->value));
        }
    }

    template <typename T> Variable operator +( T other ) {
        Variable* _other;
        if (std::string(typeid(other).name()).compare("class Variable") == 0) {
            _other = (Variable*)&other;
        }
        else {
            _other = new Variable((double)other);
        }
        return Variable(this->value + _other->value, { std::make_pair(this,[](Variable*)->Variable*{return new Variable(1);}), std::make_pair(_other,[](Variable*)->Variable* {return new Variable(1);})},'+');
    }
    
    template <typename T> Variable operator*( T other ) {
        Variable* _other;
        if (std::string(typeid(other).name()).compare("class Variable") == 0) {
            _other = (Variable*)&other;
        }
        else {
            _other = new Variable((double)other);
        }
        return Variable(this->value * _other->value, std::vector<std::pair<Variable*, Variable*(*)(Variable*)>>{ std::make_pair(this, [=](Variable*)->Variable* {return new Variable(_other->value);}), std::make_pair(_other, [=](Variable*)-> Variable* { return new Variable(this->value);})}, '*');
    }

};

int main()
{
    Variable* x = new Variable( 3 ); //class Variable
    //Variable y( 4 );
    //Variable* y = new Variable( 4 );

    Variable* z = new Variable((*x+2)*5);

    z->backward();



    std::cout<< "Valor de z: " << z->value << "\nGradiente de z con respecto a x: "<<x->gradient;
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
