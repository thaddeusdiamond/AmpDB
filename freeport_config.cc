/* freeportconfig.cc
 *
 * The following is an adaptation from Perl to C++ of Alex Thomson's
 *  freeport scanner in order to remake the config file to avoid
 *  problems with multiple port connections (address already in use errors)
 *
 */

#include "freeport_config.h"

int make_pay_config() {
    ifstream config;
    ofstream newconfig;
    string line;
    int success_code;
    
    config.open("../test.conf");                        // I/O Files 
    newconfig.open("../pay.conf");    
    
    int port = 5000;                                    // Random start port
    long sockfd = socket(AF_INET, SOCK_STREAM, 0);      // The following code
    struct hostent *he = gethostbyname("localhost");    //  sets up a socket
    struct sockaddr_in sa;                              //  for use and tests
    memset(&sa, 0, sizeof(sa));                         //  it's connections
    sa.sin_family = AF_INET;
    memmove(&sa.sin_addr, he->h_addr_list[0], he->h_length);
  
    int ports[2];                                       // Initially null ports
    ports[0] = ports[1] = 0;
    for (int i = 0; i < 50000; i++) {
        sa.sin_port = htons(port++);                    // Make new port
        success_code = connect(sockfd, reinterpret_cast<const sockaddr*>(&sa), 
                                       sizeof(sa));
        if (success_code >= 0) {                        // Connection success
            if (ports[0] == 0) {
                ports[0] = port - 1;
            } else {
                ports[1] = port - 1;
                close(sockfd);
                break;
            }
        }
    }
    port = port - 1;
    
    if (config.is_open() and newconfig.is_open()) {
        while (config.good()) {
            getline (config, line);                     // Read in config
            if (line.find("mediator") < line.length())  // Line to replace
                newconfig << "node100=mediator:1:0:0:127.0.0.1:" << ports[0];
            else if (line.find("preprocessor") < line.length())
                newconfig << "node200=preprocessor:1:0:0:127.0.0.1:" << ports[1];
            else
                newconfig << line;                      // Unaffected line
                
            if (line.find("scheduler") > line.length()) // Add endl to all
                newconfig << endl;                      //  but last line
        }
        config.close();
    } else {
        throw "Unable to open file";                    // I/O Error
    }
    
    return success_code;
}
