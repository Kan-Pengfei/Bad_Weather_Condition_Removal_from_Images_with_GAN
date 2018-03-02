#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <string>

#include "Parameters.h"
#include "Add_Rain.h"
#include "Generate_Training_Dataset.h"
#include "Preprocess_Image.h"

// need to include LuaBridge and all the library of Lua
#include <LuaBridge/LuaBridge.h>
extern "C"
{
	#include <lua5.1/lua.h>
	#include <lua5.1/lauxlib.h>
	#include <lua5.1/lualib.h>
}

namespace lb = luabridge;

// function for the check of the file existence 
// if output is 1, the file exits
// if output is 0, the its not  
bool fileExist (const char* file_name)
{
	std::ifstream ifile(file_name); 
	return (bool)ifile; 
}

int main()
{
	// check the training dataset directory 
	cv::String training_dataset_file = training_dataset_path + "*" + image_type;
	std::vector<cv::String> training_dataset_file_names;
	cv::glob(training_dataset_file, training_dataset_file_names, false);
	int training_dataset_file_num = training_dataset_file_names.size();
	if (training_dataset_file_num == 0)
	{
		addRain(original_image_path, rain_added_image_path);
		generateTrainingSet(original_image_path, rain_added_image_path, training_dataset_path); 
	}
	else
	{
		std::cout << std::endl << "The training dataset has already been generated!" << std::endl; 
	}

	// check the existence of the model, if not train the model first 
	std::string exp_name; 
	std::cout << std::endl << "Please input the name of the experiment: "; 
	std::getline(std::cin, exp_name); 

	// embed the lua files 
	lb::lua_State* L = lb::luaL_newstate(); 
	luaL_openlibs(L); 

	// check the file whether it exists 
	std::string model_file_name = trained_model_path + exp_name;
	const char* model_file_name_ptr = model_file_name.c_str(); 
	bool model_exist_state = fileExist(model_file_name_ptr); 

	std::string data_root =  "DATA_ROOT=" + dataset_path; 
	std::string model_name = " name=" + exp_name; // be careful with the space in the front 

	if (model_exist_state == 1)
	{
		std::cout << std::endl << "Do you want to continue training the current model? [y / n]" << std::endl;
		std::string continue_state; 
		std::getline(std::cin, continue_state);
		if (continue_state == "y")
		{
			std::cout << std::endl << "It is gonna to continue training the model ... " << std::endl; 
			std::string train_command_str = data_root + model_name + " continue_train=1 which_direction=BtoA th train.lua";
			const char* train_command = train_command_str.c_str(); 
			system(train_command);
		}
		else
		{
			std::cout << std::endl << "The trained model exists, you can test images." << std::endl ; 
		}
	}
	else
	{
		std::cout << std::endl << "It is gonna to train the model ... " << std::endl; 
		std::string train_command_str = data_root + model_name + " continue_train=0 which_direction=BtoA th train.lua";
		const char* train_command = train_command_str.c_str(); 
		system(train_command); 
	}

	// need to check the test dataset file has files 
	cv::String testing_data_file = testing_dataset_path + "*" + image_type; 
	cv::String image_to_process_file = image_to_process_path + "*" + image_type; 
	std::vector<cv::String> testing_data_file_names;
	std::vector<cv::String> image_to_process_file_names;
	cv::glob(testing_data_file, testing_data_file_names, false); 
	cv::glob(image_to_process_file, image_to_process_file_names, false);
	int testing_data_size = testing_data_file_names.size(); 
	int image_to_process_file_size = image_to_process_file_names.size(); 
	if (testing_data_size != 0)
	{
		std::cout << std::endl << "The testing data exists, it gonna to process the image: ..." << std::endl;
		// use the trained model to operate images 
		std::string test_command_str = data_root + model_name + " which_direction=BtoA phase=testing th test.lua";
		const char* test_command = test_command_str.c_str(); 
		system(test_command); 
	}
	else if (image_to_process_file_size != 0)
	{
		preprocessImg(image_to_process_path, testing_dataset_path); 
		// use the trained model to operate images 
		std::string test_command_str = data_root + model_name + " which_direction=BtoA phase=testing th test.lua";
		const char* test_command = test_command_str.c_str(); 
		system(test_command); 
	}
	else 
	{
		std::cout << std::endl << "There is no images in 'Your-Images' file! You need to put images in this file for processing!" << std::endl; 
	}

	lua_close(L); 
	return 0; 
}