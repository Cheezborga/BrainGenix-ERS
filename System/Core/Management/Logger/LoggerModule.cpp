//======================================================================//
// This file is part of the BrainGenix-ERS Environment Rendering System //
//======================================================================//

/*
    Description: This file is responsible for implementing the logger system used by the rest of the system.
    Additonal Notes: None
    Date Created: 2021-05-31
*/

#include <iostream>
#include <string>
#include <ctime>
#include <cstring>


class Logger {

    // Define "Local" Variables And Functions //
    private: 

        // Define Local Vars //
        bool PrintLogOutput;


        // Returns A String Of The Current UTC Time //
        std::string Get_UTC_Time() { 
            
            // Get UTC String //
            time_t CurrentTime = std::time(0);
            char* UTCString = ctime(&CurrentTime);

            // Remove New Line From String //
            if (UTCString[strlen(UTCString) - 1] == '\n') {
                UTCString[strlen(UTCString) - 1] = '\0';
            }

            return UTCString;
        }

        // Create Function That Gets Callstack? //
        // Returns callstack as std::string?


    // Define Functions //
    public: 

        // Define Initialization Function //
        void InitializeLogger(bool PrintLogOutputArgument=true) { // ** NOTE: THIS SHOULD TAKE A CONFIG DICT FROM YAML IN THE FUTRE ** //

            // Update Local Config Parameters //
            PrintLogOutput = PrintLogOutputArgument;

            // Print Log Key //
            if (PrintLogOutput) {
                std::cout << "[Level] [Time] [Message]\n";
            };

        };

        // Log An Item //
        void Log(const char* LogItem, int LogLevel=0) {

            // Get Input Params, and Reformat Into Strings //
            std::string CurrentTime = Get_UTC_Time();

            // Create Output String //
            std::string Output;

            // Is there a more elegant way to do this? I saw that std::format would be good, 
            //but for some reason my compiler doens't support it.
            // Also, is there a way to trace back to get the calling 
            //file's name and the calling function's name?

            Output += "[" + std::to_string(LogLevel) + "] ";
            Output += "[" + CurrentTime + "] ";
            Output += std::string(LogItem) + "\n";


            // If Log Print Enabled //
            if (PrintLogOutput) {
                std::cout<< Output;
            };

        }

};

