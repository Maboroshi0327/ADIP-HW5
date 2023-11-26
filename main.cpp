#include <iostream>

#include "HW_1/HW_1.h"
#include "HW_2/HW_2.h"
#include "HW_3/HW_3.h"

int main()
{
	char cmd_input = 0;

	while (cmd_input != 'q')
	{
		std::cout
			<< "1: 1_1 anwser\n"
			<< "2: 1_2 anwser\n"
			<< "3: 1_3 anwser\n"
			<< "4: 1_4 anwser\n"
			<< "5: 1_5 anwser\n"
			<< "6: 1_6 anwser\n"
			<< "7: 2_1 anwser\n"
			<< "8: 2_2 anwser\n"
			<< "9: 2_3 anwser\n"
			<< "A: 3_1 anwser\n"
			<< "q: quit\n"
			<< "Enter the question number to select output result: ";
		std::cin >> cmd_input;

		switch (cmd_input)
		{
		case '1':
			HW_1_1();
			break;

		case '2':
			HW_1_2();
			break;

		case '3':
			HW_1_3();
			break;

		case '4':
			HW_1_4();
			break;

		case '5':
			HW_1_5();
			break;

		case '6':
			HW_1_6();
			break;

		case '7':
			HW_2_1();
			break;

		case '8':
			HW_2_2();
			break;

		case '9':
			HW_2_3();
			break;

		case 'a':
		case 'A':
			HW_3_1();
			break;
		}

		std::cout << std::endl << std::endl;
	}
}
