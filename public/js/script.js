const tahun = document.querySelector(".tahun");
tahun.innerText = new Date().getFullYear();

const suhu = document.querySelector(".suhu");
const kelembaban = document.querySelector(".kelembaban");
const kipas = document.querySelector(".kipas");

const socket = io("http://localhost:3000");
socket.on("data", (data) => {
  console.log(data);
  suhu.textContent = data.suhu;
  kelembaban.textContent = data.kelembaban;
  kipas.textContent = data.kipas ? "ON" : "OFF";
});
