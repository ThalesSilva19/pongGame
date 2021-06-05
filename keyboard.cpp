#include<iostream>
#include<fstream>
#include<linux/input.h>

using namespace std;


int main(){
	
	input_event data;
 	ifstream file("/dev/input/event4");

	for(int i = 0; i < 100; i++){
		
		file.read((char*)&data,sizeof(input_event));
		
		if(data.type == EV_KEY){
			cout <<  data.code << endl;
		}		
		
	}

	file.close();
	return 1;
}


