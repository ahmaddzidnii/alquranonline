import { ReadlineParser, SerialPort } from "serialport";

import path from "path";
import { fileURLToPath } from "url";

import http from "http";
import express from "express";
import { Server } from "socket.io";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
const httpServer = http.createServer(app);

const io = new Server(httpServer);

// Fungsi untuk memformat data JSON
function formatJson(data) {
  try {
    const parsedData = JSON.parse(data);
    return {
      suhu: parsedData.suhu,
      kelembaban: parsedData.kelembaban,
      kipas: parsedData.kipas,
    };
  } catch (error) {
    console.error("Error parsing JSON:", error);
    return null;
  }
}

const port = new SerialPort({
  path: "COM5",
  baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

parser.on("data", (result) => {
  try {
    const { kelembaban, suhu, kipas } = formatJson(result);
    io.volatile.emit("data", {
      suhu,
      kelembaban,
      kipas,
    });
  } catch (error) {
    console.error(error);
  }
});

io.on("connection", (socket) => {
  console.log("a user connected", socket.id);

  socket.on("disconnect", (reason) => {
    console.log("user disconnected", reason);
  });
});

app.use(express.static(path.join(__dirname, "public")));

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/view/index.html");
});

httpServer.listen(3000, () => {
  console.log("Server is running on port 3000");
});
