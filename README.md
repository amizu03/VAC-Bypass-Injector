# VAC-Bypass-Injector
Injector which bypasses new VAC anti-injection.
This injector DOES NOT bypass the VAC anticheat itself YET, so you will have to use an undetected cheat along with this injector.
This injector only bypasses the techniques Valve uses to detect thread creation and LoadLibrary injection.

Credits to danielkrupinski for pseudocode of VAC modules.

USAGE: Place DLL to inject in same directory as injector. Run injector, and it will automatically inject the first DLL it finds in the same folder.

# Requirements
1. MSVC VS 2019 C++ Build Tools or newer.
2. Project settings that support the c++17 language standard or newer.

# Usage
1. Clone the repository or download the source files to your computer.
2. Run the .bat file to build the project. A separate .exe file will be generated in a new "bin" folder.
3. Copy your DLL into the directory or copy the injector into a directory with your DLL.
4. Open CS:GO and run the injector **(AS ADMINISTRATOR)**. It will inject the first DLL it finds in the working directory.
