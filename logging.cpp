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
#include "logging.h"
namespace pt = boost::property_tree;
using namespace std;

Logger::Logger(void)
{
    setFilePath("");
	setFile(0);
	setGoal("");
	setNOF(0);
	setFF(0);
}

Logger::~Logger()
{
	freeFile();	
}

void Logger::setFilePath(const char* fp)
{
    filepath = fp;
}

void Logger::setFile(FILE* f)
{
    file = f;
}

void Logger::setGoal(string function)
	{
		goal = function;
	}

void Logger::setNOF(int n)
	{
		nof = n;
	}

void Logger::setFF(int f)
	{
		ff = f;
	}

string Logger::getGoal()
	{
		return goal;
	}

int Logger::getNOF()
	{
		return nof;
	}

int Logger::getFF()
	{
		return ff;
	}

const char* Logger::getFilePath()
{
    return filepath;
}

FILE* Logger::getFile()
{
    return file;
}

void Logger::enableFileOutput(string new_filepath)
	{
		const char* new_filepath2 = new_filepath.c_str();
		Logger& logger_instance = getInstance();
		logger_instance.setFilePath(new_filepath2);
		logger_instance.enableFO();
	}

void Logger::getLog(const char* jsonName) 
	{
		getInstance().log(jsonName);
	}

void Logger::readJson(const char* jsonName)
    {
		pt::ptree root;
		pt::read_json("input.json",root);

    	setFF(root.get<int>("fileFrequency"));
		setNOF(root.get<int>("numberOfFile"));
		setGoal(root.get<string>("logType"));
	}

Logger& Logger::getInstance()
	{
		static Logger logger;
		return logger;
	}

void Logger::log(const char* jsonName)
	{
		readJson(jsonName);
		vector<string> name;
		vector<string>::iterator ptr;
        while(true){
			if(name.size() == 0)
			{
				name.push_back(convertUTC());
				enableFileOutput(convertUTC());
			}
			else if(name.size() == 1)
			{
				if(name[0] == convertUTC())
					enableFileOutput(convertUTC());
				else
				{
					name.push_back(convertUTC());
					enableFileOutput(convertUTC());
				}
			}
			else if(name.size() <= getNOF())
			{
				int count = 0;
				for(ptr = name.begin();ptr != name.end();ptr++)
				{
					if(*ptr == convertUTC()){
						enableFileOutput(convertUTC());
						count++;
					}
				}
				if(count > 0)
					enableFileOutput(convertUTC());
				else
				{
					name.push_back(convertUTC());
					enableFileOutput(convertUTC());
				}
			}
			if(name.size() == getNOF())
			{
				remove((*name.begin()).c_str());
				name.erase(name.begin());
			}
            printf("%s\t", convertUTCForFile().c_str());
            printf("[Info]\t");
            printf(getGoal().c_str());
            printf("\n");

            if (getFile())
            {
                fprintf(getFile(), "%s\t", convertUTCForFile().c_str());
                fprintf(getFile(), "[Info]\t");
                fprintf(getFile(), getGoal().c_str());
                fprintf(getFile(), "\n");
            }
			freeFile();
			//Use multiplier 86400 for daily logging.
			std::this_thread::sleep_for(std::chrono::seconds(86400*getFF()));
			//Use multiplier 1 for testing the logging.
            //std::this_thread::sleep_for(std::chrono::seconds(1*getFF()));
            
        }
		
	}

void Logger::enableFO()
	{
		if (getFile() != 0)
		{
			fclose(getFile());
		}

		setFile(fopen(getFilePath(), "a"));

		if (getFile() == 0)
		{
			printf("Logger: Failed to open file at %s", getFilePath());
		}
	}

void Logger::freeFile()
	{
		fclose(getFile());
		setFile(0);
	}

string Logger::convertUTC()
    {
                // current date/time based on current system
        time_t now = time(0);
        tm *ltm = localtime(&now);
		string hold;
		if(ltm->tm_hour <=2 && ltm->tm_mday==1)
		{
			if(ltm->tm_mon+1 == 1 || ltm->tm_mon+1 == 2 || ltm->tm_mon+1 == 4 || ltm->tm_mon+1 == 6 || ltm->tm_mon+1 == 8 || ltm->tm_mon+1 == 9 || ltm->tm_mon+1 == 11)
			{
				hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+30);

				//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					//+ "-" + to_string(ltm->tm_mday+30)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
			}
			else if(ltm->tm_mon+1 == 5 || ltm->tm_mon+1 == 7 || ltm->tm_mon+1 == 10 || ltm->tm_mon+1 == 12)
			{
				hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+29);
				
				//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					//+ "-" + to_string(ltm->tm_mday+29)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
			}
			else
			{
				if((1900+ltm->tm_year)%4 == 0)
				{
					hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+28);
					//For test with hour and minute.
					//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					//+ "-" + to_string(ltm->tm_mday+28)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
				}
				else
				{
					hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+27);
					//For test with hour and minute.
					//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					//+ "-" + to_string(ltm->tm_mday+27)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
				}
			}
		}
		else if(ltm->tm_hour <=2){
			hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					+ "-" + to_string(ltm->tm_mday-1);
			//For test with hour and minute.
			//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					//+ "-" + to_string(ltm->tm_mday-1)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
		}
		else{
			hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					+ "-" + to_string(ltm->tm_mday);
			//For test with hour and minute.
			//hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					//+ "-" + to_string(ltm->tm_mday)+"-"+to_string(-3+ltm->tm_hour)+":"+to_string(ltm->tm_min);
		}
        
		return hold;
    }

string Logger::convertUTCForFile()
{
                // current date/time based on current system
        time_t now = time(0);
        tm *ltm = localtime(&now);
		string hold;
		if(ltm->tm_hour <=2 && ltm->tm_mday==1)
		{
			if(ltm->tm_mon+1 == 1 || ltm->tm_mon+1 == 2 || ltm->tm_mon+1 == 4 || ltm->tm_mon+1 == 6 || ltm->tm_mon+1 == 8 || ltm->tm_mon+1 == 9 || ltm->tm_mon+1 == 11)
			{

				hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+30)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min);
			}
			else if(ltm->tm_mon+1 == 5 || ltm->tm_mon+1 == 7 || ltm->tm_mon+1 == 10 || ltm->tm_mon+1 == 12)
			{
				
				hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+29)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec);
			}
			else
			{
				if((1900+ltm->tm_year)%4 == 0)
				{
					hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+28)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec);
				}
				else
				{
					hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon) 
					+ "-" + to_string(ltm->tm_mday+27)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec);
				}
			}
		}
		else if(ltm->tm_hour <=2){
			hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					+ "-" + to_string(ltm->tm_mday-1)+"-"+to_string(21+ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec);
		}
		else{
			hold = "" + to_string(1900+ltm->tm_year) + "-" + to_string(ltm->tm_mon+1) 
					+ "-" + to_string(ltm->tm_mday)+"-"+to_string(-3+ltm->tm_hour)+":"+to_string(ltm->tm_min)+":"+to_string(ltm->tm_sec);
		}
        
		return hold;
    }
