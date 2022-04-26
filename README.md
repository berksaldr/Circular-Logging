# Circular-Logging
That repository is for daily logging with given log type, number of file and file frequency.

### Definition
In repository, code reads the variables from the json config file. the  code uses the log type for writing to and printing to terminal the on time. Code controls the number of file in project with given number of file variable. Code creates,writes and prints the log type to file with given frequency. 

### Compilation
In the code there are some little warnings. For that reason, we should use the compilation method given below.

`g++ -c logging.cpp`

`g++ logging.cpp main.cpp -o main`

`./main`

### Some Explanations About The Code
In the main.cpp, we can call just the `getLog()` function for logging with config `json` file. We don't need to call a object, because I have defined parts of the code as static.

```cpp
Logger::getLog("input.json");
```

In the constructor, code initializes the class members to zero.

```cpp
Logger::Logger(void)
{
    setFilePath("");
	setFile(0);
	setGoal("");
	setNOF(0);
	setFF(0);
}
```

There are setters and getters for private class members.

`enableFileOutput()` function receives the file path and enables the file.

```cpp
void Logger::enableFileOutput(string new_filepath)
	{
		const char* new_filepath2 = new_filepath.c_str();
		Logger& logger_instance = getInstance();
		logger_instance.setFilePath(new_filepath2);
		logger_instance.enableFO();
	}
```
`getLog()` function receives config json name and calls the log function of general object.

```cpp
void Logger::getLog(const char* jsonName) 
	{
		getInstance().log(jsonName);
	}
```
  
`log()` function firstly reads config json file. After that, it makes decisions about log files in project. If number of files and number of log files in project equal, function removes the earliest of them. Then it writes and prints date-time with UTC format and log type. Finally, it closes the file and waits according to the given frequency.

In the `convert` functions, they are converting the date and time to UTC format. Normal `converUTC()` 
makes decisions about some different situations and prints the UTC date. `convertUTCForFile()` function, it converts the date and time for file with hours, minutes and seconds.


### Test The Project

That project was made for daily log files. So, the values are for its. If you want to test the project, you can change the values below.

##### For Daily
```cpp
std::this_thread::sleep_for(std::chrono::seconds(86400*getFF()));
```

##### For Minute
```cpp
std::this_thread::sleep_for(std::chrono::seconds(60*getFF()));
```

##### For Second
```cpp
std::this_thread::sleep_for(std::chrono::seconds(1*getFF()));
```

And for minute testing you should change the `convertUTC()` function return value. Necessary changes in the function are indicated. 
```cpp
if(ltm->tm_mon+1 == 1 || ltm->tm_mon+1 == 2 || ltm->tm_mon+1 == 4 || ltm->tm_mon+1 == 6 || ltm->tm_mon+1 == 8 || ltm->tm_mon+1 == 9 || ltm->tm_mon+1 == 11)
			{
				hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+30);

				//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					//+ "-" + to_string(ltm->tm_mday+30)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
			}
```
