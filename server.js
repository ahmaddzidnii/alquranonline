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

let suhu = 0;
let kelembaban = 0;
let kipas = false;

function extractData(data) {
  const kelembabanMatch = data.match(/kelembaban:(\d+)/);
  const suhuMatch = data.match(/suhu:(\d+)/);

  if (kelembabanMatch && suhuMatch) {
    const kelembaban = parseInt(kelembabanMatch[1], 10);
    const suhu = parseInt(suhuMatch[1], 10);

    return { kelembaban, suhu };
  } else {
    throw new Error("Data format is incorrect");
  }
}

const port = new SerialPort({
  path: "COM5",
  baudRate: 9600,
});

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

parser.on("data", (result) => {
  // console.log(result); // kelembaban: 50suhu: 30
  try {
    const { kelembaban, suhu } = extractData(result);
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
