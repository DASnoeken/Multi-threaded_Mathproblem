# What to do here
Create a directory called thirdparty/pybind11 and put pybind11 in this directory.
Make sure you also run pip install pybind11.
Then run:
```
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
```
This should create a new directory called release where the python extension should 
be located.
Then in python run:
```
import mult
a=mult.Mult()
```

Now you can call the functions single and interval using
```
a.single(str(12345))
a.interval(min,max,threshold,numberOfThreads)
```
Please note that the a.single() function takes a **string** as input and not
a number!