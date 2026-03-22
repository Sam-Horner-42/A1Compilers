#
# CMakeList.txt : CMake project for Compilers, include source and define project specific logic here.
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "                                         "
# ECHO "[COMPILER SCRIPT .......................]"
# ECHO "                                         "

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Compilers")

# Add source to this project's executable.
add_executable (Compilers 
	"includes/Compilers.h"		"src/Compilers.c"
	"includes/Step1Coder.h"		"src/Step1Coder.c"	
	"includes/Step2Reader.h" "src/Step2Reader.c"
	"src/Main2Reader.c"	"src/Main1Coder.c"
	"src/Main3Scanner.c" "includes/Step3Scanner.h"
	"src/Step3Scanner.c"
 )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Compilers PROPERTY CXX_STANDARD 20)
endif()
