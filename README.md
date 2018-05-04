# Bad_Weather_Condition_Removal_from_Images_with_GAN
Bad_Weather_Condition_Removal_from_Images_with_GAN

The project is developed under Linux 

Run the code with terminal command :
g++ *.cpp -o GAN -std=c++11 -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -llua5.1 

1. Prepare
install OpenCV
install Lua
install torch
install nngraph
install hdf5

make the alias of the "opencvLua" to compile_opencv_lua.sh in your path or use the terminal command I offered above 

2. Train your own model
You can generate your own training dataset for the GAN model. 
Put your clear images inside the "Clear-Images" direcotry,
this code will generate all the corresponding training data for you.
Then follow the guide of the code 

3. Test the model 
Put the images that you want to process in the file "Your-Images". 
If your image files have different format, you should add all the formats to "img_types" in "Parameter.h". 
You need to modify the "which_epoch" value in test.lua file, you should choose the value based on your model file "*_net_G.t7". 

4. Reference
Code borrowed heavily from https://github.com/hezhangsprinter/ID-CGAN, https://github.com/jcjohnson/fast-neural-style and https://github.com/phillipi/pix2pix. Thanks for the sharing. 
