#include <iostream>

class calculator {
private:
	double num1;
		double num2;

public:
		// 생성자
		calculator(double a, double b) : num1(a), num2(b) {}

    double add() {
        return num1 + num2;
    }

    double subtract() {
        return num1 - num2;
    }

    double multiply() {
        return num1 * num2;
    }

    double divide() {
        if (num2 != 0) {
            return num1 / num2;
        } else {
            std::cerr << "Error: Division by zero!" << std::endl;
            return 0; // 에러 처리
        }
    }
	
	void sumistrue() {
	int nana = 0;
	int haha = 0;
	}
};

int main() {
 double a, b;

   std::cout << "첫 번째 숫자를 입력하세요: ";
  std::cin >> a;
  std::cout << "두 번째 숫자를 입력하세요: ";
  std::cin >> b;

		calculator calc(a, b);

 std::cout << "덧셈: " << calc.add() << std::endl;
 std::cout << "뺄셈: " << calc.subtract() << std::endl;
 std::cout << "곱셈: " << calc.multiply() << std::endl;
 std::cout << "나눗셈: " << calc.divide() << std::endl;

 return 0;
}
