#include <enet/enet.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  const std::string option = argc > 1 ? argv[1] : "";

  if (enet_initialize() != 0) {
	std::cerr << "An error occurred while initializing ENet" << std::endl;
    return EXIT_FAILURE;
  }

  if (option == "server") {
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
    while (enet_host_service (server, & event, 1000) >= 0)
    {
        //std::cout << "=> check" << std::endl;
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "New client" << std::endl;
            printf ("A new client connected from %x:%u.\n", 
                    event.peer -> address.host,
                    event.peer -> address.port);

            /* Store any relevant client information here. */
            event.peer -> data = (char*)"Client information";

            break;

        case ENET_EVENT_TYPE_RECEIVE:
            printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
                    event.packet -> dataLength,
                    event.packet -> data,
                    event.peer -> data,
                    event.channelID);

            enet_peer_send (event.peer, 0, event.packet);
            //enet_host_flush (server);

            /* Clean up the packet now that we're done using it. */
            //enet_packet_destroy (event.packet);
            break;
           
        case ENET_EVENT_TYPE_DISCONNECT:
            printf ("%s disconected.\n", event.peer -> data);

            /* Reset the peer's client information. */

            event.peer -> data = NULL;
        }
    } // while
    enet_host_destroy(server);

  } else {
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

    /* Connect to some.server.net:1234. */
    enet_address_set_host (& address, "213.175.92.111");
    address.port = 9889;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    peer = enet_host_connect (client, & address, 2, 0);    
    
    if (!peer) {
      std::cout << "No available peers for initiating an ENet connection" << std::endl;
       exit (EXIT_FAILURE);
    }
    
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service (client, & event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to some.server.net:1234 succeeded" << std::endl;
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset (peer);

        puts ("Connection to some.server.net:1234 failed.");
    }

    for (size_t i(0); i != 1000; ++i) {
        std::cout << "=> sending: ";
        sleep(1);
      /* Create a reliable packet of size 7 containing "packet\0" */
      ENetPacket * packet = enet_packet_create ("packet", 
                                                strlen ("packet") + 1, 
                                                ENET_PACKET_FLAG_RELIABLE);

      /* Extend the packet so and append the string "foo", so it now */
      /* contains "packetfoo\0"                                      */
      enet_packet_resize (packet, strlen ("packetfoo") + 1);
      packet->data[0] = 'k';
      packet->data[1] = 0;
      //strcpy (& packet->data [strlen ("packet")], "foo");
      
      /* Send the packet to the peer over channel id 0. */
      /* One could also broadcast the packet by         */
      /* enet_host_broadcast (host, 0, packet);         */
      int retVal = enet_peer_send (peer, 0, packet);
      std::cout << retVal << std::endl;
      enet_host_flush (client);
      uint64_t time =  enet_time_get();
      if (enet_host_service (client, & event, 5000) <= 0) {
        std::cout << "no answer" << std::endl;
        return EXIT_FAILURE;
      } else {
        std::cout << "got package, check rtt.." << std::endl; 
      }
      std::cout << enet_time_get() - time << std::endl;
    }

    enet_host_destroy(client);
  }

  atexit (enet_deinitialize);
  return EXIT_SUCCESS;
} 
