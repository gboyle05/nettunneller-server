#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <utility>
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include <cstdlib>

// Initial Setup

bool userExists(const std::string& username) {
    std::stringstream checkUserCmd;
    checkUserCmd << "id -u " << username << " >/dev/null 2>&1";
    return system(checkUserCmd.str().c_str()) == 0;
}

void createUser(const std::string& username, const std::string& shell) {
    if (userExists(username)) {
        std::cout << "User '" << username << "' already exists. Skipping user creation." << std::endl;
    } else {
        // Create the user
        std::string createUserCmd = "sudo useradd -m -s " + shell + " " + username;
        int createUserResult = system(createUserCmd.c_str());

        if (createUserResult != 0) {
            std::cerr << "User creation failed." << std::endl;
            exit(1);
        }
        std::cout << "User '" << username << "' created successfully." << std::endl;

        std::string createSshDirCmd = "sudo mkdir -p ~" + username + "/.ssh";
        int createSshDirResult = system(createSshDirCmd.c_str());

        if (createSshDirResult != 0) {
            std::cerr << "Create SSH Dir failed." << std::endl;
            exit(1);
        }
        std::cout << "SSH directory created successfully." << std::endl;
    }
}

void createSshKeypair() {
    std::cout << "Creating ssh keypair..." << std::endl;

    std::string createSshKeypairCmd = "ssh-keygen -qN '' -f ~/.ssh/id_rsa";
    int createSshKeypairResult = system(createSshKeypairCmd.c_str());

    if (createSshKeypairResult != 0) {
        std::cerr << "SSH keypair creation failed." << std::endl;
        exit(1);
    }
    std::cout << "SSH keypair created successfully." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void changeSshDirectoryOwnership(const std::string& username) {
    // Change ownership of the .ssh directory
    std::string chownCmd = "sudo chown -R " + username + ":" + username + " /home/" + username + "/.ssh";
    int chownResult = system(chownCmd.c_str());

    if (chownResult != 0) {
        std::cerr << ".ssh directory ownership change failed." << std::endl;
        exit(1);
    }

    std::cout << ".ssh directory ownership changed successfully." << std::endl;
}



// Represents a drone with a name, port, and status.
struct Drone {
    std::string name;
    int port;
    std::string status;

    Drone(std::string  name, int port) : name(std::move(name)), port(port), status("Unknown") {}
};

// Read in from csv
std::vector<Drone> parseCSV(const std::string& filename) {
    std::vector<Drone> drones;
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::string name;
            int port;

            std::istringstream iss(line);
            if (std::getline(iss, name, ',') && (iss >> port)) {
                drones.emplace_back(name, port);
            }
        }
        file.close();
    } else {
        std::cerr << "Failed to open " << filename << std::endl;
    }

    return drones;
}


// Function to check the connection status of a drone.
void checkConnection(Drone& drone) {
    std::string command = "nc -z -w2 localhost " + std::to_string(drone.port) + " > /dev/null 2>&1" ;
    int result = std::system(command.c_str());

    if (result == 0) {
        drone.status = "Connected";
    } else {
        drone.status = "Not Connected";
    }
}

// Function to periodically update drone statuses.
[[noreturn]] void updateDroneStatuses(std::vector<Drone>& drones, int intervalSeconds) {
    while (true) {
        for (Drone& drone : drones) {
            (void)std::async(std::launch::async, checkConnection, std::ref(drone));
        }

        std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
    }
}

// Function to display drone statuses in the console.
void displayDroneStatuses(const std::vector<Drone>& drones) {
    system("clear"); // Clear the terminal (Linux/Unix)

    for (const Drone& drone : drones) {
        std::cout << "Drone Name: " << drone.name << std::endl;
        std::cout << "Port: " << drone.port << std::endl;

        if (drone.status == "Connected") {
            std::cout << "Status: " << "\033[32m" << drone.status << "\033[0m" << std::endl; // Green color
        } else if (drone.status == "Not Connected") {
            std::cout << "Status: " << "\033[31m" << drone.status << "\033[0m" << std::endl; // Red color
        } else {
            std::cout << "Status: " << drone.status << std::endl;
        }

        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]) {
//    Predefined Drones
//    std::vector<Drone> drones = {
//        {"Drone 001", 9000},
//        {"Drone 002", 9001},
//        {"Drone 003", 9002},
//        {"Drone 004", 9003},
//        {"Drone 005", 9004},
//        {"Drone 006", 9005},
//        {"Drone 007", 9006},
//        {"Drone 008", 9007},
//        {"Drone 009", 9008},
//        {"Drone 010", 9009},
//        {"Drone 011", 9010},
//        {"Drone 012", 9011},
//        {"Drone 013", 9012},
//        {"Drone 014", 9013},
//        {"Drone 015", 9014},
//        {"Drone 016", 9015},
//        {"Drone 017", 9016},
//        {"Drone 018", 9017},
//        {"Drone 019", 9018},
//        {"Drone 020", 9019},
//        {"Drone 021", 9020},
//        {"Drone 022", 9021},
//        {"Drone 023", 9022}
//    };
        if (argc > 1 && strcmp(argv[1], "setup") == 0) {
            const char* username = "nettunneller";
            const char* shell = "/bin/true";
            createUser(username, shell);
            createSshKeypair();
//            changeSshDirectoryOwnership(username);
// TODO            To allow for ssh-copy-id
        } else {
            std::vector<Drone> drones = parseCSV("drones.csv");

            int updateIntervalSeconds = 10;

            std::thread updater(updateDroneStatuses, std::ref(drones), updateIntervalSeconds);

            while(true) {
                displayDroneStatuses(drones);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            updater.join();
        }
    return 0;
}
