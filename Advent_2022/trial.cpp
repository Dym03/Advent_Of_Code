#include <iostream>
#include <vector>
using namespace std;

int main(){

    vector<int> test(130000);
    // test.reserve(130000);

    test.push_back(10);
    cout << test.capacity()<< endl;
    cout << test[2];
    return 0;
}