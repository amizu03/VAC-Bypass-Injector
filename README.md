# VAC-Bypass-Injector
Injector which bypasses new VAC anti-injection.
This injector DOES NOT bypass the VAC anticheat itself YET, so you will have to use an undetected cheat along with this injector.
This injector only bypasses the techniques Valve uses to detect thread creation and LoadLibrary injection.

Credits to danielkrupinski for pseudocode of VAC modules.

USAGE: Place DLL to inject in same directory as injector. Run injector, and it will automatically inject the first DLL it finds in the same folder.

# Requirements
1. MSVC VS 2019 C++ Build Tools or newer.
2. Project settings that support the c++17 language standard or newer.
3. Your include directories set correctly, the "include" folder should be the additional directory.

# FAST SETUP (NO VS SOLUTION)
1. Clone the repository or download the source files to your computer.
2. Extract the folder inside the zip file if you downloaded the source directly.
3. Create a "bin" folder in the "VAC-Bypass-Injector-master" directory.
4. Run the .bat file to build the project. A separate .exe file will be generated in a new "bin" folder.
5. Copy your DLL into the directory or copy the injector into a directory with your DLL.
6. Open CS:GO and run the injector **(AS ADMINISTRATOR)**. It will inject the first DLL it finds in the working directory.

# VS SOLUTION
1-3. See FAST SETUP.
4a. Open the VS Solution provided, and make sure you compile in Release OR Debug x86 ONLY.
4b. Go to your project's Configuration Properties->General->C++ Language Standard and make sure that it is set to C++17 or newer. 
4c. Go to project's Configuration Properties->VC++ Directories->Include Directories and make sure they are set correctly (include folder should be there by default).
5-6. See FAST SETUP.
