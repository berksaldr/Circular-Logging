#pragma once
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <iostream>
#include <mutex>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
namespace pt = boost::property_tree;
using namespace std;

class Logger
{	
public:
	Logger(void);
	void setGoal(string function);
	void setNOF(int n);
	void setFF(int f);
	void setFilePath(const char* fp);
	void setFile(FILE* f);
	string getGoal();
	int getNOF();
	int getFF();
	const char* getFilePath();
	FILE* getFile();
	void static enableFileOutput(string new_filepath);
	void static getLog(const char* jsonName);
	~Logger();
private:
	void readJson(const char* jsonName);
	static Logger& getInstance();
	void log(const char* jsonName);
	void enableFO();
	void freeFile();
    string convertUTC();
	string convertUTCForFile();
	const char* filepath;
	FILE* file;
	string goal;
	int nof;
	int ff;
};