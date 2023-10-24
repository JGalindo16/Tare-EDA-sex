#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

map<string, double> variables;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isOperand(char c) {
    return isdigit(c);
}

int precedence(char op) {
    if (op == '^')
        return 3;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return -1;
}

string infixToPostfix(string expression) {
    string postfix;
    stack<char> st;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (c == ' ') continue;

        if (isalpha(c)) {
            string var;
            var.push_back(c);
            while (i + 1 < expression.length() && isalpha(expression[i + 1]))
                var.push_back(expression[++i]);
            postfix.append(to_string(variables[var]));
            postfix.push_back(' ');
        } else if (isOperand(c)) {
            string operand;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                operand.push_back(expression[i]);
                i++;
            }
            i--;
            postfix.append(operand);
            postfix.push_back(' ');
        } else if (isOperator(c)) {
            while (!st.empty() && st.top() != '(' && precedence(st.top()) >= precedence(c)) {
                postfix.push_back(st.top());
                postfix.push_back(' ');
                st.pop();
            }
            st.push(c);
        } else if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                postfix.push_back(st.top());
                postfix.push_back(' ');
                st.pop();
            }
            st.pop();
        }
    }

    while (!st.empty()) {
        postfix.push_back(st.top());
        postfix.push_back(' ');
        st.pop();
    }

    return postfix;
}

double evaluatePostfix(string postfix) {
    stack<double> st;
    istringstream iss(postfix);
    string token;

    while (iss >> token) {
        if (isOperand(token[0])) {
            st.push(stod(token));
        } else if (isOperator(token[0])) {
            double val1 = st.top();
            st.pop();
            double val2 = st.top();
            st.pop();

            switch (token[0]) {
                case '+':
                    st.push(val2 + val1);
                    break;
                case '-':
                    st.push(val2 - val1);
                    break;
                case '*':
                    st.push(val2 * val1);
                    break;
                case '/':
                    st.push(val2 / val1);
                    break;
                case '^':
                    st.push(pow(val2, val1));
                    break;
            }
        }
    }

    return st.top();
}

int main() {
    cout << "********* CALCULADORA INTERACTIVA *********" << endl;
    string input;
    while (true) {
        cout << "$ ";
        getline(cin, input);
        if (input.find('=') != string::npos) {
            string var = input.substr(0, input.find('=') - 1);
            string value = input.substr(input.find('=') + 2);
            variables[var] = stod(value);
        } else if (input == "FIN") {
            break;
        } else if (input == "ans") {
            cout << "$ ans = " << fixed << setprecision(2) << variables["ans"] << endl;
        } else if (isOperand(input[0]) || isalpha(input[0])) {
            string postfix = infixToPostfix(input);
            try {
                double result = evaluatePostfix(postfix);
                variables["ans"] = result;
            } catch (const exception &e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }
    return 0;
}









