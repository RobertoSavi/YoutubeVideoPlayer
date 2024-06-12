const express = require("express");
const { Server } = require("ws");
const path = require("path");

const app = express();
const port = process.env.PORT || 3000;

// Serve il file HTML dalla directory "public"
app.use(express.static(path.join(__dirname, "public")));

// Avvia il server
const server = app.listen(port, () => {
  console.log(`Server is listening on port ${port}`);
});

// Crea il server WebSocket
const wss = new Server({ server });

wss.on("connection", (ws) => {
  console.log("New client connected");

  ws.on("message", (message) => {
    console.log(`Received message: ${message}`);
    ws.send(`Server received: ${message}`);
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});
