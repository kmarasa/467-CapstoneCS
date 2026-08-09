Note from Margaret Barnes:

To help with cross-compilcation, I used GitHub Copilot CLI with the ‘Auto’ model enabled (I’m not sure which specific model was in use).
First I prompted Copilot to find source of compilation issue I was having with code pulled from our project repo. Copilot told me that c_cpp_properties.json was only configured to work on Windows.

I then prompted “update c_cpp_properties.json to work with a mac, windows, and linux setup”

Copilot edited c_cpp_properties.json to the current version of the file, allowing this project to be run from a mac, windows, or linux operating system.

Reflection: This was incredibly efficient. I could have examined [this VS Code reference](https://code.visualstudio.com/docs/cpp/customize-cpp-settings), but as our team works with different operating systems and cross compatibility wasn’t the point of this project, I decided I didn’t want to spend a lot of time here - I just wanted it to work. And it does. I have not at this point changed this file from how Copilot CLI updated it; I need to discuss proper citation and whether any of it needs to be changed for the final product with my team and perhaps our TA.
