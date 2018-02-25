//#include <glm/glm.cpp>
//#include <glm/ext.cpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

int main(){
	std::string input("f 20381/19883/18373 20704/19883/18358 20378/19813/18035");
	input = input.substr(2);
	std::istringstream ss(input);
	std::string p1, p2, p3;
	ss>>p1; ss>>p2; ss>>p3;
	std::cout << p1 << std::endl;
	std::cout << p2 << std::endl;
	std::cout << p3 << std::endl;
	
	std::stringstream vs(p1);
	std::string v,t,n;
	std::getline(vs, v, '/');
	std::getline(vs, t, '/');
	std::getline(vs, n, '/');
	

	std::cout << v << std::endl;
	std::cout << t << std::endl;
	std::cout << n << std::endl;

}
