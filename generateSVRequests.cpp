#include <iostream>
#include<cstdlib>
#include<fstream>

using namespace std;

int main(int argc, char ** argv){
	
	string filepath;
	string capturename;
	int offset = 40;
	
	if(argc < 2){
		cerr<<"Bad input. Please input a fully qualified filename of a DriveData folder"<<endl;
		exit(1);
	}

	else{
		filepath = argv[1];;
	}

	ifstream in_key;
	char key[100];
	in_key.open("key.txt");
	in_key.getline(key, 100, '\n');
	in_key.close();

	size_t found = filepath.find_last_of("/");
        capturename = filepath.substr(found, filepath.length());

	cout<<filepath<<endl;
	cout<<capturename<<endl;

	ifstream in;

	string infilename;
	
	infilename = filepath + "/analysis" + capturename + "_snappedPose.dat";
	cout<<"IN:"<<infilename<<endl;

	in.open(infilename.c_str());

        if(!in){
                cerr<<"FILE COULD NOT BE OPENED"<<endl;
                exit(1);
        }

        char buffer[100];
        double lat[100], lng[100];
	int orient[100];
        int i=0;

        do{
                in >> lat[i];
                in.getline(buffer, 100 ,',');
                in >> lng[i];
		in.getline(buffer, 100, ',');
		in >> orient[i];
                in.getline(buffer, 100, '\n');
                i++;

        }while(!(in.eof()));

	in.close();
        int k = i;


	ofstream out;
	string outfilename;
	outfilename = filepath + "/analysis"+ capturename + "_svRequests.dat";
	cout<<"OUT:"<<outfilename<<endl;
	out.open(outfilename.c_str(), std::fstream::out);

	if(!out){
		cerr<<"FILE COULD NOT BE OPENE"<<endl;
	}

        i=0;

        while(i<k-1){
		out<<"https://maps.googleapis.com/maps/api/streetview?";
		out<<"size=640x640&";
		out<<"location="<<lat[i]<<","<<lng[i]<<"&";
		out<<"heading="<<orient[i]<<"&";
		out<<"key="<<key;
		out<<"\n";
		i++;
        }
	out.close();

        outfilename = filepath + "/analysis"+ capturename + "_svRequestsLeft.dat";
        cout<<"OUT_LEFT:"<<outfilename<<endl;
        out.open(outfilename.c_str(), std::fstream::out);

        if(!out){
                cerr<<"FILE COULD NOT BE OPENED"<<endl;
        }

        i=0;
	int temp;

        while(i<k-1){
                out<<"https://maps.googleapis.com/maps/api/streetview?";
                out<<"size=640x640&";
                out<<"location="<<lat[i]<<","<<lng[i]<<"&";
		temp = orient[i]-offset;
		if(temp < 0)
			temp += 360;
                out<<"heading="<<temp<<"&";
                out<<"key="<<key;
                out<<"\n";
                i++;
        }
        out.close();

        outfilename = filepath + "/analysis"+ capturename + "_svRequestsRight.dat";
        cout<<"OUT_RIGHT:"<<outfilename<<endl;
        out.open(outfilename.c_str(), std::fstream::out);

        if(!out){
                cerr<<"FILE COULD NOT BE OPENED"<<endl;
        }

        i=0;

        while(i<k-1){
                out<<"https://maps.googleapis.com/maps/api/streetview?";
                out<<"size=640x640&";
                out<<"location="<<lat[i]<<","<<lng[i]<<"&";
		temp = orient[i]+offset;
		if(temp > 360)
			temp -=360;
                out<<"heading="<<temp<<"&";
                out<<"key="<<key;
                out<<"\n";
                i++;
        }
        out.close();



	return 0;
}