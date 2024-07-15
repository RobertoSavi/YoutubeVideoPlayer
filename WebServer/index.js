const express = require("express");
const { Server } = require("ws");
const path = require("path");

const app = express();
const port = process.env.PORT || 5000;

var messaggi = [];

// Serve il file HTML dalla directory "public"
app.use(express.static(path.join(__dirname, "public")));

// Avvia il server
const server = app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});

// Crea il server WebSocket
const wss = new Server({ server });
const wss2 = new Server({ port: 3000 });

wss.on("connection", (ws) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    console.log(`${message}`);
    wss2.clients.forEach((client) => {
      client.send(`${message}`);
    });
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});

wss2.on("connection", (ws) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    console.log(`${message}`);
    wss.clients.forEach((client) => {
      client.send(`${message}`);
    });
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});
