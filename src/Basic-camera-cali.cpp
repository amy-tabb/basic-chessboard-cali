//============================================================================
// Name        : Basic-camera-cali.cpp
// Author      : Amy Tabb
// Version     :
// Copyright   : MIT
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Includes.hpp"
#include "Calibration2.hpp"
#include <iostream>
using namespace std;


void EnsureDirHasTrailingBackslash(string& write_directory){
	int n_letters = write_directory.size();
	bool eval =  (write_directory[n_letters - 1] == '/');
	cout << "Last character compare " << write_directory << " " <<  eval << endl;
	if (eval == false){
		write_directory = write_directory + "/";
	}

}

//int main() {
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	return 0;
//}



int main(int argc, char** argv) {

	Eigen::initParallel();

	/////////////////////////////************************************/////////////////////////////
	string source_dir;
	string image_dir;
	string write_dir;
	string robot_file = "";
	string robot_dir;
	string hand_eye_dir;
	string cali_object_file;
	ifstream in;
	ofstream out;


	double  chess_mm_height = 0;
	double chess_mm_width = 0;
	int chess_height = 0;
	int chess_width = 0;
	int flag;
	bool do_camcali = true;
	bool do_rwhec = true;
	bool do_reconstruction = true;

	cout << "Usage is: function_name input_directory write_directory."  << endl;

	if (argc == 3){
		source_dir =  string(argv[1]);
		write_dir = argv[2];

		EnsureDirHasTrailingBackslash(source_dir);
		EnsureDirHasTrailingBackslash(write_dir);

		cali_object_file =  string(source_dir) + "calibration_object.txt";

	}	else {
		cout << "Wrong number of arguments " << endl;
		exit(1);
	}


	in.open(cali_object_file.c_str());

	if (!in){
		cout << "You forgot the calibration_object.txt file or the path is wrong" << endl << argv[1] << endl;;
		cout << cali_object_file << endl;
		exit(1);
	}

	// TODO fix this ... make it better.
	string temp;
	in >> temp >> chess_mm_height;
	in >> temp >> chess_mm_width;
	in >> temp >> chess_height;
	in >> temp >> chess_width;
	in.close();

	string filename = write_dir + "details.txt";
	out.open(filename.c_str());

		CaliObjectOpenCV2 CaliObj (0, chess_width, chess_height,
					chess_mm_width, chess_mm_height);

		image_dir = source_dir + "images";
		DIR* dir = opendir(image_dir.c_str());
		if (dir)
		{
			/* Directory exists. */
			closedir(dir);
			CaliObj.ReadImages(image_dir, 1);
		}


		// argument is whether or not to draw the corners
		CaliObj.AccumulateCornersFlexibleExternal(true);

		CaliObj.CalibrateFlexibleExternal(out,  write_dir);

	out.close();

	return 0;
}
