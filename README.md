### How to build FTXUI on Mac
```
git clone https://github.com/ArthurSonzogni/FTXUI.git
cd FTXUI
mkdir build && cd build
cmake ..
make
```

1. **Locate the Headers**:
   
   After you've built FTXUI from source, the header files should be in the `include` directory of the cloned FTXUI repository. 
   
   The path would look something like this: `/path_to_your_cloned_FTXUI_directory/include`.

2. **Locate the Libraries**:

   The libraries generated after building FTXUI should be in the `build` directory (or another specified directory if you chose a custom build location). 

   The path for libraries might look like this: `/path_to_your_cloned_FTXUI_directory/build`.

3. **Add Paths to Xcode**:

   - **Header Search Paths**:
     - In Xcode, select your project in the Project Navigator.
     - Go to the target's "Build Settings".
     - Search for "Header Search Paths".
     - Add the path from step 1.
   
   - **Library Search Paths**:
     - Still in the "Build Settings" tab, search for "Library Search Paths".
     - Add the path from step 2.

5. **Link the Libraries**:

   After setting the paths, ensure you link the required libraries in the "Link Binary with Libraries" phase.
