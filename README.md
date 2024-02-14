# Nettunneller-Server

NetTunneller Server is a lightweight solution for managing and maintaining SSH reverse tunnels. This server component seamlessly integrates with the NetTunneller client to establish secure connections between drones and a central command and control server.

It was designed to solve the issue of connecting to devices over 4G

## Features
 - ☑ **Dynamic Drone Monitoring**: Keep track of the status of multiple drones in real-time.
 - ☑ **Automated Setup**: Easily deploy and configure the NetTunneller Server with minimal effort.
 - ☑ **Secure Communication**: Utilizes SSH for secure communication between the central server and connected drones.
 - ☐ **Service Persistence**: NetTunneller Server can run persistently as a service, ensuring continuous monitoring.
 - ☑ **User Authentication**: SSH key-based authentication ensures secure and passwordless access to the server.
 - ☐ **Dynamic Tunneling**: Dynamically manage reverse SSH tunnels to allow secure access to drone resources.

## Getting Started
### Prerequisites
 - [OpenSSH Server](https://www.openssh.com/) installed on the central server.
 - [NetTunneller Client](https://github.com/gboyle05/nettunneller-client) installed on drones.

### Installation
1. Clone the repo
   
   ```
   git clone https://github.com/gboyle05/nettunneller-server.git
   ```
2. Change into the project directory:
   
   ```
   cd nettunneller-server
   ```
3. Build the project:
   
   ```
   cpp ./main.cpp - o nettunneller-server
   ```

### Usage
1. Run the project
   
   ```
   ./nettunneller-server
   ```
