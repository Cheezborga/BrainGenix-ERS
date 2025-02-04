########################################################################
# This file is part of the BrainGenix-ERS Environment Rendering System #
########################################################################


# Create Library (Name Should Be Parent Dir Name)
add_library(ERS_CLASS_LoggingSystem

            # Add Source Files (.cpp)
            "ERS_CLASS_LoggingSystem.cpp"
            "ERS_CLASS_OpenGLLoggingSystem.cpp"
            "ERS_STRUCT_MessageCallbackUserParam.cpp"

            # Add Header Files (.h)
            "ERS_CLASS_LoggingSystem.h"
            "ERS_CLASS_OpenGLLoggingSystem.h"
            "ERS_STRUCT_MessageCallbackUserParam.h"

            
            ${BACKWARD_ENABLE}
            )

# Link 3rd Party Libs
target_link_libraries(ERS_CLASS_LoggingSystem
    yaml-cpp
    glad
    )

# Link Internal Libs
target_link_libraries(ERS_CLASS_LoggingSystem
    ERS_STRUCT_RGBColor
    ERS_STRUCT_OpenGLLogItem
    )

if (WIN32) 
    ERSBuildLogger(${BoldGreen} "Build Warnings Not Enabled")
else ()
    target_compile_options(ERS_CLASS_LoggingSystem PUBLIC -Wall -Wextra -Wpedantic)
    target_compile_options(ERS_CLASS_LoggingSystem PUBLIC -Werror=uninitialized)
endif()

target_include_directories(ERS_CLASS_LoggingSystem PUBLIC ./)

