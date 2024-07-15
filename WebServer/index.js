const express = require("express");
const { Server } = require("ws");
const path = require("path");

const app = express(); //creates a node.js application
const port = process.env.PORT || 5000;

// serve the index.html file in the public directory
app.use(express.static(path.join(__dirname, "public")));

// Start the server in port 5000
const server = app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});

// Create WebSocket server on port 5000 (ESP32-index.js communications)
const wss = new Server({ server });
// Create WebSocket server on port 3000 (index.js-index.html communications)
const wss2 = new Server({ port: 3000 });

wss.on("connection", (ws) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    //when a message arrives through the 5000 port (from ESP32)
    wss2.clients.forEach((client) => {
      //for every client connected to the server on port 3000
      client.send(`${message}`); //pass message (MSP432 command) to the clients
    });
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});

wss2.on("connection", (ws) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    //when a message arrives through the 3000 port (from client)
    wss.clients.forEach((client) => {
      //for every client connected to the server on port 5000
      client.send(`${message}`); //pass message (video information) to the clients
    });
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});
