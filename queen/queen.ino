// modified from RF24's getting started example

/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios. 
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two 
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting 
 * with the serial monitor and sending a 'T'.  The ping node sends the current 
 * time to the pong node, which responds by sending the value back.  The ping 
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

#define pipe_address 0xE7E7E7E7E7LL

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(6,10);

//
// Topology
//

// Open a reading address. This address needs to match the address that the ant is transmitting at
const uint64_t pipes[1] = { pipe_address };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.  
//

// The various roles supported by this sketch
// typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
// const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
//role_e role = role_pong_back;

void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();
  printf("\n\rQueen\n\r");
  printf("ROLE: Receiver\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();
  radio.setChannel(4);
  radio.setPayloadSize(4);
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_8);
  radio.openReadingPipe(1,pipe_address);
  // optionally, increase the delay between retries & # of retries
//  radio.setRetries(15,15);
  // radio.setPALevel(RF24_PA_MID);
//  radio.setDataRate(RF24_250KBPS);
  // optionally, reduce the payload size.  seems to
  // improve reliability
  // radio.setPayloadSize(8);

  // read at the pipe
//  radio.openReadingPipe(0,pipes[0]);
 
  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
}

void loop(void)
{
  // if there is data ready
    if ( radio.available() )
    {
//      printf("got something");
      // Dump the payloads until we've gotten everything
      unsigned long received_data;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &received_data, sizeof(unsigned long) );

        // Spew it
        printf("Got payload %lu...",received_data);

        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);
      }

      // // First, stop listening so we can talk
      // radio.stopListening();

      // // Send the final one back.
      // radio.write( &got_time, sizeof(unsigned long) );
      // printf("Sent response.\n\r");

      // // Now, resume listening so we catch the next packets.
      // radio.startListening();
    }
}
