# ofxGuido

Modifications to guidolib-code/build/CMakeLists.txt

For iOS 4.3.2.

- Modify CMAKE_OSX_ARCHITECTURES to armv6 or armv7, not both:

set (CMAKE_OSX_ARCHITECTURES "armv6")

- Symbolic link iOS 5 SDK gcc and g++ to /Developer/usr

sudo ln -s /Developer/Platforms/iPhoneOS.platform/Developer/usr/llvm-gcc-4.2/bin/arm-apple-darwin10-llvm-gcc-4.2 /Developer/usr/llvm-gcc-4.2/bin/

sudo ln -s /Developer/Platforms/iPhoneOS.platform/Developer/usr/llvm-gcc-4.2/bin/arm-apple-darwin10-llvm-g++-4.2 /Developer/usr/llvm-gcc-4.2/bin/

- Add SDK includes and remove -stdlib=libc++:

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk/usr/include/c++/4.2.1 -I/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk/usr/include")

- Avoid building osx and linux devices:

elseif(APPLE AND NOT IOS)
elseif(UNIX AND NOT IOS)


- Launch cmake with -DIOS="yes" and -DINDEPENDENTSVG="yes"

 
