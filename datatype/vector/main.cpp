#include <iostream>
#include <vector>
using namespace std;
using std::vector;

int main(int argc,char *argv[])
{
	cout<<"main"<<endl;
	vector<int> mvector;
	mvector.push_back(1);

	int count = mvector.size();
	cout<<"size: "<<count<<endl;
	return 0;
}
