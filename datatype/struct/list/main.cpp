#include<iostream>
#include<list>
using namespace std;
int main()
{
	cout<<"main"<<endl;
	
	list<int> listOne;
    //声明i为迭代器  
    list<int>::iterator i;  
   
    listOne.push_front(3);  
    listOne.push_front(2);  
    listOne.push_front(1);  
   
    listOne.push_back(4);  
    listOne.push_back(5);  
    listOne.push_back(6);  
   
    cout << "listOne.begin()--- listOne.end():" << endl;  
    for (i = listOne.begin(); i != listOne.end(); ++i)  
        cout << *i << " ";  
    cout << endl;  
   

}
