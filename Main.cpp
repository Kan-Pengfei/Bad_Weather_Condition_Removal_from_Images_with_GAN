#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <fstream>
#include <string>
#include <vector>

#include "Parameters.h"
#include "My_Functions.h"
#include "Weather_Effect_Addition.h"

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
	// embed the lua files 
	lb::lua_State* L = lb::luaL_newstate(); 
	luaL_openlibs(L); 

	// check the existence of the model, if not train the model first 
	std::string exp_name; 
	std::cout << std::endl << "Please input the name of the experiment: "; 
	std::getline(std::cin, exp_name); 
	// check the file whether it exists 
	std::string model_file_name = trained_model_path + exp_name;
	const char* model_file_name_ptr = model_file_name.c_str(); 
	bool model_exist_state = fileExist(model_file_name_ptr); 

	std::string data_root =  "DATA_ROOT=" + dataset_path; 
	std::string model_name = " name=" + exp_name; // be careful with the space in the front 

	std::cout << "Do you want to train the new model or test the output? [1--train, 3--test]"; 
	int opt0; 
	std::cin >> opt0; 
	if (opt0 == 1) // train the model 
	{
		std::cout << "Do you want to generate new training dataset with your clear images? [Y / N]"; 
		std::string opt1; 
		std::cin >> opt1; 
		if (opt1.compare("Y") == 0 || opt1.compare("y") ==0)
		{
			Weather_Effect_Addition::add4GAN(clear_img_path, training_dataset_path, img_types); // add new training dataset 
			std::cout << "Your images are added into the training dataset." << std::endl; 
		}
		else 
			std::cout << "Using the default training dataset. " << std::endl; 

		// training the model 
		if (model_exist_state)
		{
			std::cout << std::endl << "Do you want to continue training the current model? [y / n] ";
			std::string continue_state; 
			std::getline(std::cin, continue_state);
			if (continue_state == "y" || continue_state == "Y")
			{
				std::cout << std::endl << "It is gonna to continue training the model ... " << std::endl; 
				std::string train_command_str = data_root + model_name + " continue_train=1 which_direction=BtoA th train.lua";
				const char* train_command = train_command_str.c_str(); 
				system(train_command);
			}
			else if (continue_state == "n" || continue_state == "N")
			{
				std::cout << std::endl << "The trained model is already trained! " << std::endl ; 
			}
			else
				std::cout << "Invalid input! " << std::endl; 
		}
		else
		{
			std::cout << std::endl << "It is gonna to train the model ... " << std::endl; 
			std::string train_command_str = data_root + model_name + " continue_train=0 which_direction=BtoA th train.lua";
			const char* train_command = train_command_str.c_str(); 
			system(train_command); 
		}

	}
	else if (opt0 == 3) // test the input images and remove the bad weather effect in them
	{
		// remove the previous test data
		std::string remove_file_command_str = "exec rm -r " + testing_dataset_path + "*"; 
		const char* remove_file_command = remove_file_command_str.c_str(); 
		system(remove_file_command);  

		// pre-process the test images 
		std::vector<cv::String> input_img_paths, input_img_names;
		for (int i = 0; i < img_types.size(); ++i)
		{
			std::vector<cv::String> img_paths; 
			cv::glob(img_to_process_path + "*." + img_types[i], img_paths, false);
			input_img_paths.insert(input_img_paths.end(), img_paths.begin(), img_paths.end()); 
		}
		if (input_img_paths.size() == 0)
		{
			std::cout << "Please put the images that need to be processed in file \" Your-Images \". " << std::endl; 
			exit(1); 
		}
		else 
		{
			input_img_names = MyFuncs::MyStrFuncs::concat(input_img_paths);
			for (int i = 0; i < input_img_paths.size(); ++i)
			{
				cv::Mat input_img = cv::imread(input_img_paths[i]);
				cv::Mat GAN_img = MyFuncs::MyImgFuncs::combineParal(input_img, input_img); 
				cv::String GAN_test_img_path = testing_dataset_path + input_img_names[i]; 
				cv::imwrite(GAN_test_img_path, GAN_img); 
				cv::waitKey(10);
			}

			// process the images with weather effect 
			if (model_exist_state)
			{
				std::cout << "Processing your images with model " << model_name << std::endl; 
				std::string test_command_str = data_root + model_name + " which_direction=BtoA phase=testing th test.lua";
				const char* test_command = test_command_str.c_str(); 
				system(test_command); 
			}
			else 
			{
				std::cout << "Error: Your model does not exist! " << std::endl; 
				exit(1); 
			}

		}
	}
	else 
		std::cout << "Invalid input! " << std::endl; 

	lua_close(L); 
	return 0; 
}