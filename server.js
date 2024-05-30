import express from "express";
import path from "path";
import { fileURLToPath } from "url";
import http from "http";
import { Server } from "socket.io";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();
const httpServer = http.createServer(app);

const io = new Server(httpServer);

let suhu = 0;
let kelembaban = 0;
let kipas = false;

// Fungsi untuk memperbarui data setiap detik
const updateData = () => {
  suhu = Math.floor(Math.random() * 100);
  kelembaban = Math.floor(Math.random() * 100);
  kipas = suhu > 30;
};

// Interval untuk memperbarui data setiap detik
setInterval(() => {
  updateData();
  io.volatile.emit("data", {
    suhu,
    kelembaban,
    kipas,
  });
}, 1000);

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
