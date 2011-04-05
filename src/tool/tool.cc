#include <enet/enet.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <stdio.h>
#include <cassert>

void client(const std::string &arg);
void server();

int main(int argc, char *argv[]) {
  const std::string arg = argc > 1 ? argv[1] : "";

  if (enet_initialize() != 0) {
	std::cerr << "An error occurred while initializing ENet" << std::endl;
    return EXIT_FAILURE;
  }

  if (arg == "server") {
    server();
  } else {
    client(arg);
  }

  enet_deinitialize();
  return EXIT_SUCCESS;
} 

void server() {
  ENetAddress address;
  ENetHost *server; 
  
  /* Bind the server to the default localhost.     */
  /* A specific host address can be specified by   */
  /* enet_address_set_host (& address, "x.x.x.x"); */
  address.host = ENET_HOST_ANY;
  /* Bind the server to port 1234. */
  address.port = 9889;
  
  server = enet_host_create (& address /* the address to bind the server host to */, 
                             32      /* allow up to 32 clients and/or outgoing connections */,
                             2      /* allow up to 2 channels to be used, 0 and 1 */,
                             0      /* assume any amount of incoming bandwidth */,
                             0      /* assume any amount of outgoing bandwidth */);
  if (!server) {
    std::cerr << "An error occurred while trying to create an ENet server host" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  ENetEvent event;
  
  /* Wait up to 1000 milliseconds for an event. */
  while (true)
  {
    int ret = enet_host_service (server, &event, 10);
    if (ret > 0) {
      /* eat all events */
      do {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
          std::cout << "rx ping, loopback" << std::endl;
          enet_peer_send (event.peer, 0, event.packet);
          enet_host_flush(server);
        }
        else if (event.type == ENET_EVENT_TYPE_CONNECT) {
          std::cout << "new client" << std::endl;
        }
      } while(enet_host_check_events(server, &event) > 0);

    }
    
  } // while
  enet_host_destroy(server);
}

void client(const std::string &arg) {
  ENetHost * client;
  client = enet_host_create (NULL /* create a client host */,
                             1 /* only allow 1 outgoing connection */,
                             2 /* allow up 2 channels to be used, 0 and 1 */,
                             0 /* 56K modem with 56 Kbps downstream bandwidth */,
                             0 /* 56K modem with 14 Kbps upstream bandwidth */);
  if (!client) {
    std::cerr << "An error occurred while trying to create an ENet client host" << std::endl;
    exit (EXIT_FAILURE);
  } 
  
  ENetAddress address;
  ENetEvent event;
  ENetPeer *peer;

  std::string host = arg.size() ? arg : "localhost";
  enet_address_set_host (&address, host.c_str());
  address.port = 9889;
  
  /* Initiate the connection, allocating the two channels 0 and 1. */
  peer = enet_host_connect (client, &address, 2, 0);    
  
  if (!peer) {
    std::cout << "No available peers for initiating an ENet connection" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  /* Wait up to 5 seconds for the connection attempt to succeed. */
  if (enet_host_service (client, &event, 5000) > 0 &&
      event.type == ENET_EVENT_TYPE_CONNECT)
  {
    std::cout << "connection to " << host << " succeeded" << std::endl;
  } else {
    /* Either the 5 seconds are up or a disconnect event was */
    /* received. Reset the peer in the event the 5 seconds   */
    /* had run out without any significant event.            */
    enet_peer_reset (peer);
        
    puts ("Connection to some.server.net:1234 failed.");
  }
  
  int since_send = 0;
  while (true) {
    ++since_send;
    
    if (since_send >= 1000) {
      /* Create a reliable packet containing current time */
      long send_time = htonl(enet_time_get());
      ENetPacket * packet = enet_packet_create(&send_time, 
                                               sizeof(long), 
                                               ENET_PACKET_FLAG_UNSEQUENCED);
      std::cout << "ping..." << std::endl;
      enet_peer_send (peer, 0, packet);
      since_send = 0;
      // enet_host_flush (client);
    }      

    int ret = enet_host_service(client, &event, 1);
    if (ret < 0) {
      std::cerr << "failure to service host" << std::endl;
    }
    else if (ret > 0) {
      /* eat all events */
      do {
        if (event.type == ENET_EVENT_TYPE_RECEIVE) {
          assert(event.packet->dataLength == sizeof(long) && "size of long doesn't match");
          long ack_time = ntohl(*reinterpret_cast<long *>(event.packet->data));
          std::cout << "rx ping ack: " << enet_time_get() - ack_time << std::endl;
        }
      } while(enet_host_check_events(client, &event) > 0);
      
    }
    usleep(1000);
  }
  
  enet_host_destroy(client);
}

